#include <algorithm>
#include <vector>
#include <set>
#include <string>
#include <complex>
#include <cassert>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <parallel/algorithm>
#include "xrand.cpp"
#include "timer.cpp"
#include "constants.h"

using namespace std;

const uint INF = 0x3f3f3f3f;

struct Point {
    uint X;
    uint Y;
    double magnitude;

    Point() { throw "cannot initialize Point with no arguments"; }
    Point(uint x, uint y) : X(x), Y(y) {
        magnitude = sqrt(X * X + Y * Y);
    }
};

// global
Point zero_point(0, 0);
// 地点
vector<Point> points;
// 地点数
size_t pn;

double calc_dist(Point &a, Point &b) {
    auto dx = a.X - b.X;
    auto dy = a.Y - b.Y;
    return sqrt(dx * dx + dy * dy);
}

double calc_dist(size_t ai, size_t bi) {
    static vector<vector<double>> memo(500, vector<double>(500, -1));
    if (ai > bi) return calc_dist(bi, ai);
    if (memo[ai][bi] >= 0) return memo[ai][bi];
    return memo[ai][bi] = calc_dist(points[ai], points[bi]);
}

// ある地点からみた近い順の地点リスト
vector<vector<uint>> nears;

void init_nears() {
    nears.clear();
    nears.resize(pn);

    for (size_t i = 0; i < pn; ++i) {
        vector<pair<double, uint>> ns;
        ns.reserve(pn - 1);
        for (size_t j = 0; j < pn; ++j) {
            if (i != j) {
                ns.emplace_back(calc_dist(i, j), j);
            }
        }
        sort(ns.begin(), ns.end());

        vector<uint> ns2(pn - 1);
        for (size_t j = 0; j < pn - 1; ++j) {
            ns2[j] = ns[j].second;
        }

        nears[i] = ns2;
    }
}


typedef double Score;
typedef vector<uint> Order;

struct Path {
    Order order;
    Score score;

    Path(){}
    Path(Order order_) : order(order_), score(-1) {}
    Path(size_t n) : order(n), score(-1) {}
    Path(const Path &p) : order(p.order), score(p.score) {}

    Score calc_score() {
        if (score >= 0) return score;
        assert(order.size() == pn);

        score = 0;

        score += points[*(order.begin())].magnitude;
        score += points[*(order.rbegin())].magnitude;

        for (size_t i = 0; i < pn - 1; ++i) {
            score += calc_dist(order[i], order[i + 1]);
        }

        return score;
    }

    void reset_score() {
        score = -1;
    }

    bool validate_order() {
        assert(order.size() == pn);
        vector<bool> done(pn);
        for (auto p : order) done[p] = true;
        return count(done.begin(), done.end(), false) == 0;
    }

    uint hash() {
        uint seed = order.size();
        for(auto &x : order) {
            seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

// TSP GA Solver
class TspGaSolver {
public:
    // 現在の世代
    uint current_generation;

    // 個体数
    uint n;

    // 交叉数
    uint crossover_num;

    // 各個体のパス
    vector<Path> current_paths;

    // 計算用のパス
    vector<Path> temp_paths;

    // 最優個体
    Path best_path;

    // 突然変異確率(0.0~1.0)
    double p_mutation = 0.1;

    // ループ回数（パラメータ調整用）
    uint loop_num;

    // 個体のグループ数
    uint group_num;

    // 個体のグループ数の最大値
    uint max_group_num;

    TspGaSolver() { }

    // パラメーターの設定
    void configure(size_t n_, uint crossover_num_, double p_mutation_, uint loop_num_, uint max_group_num_) {
        n = n_;
        crossover_num = crossover_num_;

        assert(p_mutation_ >= 0);
        assert(p_mutation_ <= 1);
        p_mutation = p_mutation_;

        loop_num = loop_num_;

        max_group_num = max_group_num_;
    }

    void init() {
        best_path.score = numeric_limits<Score>::max();

        xrand.reset_by_now();
        current_generation = 0;

        update_group_num();

        string init_type(INIT_TYPE);
        if (init_type == "rand") init_paths();
        else if (init_type == "src") init_from_results();
        else throw "Invalid init type: " + init_type;

        update_best();
    }

    // 次の世代に進む
    void next_generation() {
        for (size_t i = 0; i < n + crossover_num; ++i) {
            temp_paths[i] = current_paths[i % n];
        }

        crossover();
        mutation();
        selection();
        update_best();

        ++current_generation;

        update_group_num();
    }

    void run() {
        while (current_generation < loop_num) {
            next_generation();
        }
    }

    void skip_to_next_group_num() {
        uint current_group_num = group_num;
        while (current_group_num == group_num) {
            ++current_generation;
            update_group_num();
        }
    }

private:
    XRand xrand;

    // 個体の初期化
    // 完全ランダム
    void init_paths() {
        current_paths.clear();
        current_paths.resize(n, Path(pn));
        temp_paths.clear();
        temp_paths.resize(n + crossover_num, Path(pn));

        for (size_t i = 0; i < n; ++i) {
            Order &order = current_paths[i].order;
            iota(order.begin(), order.end(), 0);
            xrand.shuffle(order.begin(), order.end());
        }
    }

    void read_order_from_result(string src_path, Order &order) {
        ifstream ifs;
        ifs.open(src_path, ios::in);

        string dummy;
        getline(ifs, dummy);
        getline(ifs, dummy);
        getline(ifs, dummy);

        uint idx, x, y;
        for (size_t j = 0; j < pn; ++j) {
            ifs >> idx >> y >> x;
            order[j] = idx;
        }
    }

    // 個体の初期化
    // results/ から取得
    void init_from_results() {
        current_paths.clear();
        current_paths.resize(n, Path(pn));
        temp_paths.clear();
        temp_paths.resize(n + crossover_num, Path(pn));

        string dirname(SRC_RESULT_DIR_PATH);
        DIR *dp;
        dirent* entry;

        dp = opendir(dirname.c_str());
        if (dp == NULL) exit(1);

        vector<string> filenames;

        uint read_i = 0;
        string filter(SRC_RESULT_FILE_FILTER);
        while (read_i < SRC_RESULT_FILE_MAX_NUM) {
            entry = readdir(dp);
            if (!entry) break;
            
            string filename = string(entry->d_name);

            if (filename.find(".") == 0) continue;

            // 多様性確保のため利用するファイルを絞る
            if (filter.size() && filename.find(filter) != string::npos) continue;
            
            filenames.push_back(filename);
            ++read_i;
        }

        uint fn = filenames.size();

        assert(fn > 0);

        sort(filenames.begin(), filenames.end());

        vector<size_t> idxes(n);
        iota(idxes.begin(), idxes.end(), 0);
        xrand.shuffle(idxes.begin(), idxes.end());

        set<uint> exists;

        size_t i = 0;
        size_t k = 0;
        while (i < n && k < fn) {
            string src_path = dirname + "/" + filenames[k];
            cout << "Read " << k << " " << i << " " << src_path << endl;
            ++k;

            Path &path = current_paths[idxes[i]];
            Order &order = path.order;

            read_order_from_result(src_path, order);

            auto hash = path.hash();
            if (exists.count(hash)) continue;
            exists.insert(hash);

            if (!path.validate_order()) {
                cout << "validate failed" << endl;
                continue;
            }

            ++i;
        }

        while (i < n) {
            Order &order = current_paths[idxes[i]].order;
            iota(order.begin(), order.end(), 0);
            xrand.shuffle(order.begin(), order.end());
            ++i;
        }
    }


    // 交叉
    void crossover() {
        string type(CROSSOVER_TYPE);

        for (uint gi = 0; gi < group_num; ++gi) {
            size_t gn0 = n / group_num;
            size_t gn = gn0;
            size_t gc0 = crossover_num / group_num;
            size_t gc = gc0;
            if (gi + 1 == group_num) {
                gn = n - gn * (group_num - 1);
                gc = crossover_num - gc * (group_num - 1);
            }

            uint ci_s = 0;
            while (ci_s < gc) {
                uint ci_size = min(er_static_size, (uint)gc - ci_s);

                #pragma omp parallel for
                for (uint i = 0; i < ci_size; ++i) {
                    uint ci = ci_s + i;
                    size_t a, b;
                    do {
                        a = gi * gn0 + xrand.next_uint(gn);
                        b = gi * gn0 + xrand.next_uint(gn);
                    } while (a == b);

                    size_t c = n + gc0 * gi + ci;

                    Order &order_a = temp_paths[a].order;
                    Order &order_b = temp_paths[b].order;
                    Path &new_path = temp_paths[c];
                    Order &new_order = new_path.order;

                    if (type == "ox") crossover_ox(order_a, order_b, new_order);
                    else if (type == "er") crossover_er(order_a, order_b, new_order, ci % er_static_size);
                    else throw "Invalid crossover type: " + type;

                    new_path.reset_score();
                }

                ci_s += ci_size;
            }
        }
    }

    // 交叉: 順序交叉(OX, Order crossover)
    void crossover_ox(Order &order_a, Order &order_b, Order &new_order) {
        size_t p, q;
        do {
            p = xrand.next_uint(pn);
            q = xrand.next_uint(pn);
        } while (p == q);
        if (p > q) swap(p, q);

        vector<bool> done(pn);

        for (size_t j = p; j <= q; ++j) {
            new_order[j] = order_a[j];
            done[order_a[j]] = true;
        }

        for (size_t j = 0, k = 0; j < pn; ) {
            if (p <= j && j <= q) {
                ++j;
                continue;
            }

            size_t pi = order_b[k];
            if (done[pi]) {
                ++k;
                continue;
            }

            new_order[j] = pi;

            ++j;
            ++k;
        }
    }

    uint er_static_size = 100;

    // 交叉: 辺組み合わせ交叉(ER, Edge recombination crossover)
    void crossover_er(Order &order_a, Order &order_b, Order &new_order, uint static_i) {
        using Edge = pair<uint, bool>;
        constexpr uint near_max_cnt = 1;
        // listは毎回メモリを確保すると重いので使い回す
        // es[from] = {to, is_multi}
        static vector<vector<vector<Edge>>> es_list(er_static_size,
                vector<vector<Edge>>(pn));
        static vector<vector<uint>> conn_cnts_list(er_static_size, vector<uint>(pn));
        static vector<vector<bool>> done_list(er_static_size, vector<bool>(pn));
        static vector<vector<uint>> nexts2_list(er_static_size,
                vector<uint>(near_max_cnt));

        vector<vector<Edge>> &es = es_list[static_i];
        vector<uint> &conn_cnts = conn_cnts_list[static_i];
        vector<bool> &done = done_list[static_i];
        vector<uint> &nexts2 = nexts2_list[static_i];

        // init list
        for (uint i = 0; i < pn; ++i) {
            es[i].resize(0);
        }

        for (uint i = 0; i + 1 < pn; ++i) {
            for (auto &fromto : {
                    make_pair(order_a[i], order_a[i + 1]), 
                    make_pair(order_a[i + 1], order_a[i]), 
                    make_pair(order_b[i], order_b[i + 1]), 
                    make_pair(order_b[i + 1], order_b[i])}) {
                uint from = fromto.first;
                uint to = fromto.second;

                auto exists = find_if(es[from].begin(), es[from].end(),
                        [to](const Edge &e){ return e.first == to; });

                if (exists == es[from].end()) {
                    es[from].emplace_back(to, false);
                } else {
                    exists->second = true;
                }
            }
        }

        for (uint i = 0; i < pn; ++i) {
            conn_cnts[i] = es[i].size();
            done[i] = false;
        }

        new_order.resize(0);

        uint pos = xrand.next_uint(2) ? order_a[0] : order_b[0];

        // 共通辺があればそれを使う
        // 最小の辺を持つ頂点を使う
        // 上2つを満たした頂点が複数あればランダム
        // 上を満たす頂点がなければ近い順に最大near_max_cnt個の頂点からランダム
        while(true) {
            done[pos] = true;

            new_order.push_back(pos);

            for (auto &to : es[pos]) {
                --conn_cnts[to.first];
            }

            if (new_order.size() == pn) break;

            tuple<bool, uint, uint, uint> next(true, INF, INF, INF);

            for (auto &e : es[pos]) {
                uint to = e.first;
                bool is_multi = e.second;
                if (done[to]) continue;
                tuple<bool, uint, uint, uint> temp(!is_multi, conn_cnts[to], xrand.next_uint(), to);
                if (temp < next) next = temp;
            }

            if (get<1>(next) != INF) {
                pos = get<3>(next);
            } else {
                nexts2.resize(0);
                for (uint next2 : nears[pos]) {
                    if (!done[next2]) {
                        nexts2.push_back(next2);
                        if (nexts2.size() == near_max_cnt) break;
                    }
                }
                pos = nexts2[xrand.next_uint(nexts2.size())];
            }

            assert(!done[pos]);
        }

        // uint done_cnt = count(done.begin(), done.end(), true);
        // assert(done_cnt == (uint)pn);
        // for (auto conn_cnt : conn_cnts) assert(!conn_cnt);
    }

    // 突然変異
    // 二点間を入れ替える or ある経路を逆順にする or 二点間の経路をシャッフルする
    void mutation() {
        #pragma omp parallel for
        for (size_t i = 0; i < temp_paths.size(); ++i) {
            auto path = temp_paths[i];
            if (!xrand.next_coin(p_mutation)) continue;

            size_t a, b;
            do {
                a = xrand.next_uint(pn);
                b = xrand.next_uint(pn);
            } while (a == b);

            uint k = xrand.next_uint(2);

            if (k == 0) {
                swap(path.order[a], path.order[b]);
            } else if (k == 1) {
                if (a > b) swap(a, b);
                reverse(path.order.begin() + a, path.order.begin() + b);
            }

            path.reset_score();
        }
    }

    // 淘汰
    // groupごとにElitism Selection
    // 同じデータは弾く（hashを使って同一判定）
    void selection() {
        for (size_t gi = 0; gi < group_num; ++gi) {
            size_t gn0 = n / group_num;
            size_t gn = gn0;
            size_t gc0 = crossover_num / group_num;
            size_t gc = gc0;
            if (gi + 1 == group_num) {
                gn = n - gn * (group_num - 1);
                gc = crossover_num - gc * (group_num - 1);
            }

            size_t m = gn + gc;
            
            // score, index
            vector<pair<Score, uint>> scores(m);
            #pragma omp parallel for
            for (size_t i = 0; i < gn; ++i) {
                uint j = gi * gn0 + i;
                assert(j < n + crossover_num);
                scores[i].first = temp_paths[j].calc_score();
                scores[i].second = j;
            }
            #pragma omp parallel for
            for (size_t i = 0; i < gc; ++i) {
                uint j = n + gi * gc0 + i;
                assert(j < n + crossover_num);
                scores[gn + i].first = temp_paths[j].calc_score();
                scores[gn + i].second = j;
            }

            sort(scores.begin(), scores.end());
            
            set<uint> exists;
            vector<bool> done(m);

            size_t i = 0;
            for (size_t j = 0; j < m && i < gn; ++j) {
                assert(gi * gn0 + i < n);
                assert(j < m);
                auto &path = temp_paths[scores[j].second];
                uint hash = path.hash();
                if (exists.count(hash)) continue;
                exists.insert(hash);
                current_paths[gi * gn0 + i] = path;
                done[j] = true;
                ++i;
            }

            for (size_t j = 0; j < m && i < gn; ++j) {
                assert(j < m);
                if (done[j]) continue;
                current_paths[gi * gn0 + i] = temp_paths[scores[i].second];
                done[j] = true;
                ++i;
            }
        }
    }

    void update_best() {
        vector<Score> scores(n);
        #pragma omp parallel for
        for (size_t i = 0; i < n; ++i) {
            scores[i] = current_paths[i].calc_score();
        }

        auto pos = min_element(scores.begin(), scores.end()) - scores.begin();

        if (scores[pos] < best_path.score) {
            best_path = current_paths[pos];
        }
    }

    void update_group_num() {
        if (current_generation >= loop_num) return;
        auto a = (loop_num - current_generation) * max_group_num;
        group_num = 1 << ((a + loop_num - 1) / loop_num - 1);
    }
};

void read_points() {
    cin >> pn;
    points.clear();
    for (size_t i = 0; i < pn; ++i) {
        uint y, x;
        cin >> y >> x;
        points.emplace_back(y, x);
    }

    init_nears();
}

void dump_path(Path &path) {
    cout << zero_point.Y << "\t" << zero_point.X << endl;
    for (auto pi : path.order) {
        auto &p = points[pi];
        cout << p.Y << "\t" << p.X << endl;
    }
    cout << zero_point.Y << "\t" << zero_point.X << endl;
}

void dump_to_file(Path &path, uint n, uint crossover_num, uint max_group_num, double p_mutation, uint generation_num, double solve_time) {
    static uint idx = 0;
    ++idx;

    string filename = string(RESULT_DIR_PATH) + "/path_" + 
        to_string((int)path.score) + 
        "_" + LOG_FILE_ID + "_" + 
        to_string(idx) + ".txt";
    cout << "dump to " << filename << endl;

    ofstream ofs;
    ofs.open(filename, ios::out);

    ofs << "n: " << n << 
        ", crossover_num: " << crossover_num << 
        ", p_mutation: " << p_mutation <<
        ", max_group_num: " << max_group_num <<
        ", generation_num: " << generation_num << 
        ", Time: " << solve_time << " seconds" <<
        ", Score: " << path.score << endl;

    ofs << endl;

    ofs << "\t" << zero_point.Y << "\t" << zero_point.X << endl;
    for (auto pi : path.order) {
        auto &p = points[pi];
        ofs << pi << "\t" << p.Y << "\t" << p.X << endl;
    }
    ofs << "\t" << zero_point.Y << "\t" << zero_point.X << endl;
}

int main(void) {
    string summary_filename = "log_summary_" + string(LOG_FILE_ID) + ".txt";
    ofstream summary_ofs;
    summary_ofs.open(summary_filename, ios::app);

    read_points();
    uint loop_n = LOOP_NUM;
    uint n = pn * K_INDIVIDUAL_NUM;
    uint crossover_num = n * K_CHILD_NUM;
    uint max_group_num = GROUP_NUM;
    double p_mutation = P_MUTATION;

    TspGaSolver solver;

    XRand xrand;
    xrand.reset_by_now();

    while (true) {
        cout << "n: " << n << ", " <<
            "crossover_num: " << crossover_num << ", " <<
            "p_mutation: " << p_mutation << ", " <<
            "max_group_num: " << max_group_num << endl;
        
        reset_time();
        start_time("solve");

        solver.configure(n, crossover_num, p_mutation, loop_n, max_group_num);
        solver.init();

        double before_score = -1;
        uint same_score_cnt = 0;

        while (true) {
            solver.next_generation();
            cout << "Generation: " << solver.current_generation << 
                " (" << (solver.current_generation * 100 / loop_n) << "%)" << 
                ", Score: " << solver.best_path.score <<
                ", Groups: " << solver.group_num <<
                ",  (n: " << n << 
                ", crossover_num: " << crossover_num <<
                ", p_mutation: " << p_mutation <<
                ", max_group_num: " << max_group_num <<
                ")" << endl;

            if (before_score == solver.best_path.score) {
                ++same_score_cnt;
                if (same_score_cnt >= CONTINUOUS_MAX_CNT) {
                    if (solver.group_num != 1) {
                        solver.skip_to_next_group_num();
                        cout << "skip" << endl;
                        same_score_cnt = 0;
                    } else {
                        break;
                    }
                }
            } else {
                same_score_cnt = 0;
                before_score = solver.best_path.score;
            }
        }

        end_time("solve");

        double solve_time = sum_time["solve"];

        summary_ofs << "n: " << n <<
            ", crossover_num: " << crossover_num <<
            ", p_mutation: " << p_mutation << 
            ", max_group_num: " << max_group_num <<
            ", Generation: " << solver.current_generation << 
            " (" << (solver.current_generation * 100 / loop_n) << "%)" << 
            ", Score: " << solver.best_path.score <<
            ", Time: " << solve_time << " seconds" <<
            endl;

        dump_to_file(solver.best_path, n, crossover_num, max_group_num, p_mutation, solver.current_generation, solve_time);
        break;
    }

    return 0;
}


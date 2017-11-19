// 個体数を決める係数 (頂点 * n)
#ifndef K_INDIVIDUAL_NUM
#define K_INDIVIDUAL_NUM 60
#endif

// 子の数を決める係数 (個体数 * n)
#ifndef K_CHILD_NUM
#define K_CHILD_NUM 1.0
#endif

// グループ数
#ifndef GROUP_NUM
#define GROUP_NUM 1
#endif

// 最初に指定するループ数
// 実際はCONTINUoUS_MAX_CNTを使って終了判定をする。
// この値はグループ数の変動に使う。
#ifndef LOOP_NUM
#define LOOP_NUM 1000
#endif

// 突然変異確率
#ifndef P_MUTATION
#define P_MUTATION 0.01
#endif

// 同スコアを何世代続けたら終了するか
#ifndef CONTINUOUS_MAX_CNT
#define CONTINUOUS_MAX_CNT 200
#endif

// 交叉アルゴリズムの方式
// "ox" or "er"
#ifndef CROSSOVER_TYPE
#define CROSSOVER_TYPE "er"
#endif

// 初期化の方式
// "rand" or "src"
#ifndef INIT_TYPE
#define INIT_TYPE "rand"
#endif

// 格納先のresultディレクトリパス
#ifndef RESULT_DIR_PATH
#define RESULT_DIR_PATH "results_dev"
#endif

// 初期化に使うresultディレクトリパス
#ifndef SRC_RESULT_DIR_PATH
#define SRC_RESULT_DIR_PATH "results"
#endif

// 初期化に使うresultファイルの最大数
// (n * (n - 1)) / 2 <= uint::max
#ifndef SRC_RESULT_FILE_MAX_NUM
#define SRC_RESULT_FILE_MAX_NUM 50000
#endif

// 初期化に使うresultファイル名のreject filter
// 空文字列でfilterなし
#ifndef SRC_RESULT_FILE_FILTER
#define SRC_RESULT_FILE_FILTER ""
#endif


// logファイルの識別名
// 例: v1_n12000_init2
#ifndef LOG_FILE_ID
#define LOG_FILE_ID "dev"
#endif

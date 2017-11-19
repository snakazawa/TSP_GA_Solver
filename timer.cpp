#include <iostream>
#include <chrono>
#include <string>
#include <map>

using namespace std;

typedef chrono::high_resolution_clock myclock;

// 時間計測用
map<string, double> sum_time;
map<string, uint> cnt_time;
map<string, myclock::time_point> temp_time;

void start_time(string title = "") {
    temp_time[title] = myclock::now();
}

void end_time(string title = "") {
    myclock::duration d = myclock::now() - temp_time[title];
    double dsec = d.count() / 1000.0 / 1000.0 / 1000.0;

    if (sum_time.count(title)) sum_time[title] += dsec;
    else sum_time[title] = dsec;

    if (cnt_time.count(title)) cnt_time[title] += 1;
    else cnt_time[title] = 1;
}

double avg_time(string title = "") {
    return sum_time[title] / cnt_time[title];
}

void dump_time(string title = "") {
    cout << "[" << title << "] " <<
        "Avg: " << avg_time(title) << 
        ", Sum: " << sum_time[title] << 
        ", Cnt: " << cnt_time[title] << 
        endl;
}

void reset_time() {
    sum_time.clear();
    temp_time.clear();
    cnt_time.clear();
}


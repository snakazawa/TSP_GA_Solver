#include <chrono>
#include <complex>

using namespace std;

class XRand {
public:
    XRand(uint64_t seed = 0) {
        reset(seed);
    }

    void reset(uint64_t seed) {
        x_ = 314159265;
        y_ = 358979323;
        z_ = 846264338 ^ (65535 * static_cast<uint32_t>(seed >> 32));
        w_ = 327950288 ^ (65535 * static_cast<uint32_t>(seed));
    }

    void reset_by_now() {
        typedef chrono::high_resolution_clock myclock;
        myclock::time_point now = myclock::now();
        chrono::seconds s = chrono::duration_cast<chrono::seconds>(now.time_since_epoch());
        reset(s.count());
    }

    uint32_t next_uint() {
        const uint32_t t = x_ ^ x_ << 11;
        x_ = y_;
        y_ = z_;
        z_ = w_;
        return w_ = w_ ^ w_ >> 19 ^ t ^ t >> 8;
    }

    uint32_t next_uint(uint32_t m) {
        return next_uint() % m;
    }

    // return x (0 <= x <= 1)
    double next_p() {
        static constexpr uint32_t maxp = numeric_limits<uint32_t>::max() - 1;
        return 1. * next_uint(maxp + 1) / maxp;
    }

    bool next_coin(double p, double eps = 1e-8) {
        double x = next_p();
        return abs(p - x) < eps || x + eps < p;
    }

    template <typename RandomAccessIterator>
    void shuffle(RandomAccessIterator first, RandomAccessIterator last) {
        for (RandomAccessIterator iter = first; iter != last; ++iter) {
            iter_swap(first + next_uint(iter - first + 1), iter);
        }
    }

private:
    uint32_t x_, y_, z_, w_;
};


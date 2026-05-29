// Alexander BAKALOV
// Tifenn FABRICI

#ifndef STATS_H
#define STATS_H

#include <chrono>
#include <vector>

namespace simulationSDR {

enum BlockType {
    BLOCK_SOURCE = 0,
    BLOCK_ENCODER,
    BLOCK_MODULATOR,
    BLOCK_CHANNEL,
    BLOCK_DEMODULATOR,
    BLOCK_DECODER,
    BLOCK_MONITOR,
    BLOCK_COUNT
};

#ifdef ENABLE_STATS

struct BlockStats {
    double total_time = 0.0;
    double min_time = 1e9;
    double max_time = 0.0;
    uint64_t count = 0;

    void update(double duration) {
        total_time += duration;
        if (duration < min_time) min_time = duration;
        if (duration > max_time) max_time = duration;
        count++;
    }
};

struct ThreadStats {
    BlockStats blocks[BLOCK_COUNT];
};

class ScopedTimer {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    BlockStats& stats;
public:
    ScopedTimer(BlockStats& s) : stats(s) {
        start_time = std::chrono::high_resolution_clock::now();
    }
    ~ScopedTimer() {
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end_time - start_time;
        stats.update(diff.count());
    }
};

#else /* ENABLE_STATS not defined: provide no-ops */

struct BlockStats {};
struct ThreadStats {
    BlockStats blocks[BLOCK_COUNT];
};

class ScopedTimer {
public:
    ScopedTimer(BlockStats& s) { (void)s; }
};

#endif // ENABLE_STATS

} // namespace simulationSDR

#endif // STATS_H

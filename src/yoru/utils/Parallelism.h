#pragma once
#include "base/Vector.h"
#include "base/BoundingBox.h"

#include <atomic>
#include <functional>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
inline void AtomicAdd(std::atomic<double> &a, double v) {
    for (double g=a; !a.compare_exchange_weak(g, g + v);) {
    }
}

class ParallelJob {
    friend class ThreadPool;
    friend class Parallelism;

public:
    virtual bool HaveWork() = 0;
    virtual void RunStep(std::unique_lock<std::mutex> *lock) = 0;
    bool Finished() { return !HaveWork() && activeWorker == 0; }

    static class ThreadPool *threadPool;

private:
    ParallelJob *prevJob = nullptr;
    ParallelJob *nextJob = nullptr;

    int activeWorker = 0;
};

class ThreadPool {
public:
    explicit ThreadPool(int nThreads);
    void Worker();
    void WorkOrWait(std::unique_lock<std::mutex> *lock);

    std::unique_lock<std::mutex> AddJob(ParallelJob *job);
    void RemoveJob(ParallelJob *job);

private:
    int nThreads = 8;
    std::vector<std::thread> threads;

    std::mutex mutex;
    bool shutdownThreads = false;

    ParallelJob *jobList = nullptr;
    std::condition_variable jobListCV;
};

class ParrallelForLoop1D : public ParallelJob {
public:
    ParrallelForLoop1D(uint64_t startIndex, uint64_t endIndex, uint64_t chunckSize, std::function<void(uint64_t, uint64_t)> func);
    virtual bool HaveWork();
    virtual void RunStep(std::unique_lock<std::mutex> *lock);

private:
    uint64_t startIndex;
    uint64_t endIndex;
    uint64_t chunkSize;
    std::function<void(uint64_t, uint64_t)> func;
    uint64_t nextIndex;
};

class ParrallelForLoop2D : public ParallelJob {
public:
    ParrallelForLoop2D(BBox2i extent, int chunkSize,std::function<void(BBox2i tile)> func);
    virtual bool HaveWork();
    virtual void RunStep(std::unique_lock<std::mutex> *lock);

private:
    BBox2i extent;
    int chunkSize;
    std::function<void(BBox2i tile)> func;
    Point2i nextTile;
};

extern void ParrallelFor(uint64_t start, uint64_t end, std::function<void(uint64_t, uint64_t)> func);
extern void ParrallelFor(uint64_t start, uint64_t end, std::function<void(uint64_t)> func);
extern void ParrallelFor2D(BBox2i extent, std::function<void(BBox2i)> func);

class Parallelism {
public:
    struct Config {
        int nThreads;
    };
    static void Init(Config &config) {
        Parallelism::config = config;
        ParallelJob::threadPool = new ThreadPool(config.nThreads);
    }
    static int NThreads() {
        return config.nThreads;
    }

    static Config config;
};
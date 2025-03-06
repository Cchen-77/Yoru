#include "Parallelism.h"
Parallelism::Config Parallelism::config;
ThreadPool *ParallelJob::threadPool = nullptr;

ThreadPool::ThreadPool(int nThreads) : nThreads(nThreads) {
    for (int i = 0; i < nThreads - 1; ++i) {
        threads.push_back(std::thread(&ThreadPool::Worker, this));
    }
}

void ThreadPool::Worker() {
    std::unique_lock<std::mutex> lock(mutex);
    while (!shutdownThreads) {
        WorkOrWait(&lock);
    }
}

void ThreadPool::WorkOrWait(std::unique_lock<std::mutex> *lock) {
    ParallelJob *job = jobList;
    while (job && !job->HaveWork()) {
        job = job->nextJob;
    }
    if (job) {
        job->activeWorker++;
        job->RunStep(lock);

        lock->lock();
        job->activeWorker--;
        if (job->Finished()) {
            jobListCV.notify_all();
        }

    } else {
        jobListCV.wait(*lock);
    }
}

std::unique_lock<std::mutex> ThreadPool::AddJob(ParallelJob *job) {
    std::unique_lock<std::mutex> lock(mutex);
    if (job) {
        job->prevJob = nullptr;
        job->nextJob = jobList;
        jobList = job;

        jobListCV.notify_all();
    }
    return lock;
}

void ThreadPool::RemoveJob(ParallelJob *job) {
    if (job) {
        if (job == jobList) {
            if (job->nextJob) {
                job->nextJob->prevJob = nullptr;
                jobList = job->nextJob;
            } else {
                jobList = nullptr;
            }
        } else {
            job->prevJob->nextJob = job->nextJob;
            if (job->nextJob) {
                job->nextJob->prevJob = job->prevJob;
            }
        }
    }
}

ParrallelForLoop1D::ParrallelForLoop1D(uint64_t startIndex, uint64_t endIndex, uint64_t chunkSize, std::function<void(uint64_t, uint64_t)> func)
    : startIndex(startIndex), endIndex(endIndex), chunkSize(chunkSize), func(std::move(func)), nextIndex(startIndex) {
}

bool ParrallelForLoop1D::HaveWork() {
    return nextIndex < endIndex;
}

void ParrallelForLoop1D::RunStep(std::unique_lock<std::mutex> *lock) {
    uint64_t start = nextIndex, end = std::min(endIndex, nextIndex + chunkSize);
    nextIndex = end;
    if (!HaveWork()) {
        threadPool->RemoveJob(this);
    }
    lock->unlock();
    func(start, end);
}

void ParrallelFor(uint64_t start, uint64_t end, std::function<void(uint64_t, uint64_t)> func) {
    if (start >= end) {
        return;
    }
    uint64_t chunkSize = std::max(1ull, (end - start) / (8 * Parallelism::NThreads()));
    auto loop = ParrallelForLoop1D(start, end, chunkSize, func);

    auto lock = ParallelJob::threadPool->AddJob(&loop);

    while (!loop.Finished()) {
        ParallelJob::threadPool->WorkOrWait(&lock);
    }
}
void ParrallelFor(uint64_t start, uint64_t end, std::function<void(uint64_t)> func) {
    ParrallelFor(start, end, [&func](uint64_t start, uint64_t end) {
        for (uint64_t i = start; i < end; ++i) {
            func(i);
        }
    });
}


ParrallelForLoop2D::ParrallelForLoop2D(BBox2i extent, int chunkSize, std::function<void(BBox2i tile)> func) : extent(extent), chunkSize(chunkSize), func(std::move(func)) {
    nextTile = extent.pMin;
}

bool ParrallelForLoop2D::HaveWork() {
    return extent.isInside(nextTile);
}

void ParrallelForLoop2D::RunStep(std::unique_lock<std::mutex> *lock) {
    Point2i tileStart = nextTile, tileEnd = nextTile + Vector2i{chunkSize, chunkSize};
    BBox2i tile(tileStart, tileEnd);
    tile.Intersect(extent);

    assert(!tile.isEmpty());

    nextTile.x += chunkSize;
    if (nextTile.x >= extent.pMax.x) {
        nextTile.x = extent.pMin.x;
        nextTile.y += chunkSize;
    }

    if (!HaveWork()) {
        threadPool->RemoveJob(this);
    }
    lock->unlock();
    func(tile);
}

void ParrallelFor2D(BBox2i extent, std::function<void(BBox2i)> func) {
    if (extent.isEmpty()) {
        return;
    }
    Vector2i diagonal = extent.pMax - extent.pMin;
    int chunkSize = std::sqrt(diagonal.x * diagonal.y / (8 * Parallelism::NThreads()));
    chunkSize = std::clamp(chunkSize,1,32);

    auto loop = ParrallelForLoop2D(extent, chunkSize, func);

    auto lock = ParallelJob::threadPool->AddJob(&loop);

    while (!loop.Finished()) {
        ParallelJob::threadPool->WorkOrWait(&lock);
    }
}
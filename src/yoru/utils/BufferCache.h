#pragma once
#include "Hash.h"
#include "base/Vector.h"

#include <cstdint>
#include <cstring>
#include <span>
#include <unordered_set>
#include <shared_mutex>
template<typename T>
class BufferCache {
public:
    const T *LookupOrAdd(std::span<const T> buf) {
        Buffer lookUpBuffer(buf.data(), buf.size());
        int shardIndex = uint32_t(lookUpBuffer.hash) >> (32 - logNShard);
        mutex[shardIndex].lock_shared();
        if (auto iter = cache[shardIndex].find(lookUpBuffer);
            iter != cache[shardIndex].end()) {
            const T *cachedPtr = iter->ptr;
            mutex[shardIndex].unlock_shared();
            return cachedPtr;
        }

        mutex[shardIndex].unlock_shared();
        T *ptr = new T[buf.size()];
        std::copy(buf.begin(), buf.end(), ptr);
        mutex[shardIndex].lock();
        if (auto iter = cache[shardIndex].find(lookUpBuffer);
            iter != cache[shardIndex].end()) {
            const T *cachedPtr = iter->ptr;
            mutex[shardIndex].unlock();
            delete[] ptr;
            return cachedPtr;
        }
        cache[shardIndex].insert(Buffer(ptr, buf.size()));
        mutex[shardIndex].unlock();
        return ptr;
    }

private:
    struct Buffer {
        Buffer(const T *ptr, size_t size) : ptr(ptr), size(size) {
            hash = HashBuffer(ptr, size);
        }
        bool operator==(const Buffer &other) const{
            return hash == other.hash && size == other.size && std::memcmp(ptr, other.ptr, sizeof(T) * size) == 0;
        }
        const T *ptr;
        size_t size;
        size_t hash;
    };
    struct BufferHasher {
        size_t operator()(const Buffer &buffer) const {
            return buffer.hash;
        }
    };

    static constexpr int logNShard = 6;
    static constexpr int nShard = 1 << logNShard;
    std::shared_mutex mutex[nShard];
    std::unordered_set<Buffer, BufferHasher> cache[nShard];
};

extern BufferCache<int> intBufferCache;
extern BufferCache<Point3> point3BufferCache;
extern BufferCache<Vector3> vector3BufferCache;
extern BufferCache<Point2> point2BufferCache;
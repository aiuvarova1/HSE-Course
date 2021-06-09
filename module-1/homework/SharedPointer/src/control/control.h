#pragma once

#include <atomic>

class SharedCount {
public:

    SharedCount() = default;

    explicit SharedCount(size_t refs) : strongRefCount(refs) {};

    virtual ~SharedCount() = default;

    void IncreaseShared() {
        strongRefCount++;
    }

    void DecreaseShared() {
        strongRefCount--;
    }

    size_t GetShared() {
        return strongRefCount;
    }

protected:
    std::atomic<size_t> strongRefCount{0};

};

class SharedWeakCount : public SharedCount {
public:
    SharedWeakCount() = default;

    virtual ~SharedWeakCount() = default;

    void IncreaseWeak() {
        weakRefCount++;
    }

    void DecreaseWeak() {
        weakRefCount--;
    }

    size_t GetWeak() {
        return weakRefCount;
    }

protected:
    std::atomic<size_t> weakRefCount{0};

};

template<typename T, typename Deleter=std::default_delete<T*>>
class ControlBlock : public SharedWeakCount {
public:
    ControlBlock() = default;

    explicit ControlBlock(T& obj) : managed_object_(obj) {};

    ControlBlock(T& obj, Deleter& d) : managed_object_(obj), deleter_(d) {};

private:
    T managed_object_;
    Deleter deleter_;
};
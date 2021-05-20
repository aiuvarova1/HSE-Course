#pragma once

#include <atomic>

class SharedCount {
public:

    SharedCount() : strongRefCount(0) {};

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
    std::atomic<std::size_t> strongRefCount;

};

class SharedWeakCount : public SharedCount {
public:
    SharedWeakCount() : weakRefCount(0) {};

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
    std::atomic<std::size_t> weakRefCount;

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
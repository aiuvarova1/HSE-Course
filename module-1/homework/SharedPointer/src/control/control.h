#pragma once

#include <atomic>

class SharedCount {
public:

    SharedCount() : strongRefCount(0) {};

    explicit SharedCount(size_t refs) : strongRefCount(refs) {};

    virtual ~SharedCount() = default;

    void increaseShared() {
        strongRefCount++;
    }

    void decreaseShared() {
        strongRefCount--;
    }

    size_t getShared() {
        return strongRefCount;
    }

protected:
    std::atomic<std::size_t> strongRefCount;

};

class SharedWeakCount : public SharedCount {
public:
    SharedWeakCount() : weakRefCount(0) {};

    virtual ~SharedWeakCount() = default;

    void increaseWeak() {
        weakRefCount++;
    }

    void decreaseWeak() {
        weakRefCount--;
    }

    size_t getWeak() {
        return weakRefCount;
    }

protected:
    std::atomic<std::size_t> weakRefCount;

};

template<typename T, typename Deleter=std::default_delete<T*>>
class ControlBlock : public SharedWeakCount {
public:
    ControlBlock() = default;
    ControlBlock(T obj) : managedObject(obj) {};
    ControlBlock(T obj, Deleter d) : managedObject(obj), deleter(d) {};

private:
    T managedObject;
    Deleter deleter;
};
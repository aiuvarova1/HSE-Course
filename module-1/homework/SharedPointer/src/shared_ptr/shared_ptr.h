#pragma once

#include "../control/control.h"

// SharedPtr
template<typename T>
class WeakPtr;

template<typename T>
class SharedPtr {
public:
    using element_type = T;
    using weak_type = WeakPtr<T>;

    constexpr SharedPtr() noexcept = default;

    ~SharedPtr();

    template<typename Y>
    explicit SharedPtr(Y* p);

    template<typename Y, typename Deleter>
    SharedPtr(Y* p, Deleter deleter) noexcept;

    SharedPtr(const SharedPtr& other) noexcept;

    SharedPtr(SharedPtr&& other) noexcept;

    SharedPtr& operator=(const SharedPtr& r) noexcept;

    template<typename Y>
    SharedPtr& operator=(const SharedPtr<Y>& r) noexcept;

    SharedPtr& operator=(SharedPtr&& r) noexcept;

    template<typename Y>
    SharedPtr& operator=(SharedPtr<Y>&& r) noexcept;

    // Modifiers
    void Reset() noexcept;

    template<typename Y>
    void Reset(Y* p) noexcept;

    template<typename Y, typename Deleter>
    void Reset(Y* p, Deleter deleter) noexcept;

    void Swap(SharedPtr& other) noexcept;

    // Observers
    T* Get() const noexcept;

    int64_t UseCount() const noexcept;

    T& operator*() const noexcept;

    T* operator->() const noexcept;

    element_type& operator[](std::ptrdiff_t idx) const;

    explicit operator bool() const noexcept;

    template<typename U>
    friend
    class WeakPtr;

private:
    T* pointer_ = nullptr;
    SharedWeakCount* block_ = nullptr;
};

// MakeShared
template<typename T, class... Args>
SharedPtr<T> MakeShared(Args&& ... args) {
    return SharedPtr<T>(::new T(std::forward<Args>(args)...));
}
// MakeShared

// SharedPtr

template<typename T>
SharedPtr<T>::~SharedPtr() {
    if (block_ != nullptr) {
        block_->DecreaseShared();
    }
    block_ = nullptr;
    pointer_ = nullptr;
}

template<typename T>
template<typename Y>
SharedPtr<T>::SharedPtr(Y* ptr): pointer_(ptr), block_(new ControlBlock(ptr)) {
    block_->IncreaseShared();
}

template<typename T>
template<typename Y, typename Deleter>
SharedPtr<T>::SharedPtr(Y* ptr, Deleter deleter) noexcept :
        pointer_(ptr), block_(new ControlBlock(ptr, deleter)) {
    block_->IncreaseShared();
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& other) noexcept : pointer_(other.pointer_), block_(other.block_) {
    block_->IncreaseShared();
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr&& other) noexcept : pointer_(other.pointer_), block_(other.block_) {
    other.block_ = nullptr;
    other.pointer_ = nullptr;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& r) noexcept {
    SharedPtr<T>(r).Swap(*this);
    return *this;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& r) noexcept {
    SharedPtr<T>(std::move(r)).Swap(*this);
    return *this;
}

template<typename T>
void SharedPtr<T>::Reset() noexcept {
    SharedPtr<T>().Swap(*this);
}

template<typename T>
template<typename Y>
void SharedPtr<T>::Reset(Y* p) noexcept {
    SharedPtr<T>(p).Swap(*this);
}

template<typename T>
template<typename Y, typename Deleter>
void SharedPtr<T>::Reset(Y* p, Deleter deleter) noexcept {
    SharedPtr<T>(p, deleter).Swap(*this);
}

template<typename T>
void SharedPtr<T>::Swap(SharedPtr& other) noexcept {
    std::swap(other.block_, block_);
    std::swap(other.pointer_, pointer_);
}

template<typename T>
T* SharedPtr<T>::Get() const noexcept {
    return pointer_;
}

template<typename T>
int64_t SharedPtr<T>::UseCount() const noexcept {
    return block_ ? block_->GetShared() : 0;
}

template<typename T>
T& SharedPtr<T>::operator*() const noexcept {
    return *pointer_;
}

template<typename T>
T* SharedPtr<T>::operator->() const noexcept {
    return Get();
}

template<typename T>
typename SharedPtr<T>::element_type& SharedPtr<T>::operator[](std::ptrdiff_t idx) const {
    return pointer_ ? pointer_[idx] : nullptr;
}

template<typename T>
SharedPtr<T>::operator bool() const noexcept {
    return pointer_ != nullptr;
}
// SharedPtr

// WeakPtr
template<typename T>
class WeakPtr {

public:
    using element_type = T;

    // Special-member functions
    constexpr WeakPtr() noexcept = default;

    template<typename Y>
    explicit WeakPtr(const SharedPtr<Y>& other);

    WeakPtr(const WeakPtr& other) noexcept;

    WeakPtr(WeakPtr&& other) noexcept;

    template<typename Y>
    WeakPtr& operator=(const SharedPtr<Y>& other);

    WeakPtr& operator=(const WeakPtr& other) noexcept;

    WeakPtr& operator=(WeakPtr&& other) noexcept;

    ~WeakPtr();

    // Modifiers
    void Reset() noexcept;

    void Swap(WeakPtr<T>& other) noexcept;

    // Observers
    bool Expired() const noexcept;

    SharedPtr<T> Lock() const noexcept;

    template<typename U>
    friend
    class SharedPtr;

public:
    SharedWeakCount* block_ = nullptr;
    T* pointer_ = nullptr;
};


// WeakPtr
template<typename T>
template<typename Y>
WeakPtr<T>::WeakPtr(const SharedPtr<Y>& other) : pointer_(other.pointer_), block_(other.block_) {
    if (block_ != nullptr) {
        block_->IncreaseWeak();
    }
}

template<typename T>
WeakPtr<T>::WeakPtr(const WeakPtr& other) noexcept : pointer_(other.pointer_), block_(other.block_) {
    if (block_ != nullptr) {
        block_->IncreaseWeak();
    }
}

template<typename T>
WeakPtr<T>::WeakPtr(WeakPtr&& other) noexcept : pointer_(other.pointer_), block_(other.block_) {
    other.block_ = nullptr;
    other.pointer_ = nullptr;
}

template<typename T>
template<typename Y>
WeakPtr<T>& WeakPtr<T>::operator=(const SharedPtr<Y>& other) {
    WeakPtr<T>(other).Swap(*this);
    return *this;
}

template<typename T>
WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr& other) noexcept {
    WeakPtr<T>(other).Swap(*this);
    return *this;
}

template<typename T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr&& other) noexcept {
    WeakPtr<T>(std::move(other)).Swap(*this);
    return *this;
}

template<typename T>
WeakPtr<T>::~WeakPtr() {
    Reset();
}

template<typename T>
void WeakPtr<T>::Reset() noexcept {
    if (block_ != nullptr) {
        block_->DecreaseWeak();
    }
    block_ = nullptr;
    pointer_ = nullptr;
}

template<typename T>
void WeakPtr<T>::Swap(WeakPtr<T>& other) noexcept {
    std::swap(other.pointer_, this->pointer_);
    std::swap(other.block_, this->block_);
}

template<typename T>
bool WeakPtr<T>::Expired() const noexcept {
    return block_ == nullptr || block_->GetShared() == 0;
}

template<typename T>
SharedPtr<T> WeakPtr<T>::Lock() const noexcept {
    return Expired() ? SharedPtr<T>() : SharedPtr<T>(pointer_);
}
// WeakPtr
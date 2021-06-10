#include <memory>
#include <type_traits>

template <typename T>
class CustomAllocator {
public:
    template <typename U>
    struct rebind {  // NOLINT
        using other = CustomAllocator<U>;
    };

    using value_type = T;
    using pointer = T*;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::false_type;
    using propagate_on_container_swap = std::true_type;

    CustomAllocator() = default;

    CustomAllocator(const CustomAllocator& other) noexcept;

    ~CustomAllocator();

    template <typename U>
    explicit CustomAllocator(const CustomAllocator<U>& other) noexcept;

    T* allocate(size_t n) {  // NOLINT
        if (n == 0 || *cur_size_ + n > kMaxSize) {
            return nullptr;
        }

        *cur_size_ += n;
        return static_cast<T*>(data_) + (*cur_size_ - n);
    }

    void deallocate(T* p, size_t n){
        // NOLINT
        // no realization for linear allocator
    };

    template <typename... Args>
    void construct(pointer p, Args&&... args) {  // NOLINT
        new (static_cast<void*>(p)) T{std::forward<Args>(args)...};
    };

    void destroy(pointer p) {  // NOLINT
        p->~T();
    };

    template <typename K, typename U>
    friend bool operator==(const CustomAllocator<K>& lhs, const CustomAllocator<U>& rhs) noexcept;

    template <typename K, typename U>
    friend bool operator!=(const CustomAllocator<K>& lhs, const CustomAllocator<U>& rhs) noexcept;

private:
    static const size_t kMaxSize{10000};

    void* data_{::operator new(sizeof(T) * kMaxSize)};
    size_t* cur_size_{new size_t(0)};
    size_t* refs_{new size_t(1)};

public:
    void* GetData() const {
        return data_;
    }

    size_t* GetSize() const {
        return cur_size_;
    }

    size_t GetMaxSize() const {
        return kMaxSize;
    }

    size_t* GetRefs() const {
        return refs_;
    }
};

template <typename T, typename U>
bool operator==(const CustomAllocator<T>& lhs, const CustomAllocator<U>& rhs) noexcept {
    return lhs.data_ == rhs.data_;
}

template <typename T, typename U>
bool operator!=(const CustomAllocator<T>& lhs, const CustomAllocator<U>& rhs) noexcept {
    return lhs.data_ != rhs.data_;
}

template <typename T>
CustomAllocator<T>::CustomAllocator(const CustomAllocator& other) noexcept
    : data_(other.GetData()), cur_size_(other.GetSize()), refs_(other.GetRefs()) {
    *refs_ += 1;
}

template <typename T>
template <typename U>
CustomAllocator<T>::CustomAllocator(const CustomAllocator<U>& other) noexcept
    : data_(other.GetData()), cur_size_(other.GetSize()), refs_(other.GetRefs()) {
    *refs_ += 1;
}

template <typename T>
CustomAllocator<T>::~CustomAllocator() {
    *refs_ -= 1;
    if (*refs_ == 0) {
        ::operator delete(data_);
        delete refs_;
        delete cur_size_;
    }
}

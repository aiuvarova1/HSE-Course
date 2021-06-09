#pragma once

#include <list>
#include <memory>
#include <type_traits>

namespace task {

template <typename T, typename Allocator = std::allocator<T>>
class List {

public:
    class Iterator;

    using value_type = T;
    using allocator_type = Allocator;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using iterator = Iterator;
    using const_iterator = const Iterator;

    // Special member functions
    List();

    List(const List& other);

    List(const List& other, const Allocator& alloc);

    List(List&& other);

    List(List&& other, const Allocator& alloc);

    ~List();

    List& operator=(const List& other);

    List& operator=(List&& other) noexcept;

    // Element access
    reference Front();

    const_reference Front() const;

    reference Back();

    const_reference Back() const;

    // Iterators
    iterator Begin() noexcept;

    const_iterator Begin() const noexcept;

    iterator End() noexcept;

    const Iterator End() const noexcept;

    // Capacity
    bool Empty() const noexcept;

    size_type Size() const noexcept;

    size_type MaxSize() const noexcept;

    // Modifiers
    void Clear();

    void Swap(List& other) noexcept;

    void PushBack(const T& value);

    void PushBack(T&& value);

    template <typename... Args>
    void EmplaceBack(Args&&... args);

    void PopBack();

    void PushFront(const T& value);

    void PushFront(T&& value);

    template <typename... Args>
    void EmplaceFront(Args&&... args);

    void PopFront();

    void Resize(size_type count);

    // Operations
    void Remove(const T& value);

    void Unique();

    void Sort();

    allocator_type GetAllocator() const noexcept;

private:
    struct Node {
        value_type _value;
        Node* _prev;
        Node* _next;

        Node() : _value(), _prev(nullptr), _next(nullptr){};

        explicit Node(value_type value, Node* prev = nullptr, Node* next = nullptr)
            : _value(value), _prev(prev), _next(next) {
        }

        ~Node() = default;
    };

    typedef typename std::allocator_traits<Allocator>::template rebind_alloc<Node> node_allocator;
    typedef typename std::allocator_traits<node_allocator> allocator_traits;
    node_allocator allocator_;

    Node* nil_;

    size_t size_;

    // returns prev node_
    Node* RemoveNode(Node* to_remove);

    void QuickSort(Node* start, Node* end);

    Node* Partition(Node* start_node, Node* end_node);

    void Swap(Node* first, Node* second);

    void Clone(List<T, Allocator>& other);

    void BaseInit();

public:
    class Iterator {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::bidirectional_iterator_tag;

        Iterator() : node_(nullptr){};

        explicit Iterator(Node* value) : node_(value){};

        explicit Iterator(Iterator* other) : node_(other->node_){};

        reference GetReference() {
            return node_->_value;
        }

        value_type GetValue() {
            return node_->_value;
        }

        Iterator& operator++() {
            node_ = node_->_next;
            return *this;
        }

        Iterator& operator--() {
            node_ = node_->_prev;
            return *this;
        }

        Iterator operator++(int) {
            Iterator copy(*this);
            ++(*this);
            return copy;
        }

        Iterator operator--(int) {
            Iterator copy(*this);
            --(*this);
            return copy;
        }

        bool operator==(const Iterator& other) {
            return node_ == other.node_;
        }

        bool operator!=(const Iterator& other) {
            return node_ != other.node_;
        }

        T& operator*() {
            return GetReference();
        }

    private:
        Node* node_;
    };
};

template <typename T, typename Allocator>
void List<T, Allocator>::BaseInit() {
    size_ = 0;
    nil_ = allocator_.allocate(1);
    allocator_.construct(nil_);
    nil_->_next = nil_;
    nil_->_prev = nil_;
}

template <typename T, typename Allocator>
List<T, Allocator>::List() {
    BaseInit();
}

template <typename T, typename Allocator>
void List<T, Allocator>::Clone(List<T, Allocator>& other) {
    for (auto i = other.Begin(); i != other.End(); i++) {
        PushBack(i.GetReference());
    }
}

template <typename T, typename Allocator>
List<T, Allocator>::List(const List& other) {
    allocator_(allocator_traits::select_on_container_copy_construction(other.allocator_));
    BaseInit();
    Clone(other);
}

template <typename T, typename Allocator>
List<T, Allocator>::List(const List& other, const Allocator& alloc) : allocator_(alloc) {
    BaseInit();
    Clone(other);
}

template <typename T, typename Allocator>
List<T, Allocator>::List(List&& other) {
    allocator_(std::move(other.allocator_));
    size_ = other.size_;
    nil_ = other.nil_;
    other.nil_ = nullptr;
    other.size_ = 0;
}

template <typename T, typename Allocator>
List<T, Allocator>::List(List&& other, const Allocator& alloc) {
    allocator_(alloc);
    size_ = other.size_;
    nil_ = other.nil_;
    other.nil_ = nullptr;
    other.size_ = 0;
}

template <typename T, typename Allocator>
List<T, Allocator>::~List() {
    if (nil_ == nullptr) {
        return;
    }

    Node* cur_node = nil_->_next;
    while (cur_node != nil_) {
        Node* to_delete = cur_node;
        cur_node = cur_node->_next;
        allocator_.destroy(to_delete);
        allocator_.deallocate(to_delete, 1);
    }
    allocator_.destroy(nil_);
    allocator_.deallocate(nil_, 1);
}

template <typename T, typename Allocator>
List<T, Allocator>& List<T, Allocator>::operator=(const List& other) {

    if (this == &other) {
        return *this;
    }

    Clear();

    if (allocator_traits::propagate_on_container_copy_assignment::value) {
        allocator_ = other.allocator_;
    }

    for (auto i = other.Begin(); i != other.End(); i++) {
        PushBack(i.GetValue());
    }

    return *this;
}

template <typename T, typename Allocator>
List<T, Allocator>& List<T, Allocator>::operator=(List&& other) noexcept {
    Clear();

    if (allocator_traits::propagate_on_container_move_assignment::value) {
        allocator_.destroy(nil_);
        allocator_.deallocate(nil_, 1);

        allocator_ = std::move(other.allocator_);
        nil_ = other.nil_;
        other.nil_ = nullptr;
    } else {
        for (auto i = other.Begin(); i != other.End(); i++) {
            PushBack(std::move(i.GetReference()));
        }
    }

    return *this;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::iterator List<T, Allocator>::Begin() noexcept {
    return this->nil_ == nullptr ? Iterator() : Iterator(nil_->_next);
}

template <typename T, typename Allocator>
typename List<T, Allocator>::const_iterator List<T, Allocator>::Begin() const noexcept {
    return this->nil_ == nullptr ? Iterator() : Iterator(nil_->_next);
}

template <typename T, typename Allocator>
typename List<T, Allocator>::iterator List<T, Allocator>::End() noexcept {
    return Iterator(nil_);
}

template <typename T, typename Allocator>
typename List<T, Allocator>::const_iterator List<T, Allocator>::End() const noexcept {
    return Iterator(nil_);
}

template <typename T, typename Allocator>
typename List<T, Allocator>::reference List<T, Allocator>::Front() {
    return nil_->_next;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::const_reference List<T, Allocator>::Front() const {
    return nil_->_next;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::reference List<T, Allocator>::Back() {
    return nil_->prev_;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::const_reference List<T, Allocator>::Back() const {
    return nil_->_prev;
}

template <typename T, typename Allocator>
bool List<T, Allocator>::Empty() const noexcept {
    return size_ == 0;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::size_type List<T, Allocator>::Size() const noexcept {
    return size_;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::size_type List<T, Allocator>::MaxSize() const noexcept {
    return allocator_.max_size;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::allocator_type List<T, Allocator>::GetAllocator() const noexcept {
    return allocator_;
}

template <typename T, typename Allocator>
void List<T, Allocator>::Clear() {
    while (!Empty()) {
        PopBack();
    }
}

template <typename T, typename Allocator>
void List<T, Allocator>::PushBack(const T& value) {
    size_ += 1;

    Node* node = static_cast<Node*>(allocator_.allocate(1));
    allocator_.construct(node, value);

    nil_->_prev->_next = node;

    node->_prev = nil_->_prev;
    node->_next = nil_;
    nil_->_prev = node;
}

template <typename T, typename Allocator>
void List<T, Allocator>::PushBack(T&& value) {
    size_ += 1;

    Node* node = static_cast<Node*>(allocator_.allocate(1));
    allocator_.construct(node, std::forward<T>(value));

    nil_->_prev->_next = node;

    node->_prev = nil_->_prev;
    node->_next = nil_;
    nil_->_prev = node;
}

template <typename T, typename Allocator>
template <typename... Args>
void List<T, Allocator>::EmplaceBack(Args&&... args) {
    size_ += 1;

    Node* node = static_cast<Node*>(allocator_.allocate(1));
    allocator_.construct(node, std::forward<Args>(args)...);

    nil_->_prev->_next = node;

    node->_prev = nil_->_prev;
    node->_next = nil_;
    nil_->_prev = node;
}

template <typename T, typename Allocator>
void List<T, Allocator>::PopBack() {
    if (Empty()) {
        return;
    }
    size_ -= 1;
    Node* to_delete = nil_->_prev;
    nil_->_prev = to_delete->_prev;
    to_delete->_prev->_next = nil_;

    allocator_.destroy(to_delete);
    allocator_.deallocate(to_delete, 1);
}

template <typename T, typename Allocator>
void List<T, Allocator>::PushFront(const T& value) {
    size_ += 1;

    Node* node = static_cast<Node*>(allocator_.allocate(1));
    allocator_.construct(node, value, nil_, nil_->_next);

    nil_->_next = node;
    node->_next->_prev = node;
}

template <typename T, typename Allocator>
void List<T, Allocator>::PushFront(T&& value) {
    size_ += 1;

    Node* node = static_cast<Node*>(allocator_.allocate(1));
    allocator_.construct(node, std::forward<T>(value), nil_, nil_->_next);

    nil_->_next = node;
    node->_next->_prev = node;
}

template <typename T, typename Allocator>
template <typename... Args>
void List<T, Allocator>::EmplaceFront(Args&&... args) {
    size_ += 1;

    Node* node = static_cast<Node*>(allocator_.allocate(1));
    allocator_.construct(node, std::forward<Args>(args)...);

    node->_prev = nil_;
    node->_next = nil_->_next;

    nil_->_next = node;
    node->_next->_prev = node;
}

template <typename T, typename Allocator>
void List<T, Allocator>::PopFront() {
    if (Empty()) {
        return;
    }
    size_ -= 1;
    Node* to_delete = nil_->_next;
    nil_->_next = to_delete->_next;
    to_delete->_next->_prev = nil_;

    allocator_.destroy(to_delete);
    allocator_.deallocate(to_delete, 1);
}

template <typename T, typename Allocator>
void List<T, Allocator>::Swap(List& other) noexcept {

    if (allocator_ != other.allocator_ && !allocator_traits::propagate_on_container_swap::value) {
        return;
    }

    if (allocator_ != other.allocator_) {
        std::swap(allocator_, other.allocator_);
    }
    std::swap(nil_, other.nil_);
    std::swap(size_, other.size_);
}

template <typename T, typename Allocator>
void List<T, Allocator>::Resize(List::size_type count) {
    if (count == 0) {
        Clear();
    } else if (count < size_) {
        while (count < size_) {
            PopBack();
        }
    } else {
        for (size_t i = size_; i < count; i++) {
            PushBack({});
        }
    }

    size_ = count;
}

template <typename T, typename Allocator>
void List<T, Allocator>::Remove(const T& value) {
    Node* cur_node = nil_->_next;

    while (cur_node != nil_) {
        if (cur_node->_value == value) {
            cur_node = RemoveNode(cur_node);
        } else {
            cur_node = cur_node->_next;
        }
    }
}

template <typename T, typename Allocator>
void List<T, Allocator>::Unique() {
    Node* cur_node = nil_->_next;
    Node* next_node = cur_node->_next;

    while (cur_node != nil_->_prev && next_node != nil_) {
        if (cur_node->_value == next_node->_value) {
            RemoveNode(next_node);
            next_node = cur_node->_next;
        } else {
            cur_node = cur_node->_next;
            next_node = next_node->_next;
        }
    }
}

template <typename T, typename Allocator>
void List<T, Allocator>::Sort() {
    QuickSort(nil_->_next, nil_->_prev);
}

template <typename T, typename Allocator>
typename List<T, Allocator>::Node* List<T, Allocator>::RemoveNode(List::Node* cur_node) {
    Node* to_remove = cur_node;
    cur_node = cur_node->_next;
    Node* prev_node = to_remove->_prev;

    prev_node->_next = cur_node;
    cur_node->_prev = prev_node;

    size_ -= 1;
    allocator_.destroy(to_remove);
    allocator_.deallocate(to_remove, 1);

    return cur_node;
}

template <typename T, typename Allocator>
void List<T, Allocator>::QuickSort(List::Node* start, List::Node* end) {
    if (start == end || end == nil_ || start == end->_next) {
        return;
    }

    Node* pivot = Partition(start, end);

    QuickSort(start, pivot->_prev);
    QuickSort(pivot->_next, end);
}

template <typename T, typename Allocator>
typename List<T, Allocator>::Node* List<T, Allocator>::Partition(List::Node* start_node,
                                                                 List::Node* end_node) {
    auto pivot{end_node->_value};

    Node* last_smaller = start_node;

    Node* cur_node = start_node;

    while (cur_node != end_node) {
        if (cur_node->_value <= pivot) {
            Swap(last_smaller, cur_node);
            last_smaller = last_smaller->_next;
        }
        cur_node = cur_node->_next;
    }
    Swap(last_smaller, end_node);

    return last_smaller;
}

template <typename T, typename Allocator>
void List<T, Allocator>::Swap(List::Node* first, List::Node* second) {
    auto tmp_val{first->_value};
    first->_value = second->_value;
    second->_value = tmp_val;
}

}  // namespace task
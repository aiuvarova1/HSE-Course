#include "list.h"

task::list::list(size_t count, const int& value) {
    _size = count;
    _head = new Node(value);

    Node* cur_node;
    Node* prev_node = _head;

    for (size_t i = 0; i < count; i++) {
        cur_node = new Node(value);
        prev_node->_next = cur_node;
        cur_node->_prev = prev_node;
        prev_node = cur_node;
    }

    _tail = cur_node;
}

task::list::list(const task::list& other) {
    _size = other._size;

    Node* cur_node = new Node(other._head->_value);
    Node* cur_other_node = other._head->_next;

    _head = cur_node;

    while (cur_other_node != nullptr) {
        Node* new_node = new Node(cur_other_node->_value);
        cur_node->_next = new_node;
        new_node->_prev = cur_node;

        cur_node = cur_node->_next;
        cur_other_node = cur_other_node->_next;
    }

    _tail = cur_node;
}

task::list::~list() {
    clear();
}

task::list& task::list::operator=(const task::list& other) {
    if (this == &other)
        return *this;

    clear();
    _size = other._size;

    Node* cur_node = new Node(other._head->_value);
    Node* cur_other_node = other._head->_next;

    _head = cur_node;

    while (cur_other_node != nullptr) {
        Node* new_node = new Node(cur_other_node->_value);
        cur_node->_next = new_node;
        new_node->_prev = cur_node;

        cur_node = cur_node->_next;
        cur_other_node = cur_other_node->_next;
    }

    _tail = cur_node;
    return *this;
}

int& task::list::front() {
    return _head->_value;
}

const int& task::list::front() const {
    return _head->_value;
}

int& task::list::back() {
    return _tail->_value;
}

const int& task::list::back() const {
    return _tail->_value;
}

bool task::list::empty() const {
    return _size == 0;
}

size_t task::list::size() const {
    return _size;
}

void task::list::clear() {
    Node* cur_node = _head;
    while (cur_node != nullptr) {
        Node* to_delete = cur_node;
        cur_node = cur_node->_next;
        delete to_delete;
    }
    _head = nullptr;
    _tail = nullptr;
    _size = 0;
}

void task::list::push_back(const int& value) {
    _size += 1;
    Node* node = new Node(value);

    node->_prev = _tail;

    if (_tail != nullptr)
        _tail->_next = node;
    if (_head == nullptr)
        _head = node;

    _tail = node;
}

void task::list::pop_back() {
    if (_size == 0) {
        return;
    }
    _size -= 1;
    Node* to_delete = _tail;
    _tail = to_delete->_prev;

    if (_tail != nullptr)
        _tail->_next = nullptr;
    if (_size == 0)
        _head = nullptr;

    delete to_delete;
}

void task::list::push_front(const int& value) {
    _size += 1;
    Node* node = new Node(value, nullptr, _head);

    if (_head != nullptr)
        _head->_prev = node;
    if (_tail == nullptr)
        _tail = node;

    _head = node;
}

void task::list::pop_front() {
    if (_size == 0)
        return;

    _size -= 1;
    Node* to_delete = _head;
    _head = to_delete->_next;

    if (_head != nullptr)
        _head->_prev = nullptr;
    if (_size == 0)
        _tail = nullptr;

    delete to_delete;
}

void task::list::resize(size_t count) {
    if (count == 0) {
        clear();
    } else if (count < _size) {
        Node* cur_node = _head;
        while (cur_node != _tail) {
            Node* to_delete = cur_node;
            cur_node = cur_node->_prev;
            delete to_delete;
        }
        _tail = cur_node;
        _tail->_next = nullptr;
    } else {
        for (size_t i = _size; i < count; i++) {
            push_back(0);
        }
    }

    _size = count;
}

void task::list::swap(task::list& other) {
    Node* temp = _head;
    _head = other._head;
    other._head = temp;

    temp = _tail;
    _tail = other._tail;
    other._tail = temp;

    size_t size = _size;
    _size = other._size;
    other._size = size;
}

void task::list::remove(const int value) {
    Node* cur_node = _head;

    while (cur_node != nullptr) {
        if (cur_node->_value == value) {
            cur_node = remove_node(cur_node);
        } else {
            cur_node = cur_node->_next;
        }
    }
}

void task::list::unique() {
    Node* cur_node = _head;
    Node* next_node = cur_node->_next;

    while (cur_node != _tail && next_node != nullptr) {
        if (cur_node->_value == next_node->_value) {
            remove_node(next_node);
            next_node = cur_node->_next;
        } else {
            cur_node = cur_node->_next;
            next_node = next_node->_next;
        }
    }
}

void task::list::sort() {
    quick_sort(_head, _tail);
}

task::list::Node* task::list::remove_node(task::list::Node* cur_node) {
    Node* to_remove = cur_node;
    cur_node = cur_node->_next;
    Node* prev_node = to_remove->_prev;

    if (prev_node != nullptr) {
        prev_node->_next = cur_node;
        if (prev_node->_next == nullptr)
            _tail = prev_node;
    }

    if (cur_node != nullptr) {
        cur_node->_prev = prev_node;

        if (cur_node->_prev == nullptr)
            _head = cur_node;
    }

    if (to_remove == _head)
        _head = nullptr;
    if (to_remove == _tail)
        _tail = nullptr;

    _size -= 1;
    delete to_remove;

    return cur_node;
}

task::list::Node* task::list::partition(task::list::Node* start_node, task::list::Node* end_node) {
    int pivot = end_node->_value;

    Node* last_smaller = start_node;

    Node* cur_node = start_node;

    while (cur_node != end_node) {
        if (cur_node->_value <= pivot) {
            swap(last_smaller, cur_node);
            last_smaller = last_smaller->_next;
        }
        cur_node = cur_node->_next;
    }
    swap(last_smaller, end_node);

    return last_smaller;
}

void task::list::swap(task::list::Node* first, task::list::Node* second) {
    int tmp_val = first->_value;
    first->_value = second->_value;
    second->_value = tmp_val;
}

void task::list::quick_sort(task::list::Node* start, task::list::Node* end) {
    if (start == end || end == nullptr || start == end->_next)
        return;

    Node* pivot = partition(start, end);

    quick_sort(start, pivot->_prev);
    quick_sort(pivot->_next, end);
}

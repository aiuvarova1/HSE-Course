#pragma once
#include <cstddef>


namespace task {

class list {
public:

    list() : _size(0), _head(nullptr), _tail(nullptr) {};
    list(size_t count, const int& value = int());

    ~list();
    list& operator=(const list& other);

    int& front();
    const int& front() const;

    int& back();
    const int& back() const;


    bool empty() const;
    size_t size() const;
    void clear();


    void push_back(const int& value);
    void pop_back();
    void push_front(const int& value);
    void pop_front();
    void resize(size_t count);
    void swap(list& other);


    void remove(const int& value);
    void unique();
    void sort();

private:
    struct Node {
        int _value;
        Node* _next;
        Node* _prev;

        Node(int value) : _value(value), _next(nullptr), _prev(nullptr) {};
        Node(int value, Node* prev, Node* next) : _value(value), _prev(prev), _next(next) {};

        ~Node() = default;
    };

    Node* _head;
    Node* _tail;

    size_t _size;

    //returns prev node
    Node* remove_node(Node* to_remove);
    void quick_sort(Node* start, Node* end);
    Node* partition(Node* start_node, Node* end_node);
    void swap(Node* first, Node* second);


};

}  // namespace task

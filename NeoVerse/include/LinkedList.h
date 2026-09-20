#pragma once
#include <functional>
#include <cstddef>

// A minimal hand-rolled singly linked list with a tail pointer.
// Used for the historical city log, which the brief says must support
// "unlimited growth". Written by hand (rather than using std::list)
// to show the underlying node/pointer mechanics explicitly.
template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        explicit Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    std::size_t count;

public:
    LinkedList() : head(nullptr), tail(nullptr), count(0) {}

    // Destructor walks the list and frees every node - prevents memory
    // leaks since each Node was heap-allocated with `new`.
    ~LinkedList() {
        Node* cur = head;
        while (cur) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }

    // O(1) append: because we keep a tail pointer, adding a new log entry
    // never requires walking or shifting existing nodes, unlike a vector,
    // which occasionally has to reallocate and copy everything it holds.
    void append(const T& value) {
        Node* n = new Node(value);
        if (!tail) {
            head = tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
        ++count;
    }

    // O(n) removal by predicate - the list has no random access, so we
    // must walk node by node until we find a match.
    bool removeIf(const std::function<bool(const T&)>& pred) {
        Node* prev = nullptr;
        Node* cur = head;
        while (cur) {
            if (pred(cur->data)) {
                if (prev) prev->next = cur->next; else head = cur->next;
                if (cur == tail) tail = prev;
                delete cur;
                --count;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }

    // O(n) traversal, visiting every node via its `next` pointer.
    void forEach(const std::function<void(const T&)>& fn) const {
        Node* cur = head;
        while (cur) {
            fn(cur->data);
            cur = cur->next;
        }
    }

    std::size_t size() const { return count; }
    bool empty() const { return count == 0; }
};

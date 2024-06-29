#include "tortoise_hare.h"
#include <iostream>
#include <memory>
#include <cassert>

using namespace std;

template<typename T>
struct Node {
    T data;
    shared_ptr<Node<T>> next;
    Node(T data, shared_ptr<Node<T>> next) : data(data), next(next) {}
};

template<typename T>
class List {
    using NodePtr = shared_ptr<Node<T>>;
    NodePtr head{};
    int count = 0;

public:
    NodePtr get_head() {
        return head;
    }

    void add(const T& data) {
        head = make_shared<Node<T>>(data, head);
        ++count;
    }

    bool make_cycle(int n) {
        if (n > count) return false;
        NodePtr nth_node = head;
        while (n--)
            nth_node = nth_node->next;

        NodePtr tail = nth_node;
        while (tail->next != nullptr)
            tail = tail->next;

        // now make a cycle, let the evil begin
        tail->next = nth_node;

        return true;
    }
};

void test1()
{
    List<int> list;
    for (int i = 0; i < 6; ++i)
        list.add(i);

    assert(list.make_cycle(2));

    int lambda = 0;
    int mu = 0;

    auto p = tortoise_hare(list.get_head(), lambda, mu);
    if (p == nullptr) {
        cout << "No loop detected\n";
    }
    else {
        cout << "Detected a loop with length = " << lambda
             << "\nLoop started with a distance of " << mu
             << " from the head node, its value is " << p->data
             << '\n';
        // break the cycle, so resources can be cleaned up
        // see also https://en.cppreference.com/w/cpp/memory/weak_ptr/~weak_ptr#Example
        p->next = nullptr;
    }
}

void test2()
{
    List<char> list;
    for (char c = 'A'; c < 'a'; ++c)
        list.add(c);
    int lambda = 0, mu = 0;
    auto p = tortoise_hare(list.get_head(), lambda, mu);
    assert(p == nullptr);
}

int main()
{
    test1();
    test2();
    return 0;
}

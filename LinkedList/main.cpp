#include <iostream>
#include <memory>
#include <vector>

struct Node {
    using NodeUPtr = std::unique_ptr<Node>;
    using NodePtr = Node*;

    int val = 0;
    NodeUPtr next = nullptr;
    NodePtr prev = nullptr;

    Node() = default;
    Node(int val): val(val) {}
    Node(int val, NodePtr prev, NodeUPtr next): val(val), prev(prev), next(std::move(next)) {}

    void insertAsPrev(int val)
    {
        NodeUPtr node = std::make_unique<Node>(val, prev, std::move(prev->next));
        prev->next = std::move(node);
        prev = prev->next.get();
    }

    void insertAsNext(int val)
    {
        NodeUPtr node = std::make_unique<Node>(val, this, std::move(next));
        next = std::move(node);
        next->next->prev = next.get();
    }

    void erase()
    {
        next->prev = prev;
        prev->next = std::move(next);
    }
};

struct LinkedList {
    using NodeUPtr = std::unique_ptr<Node>;
    using NodePtr = Node*;

    size_t _size = 0;
    NodeUPtr dummyHead = nullptr;
    NodePtr dummyTail = nullptr;

    LinkedList()
    {
        dummyHead = std::make_unique<Node>();
        dummyHead->next = std::make_unique<Node>();
        dummyTail = dummyHead->next.get();
        dummyTail->prev = dummyHead.get();
    }

    bool empty()
    {
        return dummyHead->next.get() == dummyTail;
    }

    size_t size()
    {
        return _size;
    }

    NodePtr front()
    {
        return dummyHead->next.get();
    }    

    NodePtr back()
    {
        return dummyTail->prev;
    }

    void push_front(int val)
    {
        dummyHead->insertAsNext(val);
        ++_size;
    }

    void push_back(int val)
    {
        dummyTail->insertAsPrev(val);
        ++_size;
    }

    int pop_front()
    {
        int ret = front()->val;
        front()->erase();
        --_size;
        return ret;
    }

    int pop_back()
    {
        int ret = back()->val;
        back()->erase();
        --_size;
        return ret;
    }

    NodePtr at(size_t index)
    {
        NodePtr cur = front();
        for (size_t i = 0; i < index && cur; ++i) {
            cur = cur->next.get();
        }
        return cur;
    }

    void clear()
    {
        dummyHead->next = std::move(dummyTail->prev->next);
        dummyTail->prev = dummyHead.get();
        _size = 0;
    }
};

int main()
{
    LinkedList list;
    std::vector<int> vec = {1, 2, 3, 4, 5};

    for (auto const &i: vec) {
        list.push_back(i);
    }

    for (int i = 0; i < list.size(); ++i) {
        std::cout << list.at(i)->val << ' ';
    }

    std::cout << "First element is " << list.pop_front() << "\n";
    std::cout << "Last element is " << list.pop_back() << "\n";

    for (int i = 0; i < list.size(); ++i) {
        std::cout << list.at(i)->val << ' ';
    }

    list.clear();
    if (list.empty())
        std::cout << "List is cleared!" << std::endl;

    return 0;
}
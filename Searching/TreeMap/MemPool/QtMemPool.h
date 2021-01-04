/*
 * A single-threaded pool allocator for qt from blog:
 * https://www.qt.io/blog/a-fast-and-thread-safe-pool-allocator-for-qt-part-1
 * 
 */
#ifndef QTMEMPOOL_H
#define QTMEMPOOL_H

#include <vector>
#include <cstdlib>

template<typename T>
class PoolAllocator
{
    static constexpr size_t ChunkSize = sizeof(T);

    struct Page
    {
        char memory[4096];
    };

    union Node
    {
        Node* previous;
        char  chunk[ChunkSize];
    };
public:
    PoolAllocator()
    {
    }
    ~PoolAllocator()
    {
        clear();
    }
    void* allocate(/*[[maybe_unused]] size_t count = 1*/)
    {        
        if (!stack && !grow())
            return nullptr;
        Node* top = stack;
        stack = stack->previous;
        return top->chunk;
    }
    void recycle(void* ptr)
    {
        if (ptr == nullptr) return;
        Node* top = static_cast<Node*>(ptr);
        top->previous = stack;
        stack = top;
    }
    void clear()
    {
        for (auto page : pages)
            ::free(page);
        pages.clear();
    }

private:
    bool grow()
    {
        Page* newPage = static_cast<Page*>(::malloc(sizeof(Page)));
        if (!newPage)
            return false;
        initPage(newPage);
        pages.push_back(newPage);
        return true;
    }
    void initPage(Page* page)
    {
        Node* newStack = reinterpret_cast<Node*>(page->memory);
        newStack->previous = stack;

        const size_t numChunks = sizeof(Page) / sizeof(Node);
        for (size_t i = 0; i < numChunks - 1; ++i, ++newStack)
            newStack[1].previous = newStack;
        stack = newStack;
    }

    Node* stack = nullptr;
    std::vector<Page*> pages;
};

#endif // !QTMEMPOOL_H

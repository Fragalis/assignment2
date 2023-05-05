#include "main.h"

struct order {
    int id;
    int count;
    void reset() {
        id = 0;
        count = 0;
    }
};

class MinHeap {
private:
    int capacity = MAXSIZE;
    order orders[MAXSIZE];
    int size;
    vector<int> time;
    // Always call when a new customer is assigned
    void _push(int id) {
        // Heap is full
        if(IsFull()) return;
        
        // Else
        orders[size].id = id;
        orders[size].count = 1;
        _reheapUp(size);
        ++size;
        time.push_back(id);
    }

    void _pop(int id) {
        // Heap is empty
        if(size <= 0) return;
    
        // Else
        int idx = 0;
        while(orders[idx].id != id) ++idx;
        time.erase(find(time.begin(), time.end(), id));
        orders[idx] = orders[size - 1];
        orders[size - 1].reset();
        --size;
        _reheapDown(idx);
    }

    void _reheapUp(int position) {
        if(position > 0) {
            int parent = (position - 1) / 2;
            if(orders[position].count < orders[parent].count) {
                swap(orders[position], orders[parent]);
                _reheapUp(parent);
            }
            else if(orders[position].count == orders[parent].count) {
                int step = find(time.begin(), time.end(), orders[position].id) - 
                            find(time.begin(), time.end(), orders[parent].id);
                if(step < 0) {
                    swap(orders[position], orders[parent]);
                    _reheapUp(parent);
                }
            }
        }
    }

    void _reheapDown(int position) {
        int left = 2 * position + 1;
        int right = 2 * position + 2;
        int smaller = -1;
        if(left < size) {
            if(right < size) {
                // IF right.count < left.count
                if(orders[right].count < orders[left].count) smaller = right;
                // IF right.count == left.count
                else if(orders[right].count == orders[left].count) {
                    int step = find(time.begin(), time.end(), orders[right].id) - 
                               find(time.begin(), time.end(), orders[left].id);
                    if(step < 0) smaller = right;
                    else smaller = left;
                }
                else smaller = left;
            }
            else smaller = left;
            if(orders[position].count > orders[smaller].count) {
                swap(orders[position], orders[smaller]);
                _reheapDown(smaller);
            }
            else if(orders[position].count == orders[smaller].count) {
                int step = find(time.begin(), time.end(), orders[position].id) - 
                            find(time.begin(), time.end(), orders[smaller].id);
                if(step > 0) {
                    swap(orders[position], orders[smaller]);
                    _reheapDown(smaller);
                }
            }
        }
    }

    int _getOrderCount(int id) {
        for(int i = 0; i < size; ++i) {
            if(orders[i].id == id) return orders[i].count;
        }
        return 0;
    }

    void _print(int position) {
        if(position >= size) return;
        // Print data at position
        cout << orders[position].id << "-" << orders[position].count << "\n";
        // Traverse to left
        _print(2 * position + 1);
        // Traverse to right
        _print(2 * position + 2);
    }
public:
    MinHeap() {
        size = 0;
        for(int i = 0; i < MAXSIZE; ++i) {
            orders[i].count = 0;
            orders[i].id = 0;
        }
    }

    ~MinHeap() {}

    bool IsFull() {
        return (size >= capacity);
    }

    // If the id is new, we add order{id, 1} to Heap
    // Else, we increment the order count by 1
    void Push(int id) {
        // We traverse the heap for id
        for(int i = 0; i < size; ++i) {
            // If we found the id
            if(orders[i].id == id) {
                // Increment the count, then reheapDown at this position and return
                orders[i].count++;
                _reheapDown(i);
                return;
            }
        }
        // Here if the id is new <- we can't find it in that loop before
        _push(id);
    }

    // This id will always be available due to this being called when removal is needed
    void Pop(int id) {
        _pop(id);
    }

    // Get the id of customer who orders the least (always be orders[0])
    int Front() {
        if(size <= 0) return -1;
        return orders[0].id;
    }

    int GetOrder(int id) {
        return _getOrderCount(id);
    }

    void PrintHeap() {
        cout << "MIN HEAP: [\n";
        for(int i = 0; i < size; ++i) {
            cout << "ID: " << orders[i].id << " Count: " << orders[i].count << "\n";
        }
        cout << "]\n";
    }

    void Print_Command() {
        _print(0);
    }
};
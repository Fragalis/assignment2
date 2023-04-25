#include "minheap.cpp"

int main() {
    MinHeap minheap_sample;
    const int sample = 32;
    // int id[3]; for(int i = 0; i < 3; ++i) id[i] = i + 1;
    int order[7] = {1, 2, 3, 2, 1, 3, 1};
    for(int i = 0; i < 7; ++i) { 
        int ida = order[i];
        cout << "Add/Increment: " << ida << endl;
        minheap_sample.Push(ida);
        minheap_sample.PrintHeap();
    }
    minheap_sample.PrintHeap();
}
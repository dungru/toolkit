#include <iostream>
#include <vector>
#include <stdio.h>
using namespace std;
class PQueue {
    private:
        std::vector<int> min_heap;
        int heap_size;

    public:
        PQueue () : heap_size(0) {}
        void sink(int idx) {
            int left, right,smallest;
            while (1) {
                left = 2 * idx + 1;
                right = 2 * idx + 2;
                smallest = left;
                if( right < heap_size && min_heap.at(right) < min_heap.at(left))
                    smallest = right;
                if (left >= heap_size || min_heap.at(idx) < min_heap.at(smallest))
                    break;
                swap(min_heap.at(idx), min_heap.at(smallest));
                idx = smallest;
            }
        }
        void swim(int idx) {
            int parent;
            parent = (idx - 1) / 2;
            while (idx > 0 && min_heap.at(parent) > min_heap.at(idx)) {
                swap(min_heap.at(parent), min_heap.at(idx));/* code */
                idx = parent;
                parent = (idx - 1) / 2;
            }
            
        }
        void insert(int val) {
            min_heap.push_back(val);
            heap_size = min_heap.size();
            swim(heap_size - 1);
        }
        void remove(int val) {
            int key = 0;
            int parent;
            std::vector<int>::iterator iter;
            for (iter = min_heap.begin(); iter != min_heap.end(); ++iter) {
                if (*iter == val) {
                    key = iter - min_heap.begin();
                    break;
                }
            }
            if (key >= heap_size) {
                cout << val <<" not found." << endl;
                return;
            }
            swap(min_heap.back(), min_heap.at(key));
            min_heap.pop_back();
            heap_size = min_heap.size();
            if (heap_size == 0) {
                return;
            }
            parent = (key - 1) / 2;
            if (parent >= 0 && min_heap.at(key) < min_heap.at(parent))
            {
                swim(key);
            } else {
                sink(key);
            } 
        }

        void poll(int *val) {
            if (min_heap.empty())
                return;
            *val = min_heap.front();
            swap(min_heap.back(), min_heap.front());
            min_heap.pop_back();
            heap_size = min_heap.size();
            if (min_heap.empty())
                return;
            sink(0);
        }

        void heap_dump () {
            for (std::vector<int>::iterator it = min_heap.begin(); it != min_heap.end(); ++it)
            {
                cout << *it << " ";
            }
            cout << endl;
        }
};


int main(int argc, char const *argv[])
{
    PQueue pq_obj;
    int val;
    pq_obj.insert(1);
    pq_obj.insert(5);
    pq_obj.insert(1);
    pq_obj.insert(8);
    pq_obj.insert(6);
    pq_obj.insert(2);
    pq_obj.insert(2);
    pq_obj.insert(13);
    pq_obj.insert(12);
    pq_obj.insert(11);
    pq_obj.insert(7);
    pq_obj.insert(2);
    pq_obj.insert(15);
    pq_obj.insert(3);
    pq_obj.insert(10);
    pq_obj.heap_dump();
    pq_obj.poll(&val);
    pq_obj.heap_dump();
    pq_obj.remove(12);
    pq_obj.heap_dump();
    pq_obj.remove(3);
    pq_obj.heap_dump();
    pq_obj.poll(&val);
    pq_obj.heap_dump();
    return 0;
}
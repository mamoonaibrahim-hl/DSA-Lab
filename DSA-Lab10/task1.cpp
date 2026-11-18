//Task 1: Max Heap Insertion and Heap Sort (with user input)
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

// Reads one whole number from the user and keeps asking until it is valid.
// Checks: not empty, only digits (and one minus sign at the start if allowed),
// not too long, and inside the range low..high. Time: O(length of input)
int readInt(string message, int low, int high) {
    while (true) {
        cout << message;
        string line;
        if (!getline(cin, line)) {          // input ended (Ctrl+Z / Ctrl+D)
            cout << "\nInput ended. Program stopped." << endl;
            exit(1);
        }

        // remove spaces from the start and end
        while (!line.empty() && line[0] == ' ')
            line.erase(0, 1);
        while (!line.empty() && line[line.size() - 1] == ' ')
            line.erase(line.size() - 1, 1);

        if (line.empty()) {
            cout << "Error: nothing was entered. Try again." << endl;
            continue;
        }

        int start = 0;
        bool negative = false;
        if (line[0] == '-') {
            negative = true;
            start = 1;
        }
        if (start == (int)line.size()) {
            cout << "Error: '-' alone is not a number. Try again." << endl;
            continue;
        }
        if (line.size() - start > 9) {
            cout << "Error: number is too long. Try again." << endl;
            continue;
        }

        bool ok = true;
        int number = 0;
        for (int i = start; i < (int)line.size(); i++) {
            if (line[i] < '0' || line[i] > '9') {
                ok = false;
                break;
            }
            number = number * 10 + (line[i] - '0');
        }
        if (!ok) {
            cout << "Error: please enter a whole number only (no letters or symbols). Try again." << endl;
            continue;
        }
        if (negative)
            number = -number;

        if (number < low || number > high) {
            cout << "Error: number must be between " << low << " and " << high << ". Try again." << endl;
            continue;
        }
        return number;
    }
}

class MaxHeap {
private:
    int* arr;       // dynamic array that stores the heap
    int size;       // how many elements are in the heap now
    int capacity;   // how many elements the array can hold

    // Swap two numbers. Time: O(1)
    void swapValues(int& a, int& b) {
        int temp = a;
        a = b;
        b = temp;
    }

public:
    // Constructor: makes an empty heap. Time: O(1)
    MaxHeap(int cap) {
        capacity = cap;
        size = 0;
        arr = new int[capacity];
    }

    // Destructor: frees the array memory. Time: O(1)
    ~MaxHeap() {
        delete[] arr;
    }

    // Insert a value, then sift it up. Returns the number of swaps.
    // Time: O(log n) because the value can climb at most the height of the tree.
    int insert(int value) {
        if (size == capacity) {
            cout << "Heap is full!" << endl;
            return 0;
        }
        arr[size] = value;      // step 1: put value at the end
        int i = size;
        size++;

        int swaps = 0;
        // step 2: sift up while value is bigger than its parent
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (arr[i] > arr[parent]) {
                swapValues(arr[i], arr[parent]);
                swaps++;
                i = parent;     // move up one level
            } else {
                break;          // parent is bigger, we are done
            }
        }
        return swaps;
    }

    // Print the heap array. Time: O(n)
    void printHeap() {
        for (int i = 0; i < size; i++) {
            cout << arr[i];
            if (i < size - 1)
                cout << " ";
        }
        cout << endl;
    }

    // Sift down inside array a, looking only at the first n elements.
    // Time: O(log n)
    void siftDown(int* a, int n, int i) {
        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int largest = i;

            if (left < n && a[left] > a[largest])
                largest = left;
            if (right < n && a[right] > a[largest])
                largest = right;

            if (largest == i)
                break;          // node is already bigger than its children

            swapValues(a[i], a[largest]);
            i = largest;        // continue from the child's position
        }
    }

    // Heap sort: puts the values in ascending order in a new array.
    // A copy is used so the original max heap stays unchanged and can be printed.
    // Time: O(n log n)
    int* heapSort() {
        int* sorted = new int[size];
        for (int i = 0; i < size; i++)      // copy the heap
            sorted[i] = arr[i];

        // Repeat: move the biggest (root) to the end, shrink heap, fix root.
        for (int end = size - 1; end > 0; end--) {
            swapValues(sorted[0], sorted[end]);
            siftDown(sorted, end, 0);       // heap now has "end" elements
        }
        return sorted;
    }

    int getSize() {
        return size;
    }
};

int main() {
    cout << "Max Heap Insertion and Heap Sort" << endl;
    int n = readInt("How many values do you want to insert (1 to 50)? ", 1, 50);

    MaxHeap heap(n);

    cout << endl;
    for (int i = 1; i <= n; i++) {
        string message = "Enter value " + to_string(i) + " of " + to_string(n) + ": ";
        int value = readInt(message, -100000000, 100000000);
        int swaps = heap.insert(value);
        cout << "Inserted " << value << ", swaps = " << swaps << ", heap: ";
        heap.printHeap();
    }

    cout << endl << "Final max heap: ";
    heap.printHeap();

    int* sorted = heap.heapSort();
    cout << "Sorted output: ";
    for (int i = 0; i < heap.getSize(); i++) {
        cout << sorted[i];
        if (i < heap.getSize() - 1)
            cout << " ";
    }
    cout << endl;
    delete[] sorted;

    /*
    QUESTIONS

    1. Worst-case swaps for one insertion into a heap of n elements?
       The new value starts at the bottom and can move up to the root.
       The height of a heap is about log2(n), so the worst case is
       about log2(n) swaps, which is O(log n).

    2. Time complexity?
       - Building the heap by n insertions: each insertion is O(log n),
         so n insertions give O(n log n).
       - Heap sort: n times we move the root to the end and sift down
         (O(log n) each), so O(n log n).
    */
    return 0;
}

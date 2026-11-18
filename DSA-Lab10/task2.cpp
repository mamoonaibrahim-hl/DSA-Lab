//Task 2: Hospital ER Wait-Time Monitor (median with two heaps, user input)
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

// Reads one whole number from the user and keeps asking until it is valid.
// Checks: not empty, only digits, not too long, and inside the range low..high.
// Time: O(length of input)
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
        if (line.size() > 9) {
            cout << "Error: number is too long. Try again." << endl;
            continue;
        }

        bool ok = true;
        int number = 0;
        for (int i = 0; i < (int)line.size(); i++) {
            if (line[i] < '0' || line[i] > '9') {
                ok = false;
                break;
            }
            number = number * 10 + (line[i] - '0');
        }
        if (!ok) {
            cout << "Error: please enter a whole number only (no letters, decimals or minus sign). Try again." << endl;
            continue;
        }
        if (number < low || number > high) {
            cout << "Error: number must be between " << low << " and " << high << ". Try again." << endl;
            continue;
        }
        return number;
    }
}

// One heap class that works as a max heap or a min heap.
// isMax = true  -> max heap (biggest value at the root)
// isMax = false -> min heap (smallest value at the root)
class Heap {
private:
    int* arr;
    int size;
    int capacity;
    bool isMax;

    // Returns true if value a should be above value b in this heap. Time: O(1)
    bool higherPriority(int a, int b) {
        if (isMax)
            return a > b;
        else
            return a < b;
    }

    void swapValues(int& a, int& b) {
        int temp = a;
        a = b;
        b = temp;
    }

    // Move the element at index i up until the parent is better. Time: O(log n)
    void siftUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (higherPriority(arr[i], arr[parent])) {
                swapValues(arr[i], arr[parent]);
                i = parent;
            } else {
                break;
            }
        }
    }

    // Move the element at index i down until both children are worse. Time: O(log n)
    void siftDown(int i) {
        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int best = i;

            if (left < size && higherPriority(arr[left], arr[best]))
                best = left;
            if (right < size && higherPriority(arr[right], arr[best]))
                best = right;

            if (best == i)
                break;

            swapValues(arr[i], arr[best]);
            i = best;
        }
    }

public:
    Heap(int cap, bool maxHeap) {
        capacity = cap;
        size = 0;
        isMax = maxHeap;
        arr = new int[capacity];
    }

    ~Heap() {
        delete[] arr;
    }

    // Add a value at the end and sift it up. Time: O(log n)
    void push(int value) {
        if (size == capacity) {
            cout << "Heap is full!" << endl;
            return;
        }
        arr[size] = value;
        size++;
        siftUp(size - 1);
    }

    // Remove and return the root. Time: O(log n)
    int pop() {
        if (size == 0) {
            cout << "Heap is empty!" << endl;
            return -1;
        }
        int root = arr[0];
        arr[0] = arr[size - 1];     // last element goes to the root
        size--;
        if (size > 0)
            siftDown(0);
        return root;
    }

    // Look at the root without removing it. Time: O(1)
    int top() {
        return arr[0];
    }

    int getSize() {
        return size;
    }

    bool isEmpty() {
        return size == 0;
    }
};

class WaitMonitor {
private:
    Heap lower;     // max heap: the smaller half of the waits
    Heap upper;     // min heap: the larger half of the waits

public:
    // Each heap can get at most n values (n = number of patients)
    WaitMonitor(int cap) : lower(cap, true), upper(cap, false) {
    }

    // Insert a new wait time and rebalance the heaps. Time: O(log n)
    void addWait(int x) {
        // Step 1: put x in the correct heap
        if (lower.isEmpty() || x <= lower.top())
            lower.push(x);
        else
            upper.push(x);

        // Step 2: rebalance so that
        //   lower size == upper size   OR   lower size == upper size + 1
        if (lower.getSize() > upper.getSize() + 1) {
            upper.push(lower.pop());    // lower has too many
        } else if (upper.getSize() > lower.getSize()) {
            lower.push(upper.pop());    // upper has too many
        }
    }

    // Median from the two roots. Time: O(1)
    double getMedian() {
        if (lower.getSize() > upper.getSize())
            return lower.top();
        return (lower.top() + upper.top()) / 2.0;
    }

    // Print the result line for one arrival
    void printStatus(int arrival, int wait, int threshold) {
        cout << "Arrival " << arrival << ": Wait = " << wait;
        cout << ", Max heap root = " << lower.top();
        cout << ", Min heap root = ";
        if (upper.isEmpty())
            cout << "none";
        else
            cout << upper.top();

        double median = getMedian();
        cout << ", Median = " << median << ", ";

        if (median > threshold)
            cout << "ALERT: call extra staff" << endl;
        else
            cout << "Normal" << endl;
    }
};

int main() {
    cout << "Hospital ER Wait-Time Monitor" << endl;
    int n = readInt("How many patients (1 to 100)? ", 1, 100);
    int threshold = readInt("Alert threshold in minutes (0 to 1440)? ", 0, 1440);

    WaitMonitor monitor(n);

    cout << endl;
    for (int i = 1; i <= n; i++) {
        string message = "Enter wait time (minutes) of patient " + to_string(i) + ": ";
        int wait = readInt(message, 0, 1440);
        monitor.addWait(wait);
        monitor.printStatus(i, wait, threshold);
    }

    /*
    QUESTIONS

    7. Why is the median better than the average? (arrival 6)
       After arrival 6 the waits are 15 20 30 45 60 90. The median is 37.5.
       The average is (15+20+30+45+60+90)/6 = 43.3. The single long wait
       of 90 minutes pulls the average up, so it is distorted by one extreme
       case. The median only looks at the middle values, so one extreme
       patient cannot change it much.

    8. What goes wrong if the heap sizes differ by two or more?
       The two roots are no longer the middle of the sorted data. For
       example, if the max heap had 5 values and the min heap 2, the root of
       the max heap would be the 5th smallest, not the middle value. The
       median we print would be wrong.

    9. Total operations for n patients:
       - Repeated sorting: sorting n values costs O(n log n), and we do it
         after every arrival, so the total is about O(n^2 log n).
       - Heap method: each arrival costs O(log n) and the median costs O(1),
         so the total is O(n log n).
    */
    return 0;
}

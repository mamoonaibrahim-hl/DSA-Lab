#include<iostream>
using namespace std;

class StringPool {
    string* stringPool; // Dynamic array of strings.
    int currentSize; // Current number of strings in the pool.
    int maxSize; // Maximum size of the pool.
public:
    // Constructor to initialize the string pool with size 5.
    StringPool(int size=5) {
        maxSize = size;
        currentSize = 0;
        stringPool = new string[maxSize];
    }
    //destructor to free the memory allocated for the string pool
    ~StringPool() {
        delete[] stringPool; // free memory
        stringPool = nullptr;
        cout << "Memory freed!" << endl;
    }
    //add a string to the pool if there is space, otherwise print "Pool full!"
    void addString(string s) {
        if (currentSize < maxSize) stringPool[currentSize++] = s;
        else cout << "Pool full!" << endl;
    }
    //remove a string from the pool at the given index, if the index is valid
    void removeString(int index) {
        if (index < currentSize) {
            cout << "Removing: " << stringPool[index] << endl;
            stringPool[index].clear(); // memory still allocated
        }
    }
    // Display pool status
    void displayPool() {
        cout << "Pool Status: ";
        for (int i = 0; i < maxSize; i++) {
            if (stringPool[i].empty())
                cout << "[empty] ";
            else
                cout << "[" << stringPool[i] << "] ";
        }
        cout << endl;
    }
   
};

int main() {
    StringPool sp;
    sp.addString("Hello");
    sp.addString("World");
    sp.displayPool();
    sp.removeString(0);
    sp.removeString(1);
    sp.displayPool(); 
    return 0;
}

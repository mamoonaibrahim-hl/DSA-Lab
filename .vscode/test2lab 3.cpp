#include <iostream>
using namespace std;

class StringPool {
    string* stringPool;
    int currentSize;
    int maxSize;
    static int allocations;
    static int deallocations;
public:
    StringPool(int size=5) {
        maxSize = size;
        currentSize = 0;
        stringPool = new string[maxSize];
        allocations++;
    }
    
    void addString(string s) {
        if (currentSize < maxSize) stringPool[currentSize++] = s;
    }
    void removeString(int index) {
        if (index < currentSize) {
            cout << "Removing: " << stringPool[index] << endl;
            stringPool[index].clear(); // content gone, memory still allocated
        }
    }
   
    void report() {
        cout << "Allocations: " << allocations 
             << " Deallocations: " << deallocations << endl;
    }
};

int StringPool::allocations = 0;
int StringPool::deallocations = 0;

int main() {
    StringPool sp;
    sp.addString("Hello");
    sp.addString("World");
    sp.removeString(0);   // clears content, memory still allocated
    sp.removeString(1);   // clears content, memory still allocated
    sp.report();          // shows allocations vs deallocations
   
}

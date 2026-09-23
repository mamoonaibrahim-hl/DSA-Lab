#include <iostream>
using namespace std;

int main() {
    int n = 3;
    int* values = new int[n];

    //loop till n-1 not n
    for (int i = 0; i < n; i++)
        cin >> values[i];

    // Deallocate memory for the values array using right syntax
    delete[] values;
    values = nullptr;

    //no accessing freed memory, as values is set to nullptr
    return 0;
}

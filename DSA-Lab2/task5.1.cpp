#include <iostream>
using namespace std;

// Function to swap two integers using pointers
void swapValues(int *x, int *y) {  //takes addresses of two integers as parameters
    int temp = *x;   // store value at x
    *x = *y;         // assign value of y to x
    *y = temp;       // assign stored value to y (value of x)
}

int main() {
    int a = 5, b = 10;
    
    // Call the swap function using pointers
    cout << "Before swap: a = " << a << ", b = " << b << endl;
    swapValues(&a, &b);
    cout << "After swap: a = " << a << ", b = " << b << endl;

    return 0;
}

#include <iostream>
using namespace std;

//function to swap two integers using pointer to pointer
void swapValuesPP(int **x, int **y) {
    int temp = **x;   // value at address pointed by *x
    **x = **y;        // assign value of b to a
    **y = temp;       // assign stored value to b
}
int main() {
    int a = 5, b = 10;

    // Call the swap function using pointer to pointer
    int *pa = &a;
    int *pb = &b;
    int **ppa = &pa;
    int **ppb = &pb;
    cout << "Before swap: a = " << a << ", b = " << b << endl;
    swapValuesPP(ppa, ppb);
    cout << "After swap: a = " << a << ", b = " << b << endl;

    return 0;
}

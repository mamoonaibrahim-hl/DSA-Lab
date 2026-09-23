#include <iostream>
using namespace std;

int main() {
    int n;
    // Read the number of marks from the user
    cout << "Enter number of marks (1-10): ";
    cin >> n;

    //allocate memory for n marks dynamically
    int *marks = new int[n];
    for(int i = 0; i < n; i++)
        cin >> *(marks + i);

    //allocate memory for n+1 marks dynamically and copy the existing marks to the new array
    int *newMarks = new int[n+1];
    for(int i = 0; i < n; i++)
        *(newMarks + i) = *(marks + i);

    //take a new mark from the user and add it to the new array
    cout << "Enter new mark: ";
    cin >> *(newMarks + n);

    //deallocate the old array and point marks to the new array
    delete[] marks;
    marks = newMarks;
    n++;

    // Display the marks
    cout << "Marks: ";
    for(int i = 0; i < n; i++)
        cout << *(marks + i) << " ";
    cout << endl;

    // Deallocate memory for the marks array
    delete[] marks;
    marks = nullptr;

    return 0;
}

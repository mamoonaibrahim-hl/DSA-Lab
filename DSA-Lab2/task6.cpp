#include<iostream>
#include<conio.h>
using namespace std;
int main()
{
    int row, col;
    //number of students and number of subjects are entered at runtime
    cout<<"Enter the number of students and subjects: ";
    cin>>row>>col;

    //Read and validate rows and cols before allocation
    if(row<=0 || col<=0)
    {
        cout<<"Invalid input for number of students or subjects. Please enter positive integers."<<endl;
        return 1; // Exit the program with an error code
    }

    //allocate memory for a 2D array dynamically
    int **marks = new int*[row];
    for(int i=0; i<row; i++)
    {
        marks[i] = new int[col];
    }

    //Read marks from 0 to 100. Assume entered marks are valid
    cout<<"Enter the marks for each student in each subject (0-100):"<<endl;
    for(int r = 0; r < row; r++)
        for(int c = 0; c < col; c++)
            cin >> *(*(marks + r) + c);

    // Display the matrix
    cout << endl << "Marks Matrix:" << endl;
    for(int r = 0; r < row; r++)
    {
        cout << "Student " << (r+1) << ": ";
        for(int c = 0; c < col; c++)
        {
            cout << *(*(marks + r) + c) << " ";
        }
        cout << endl;
    }

    //Calculate each student’s total.
    // Display the student with the highest total and that total
    int bestTotal = 0, bestStudent = 1;
    for(int r = 0; r < row; r++) {
        int total = 0;
        for(int c = 0; c < col; c++)
            total += marks[r][c];
        cout << "Student " << (r+1) << " total = " << total << endl;
        if(r == 0 || total > bestTotal) {
            bestTotal = total;
            bestStudent = r+1;
        }
    }
    cout << "Top student: " << bestStudent << " with total " << bestTotal << endl;

    // Deallocate memory
    for(int i = 0; i < row; i++)
        delete[] marks[i];
    delete[] marks;
    marks = nullptr;

    return 0;

}
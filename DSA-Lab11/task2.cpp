#include <iostream>
using namespace std;

int totalSize = 0;                  // total number of elements (used to print the whole array)
long long comparisons = 0;     // counts every arr[j] <= pivot comparison

// print the entire array
void printArray(int arr[]) {
    cout << "[ ";
    for (int i = 0; i < totalSize; i++) {
        cout << arr[i] << " ";
    }
    cout << "]";
}

// swap two values
void swapValues(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

// Lomuto partition: pivot = last element, returns pivot's final index
int partition(int arr[], int low, int high) {
    int pivot = arr[high];     // last element is the pivot
    int i = low - 1;           // end of the "<= pivot" region (empty at start)

    for (int j = low; j < high; j++) {
        comparisons++;
        if (arr[j] <= pivot) {
            i++;
            swapValues(arr[i], arr[j]);
        }
    }

    // place the pivot between the two regions
    swapValues(arr[i + 1], arr[high]);
    int pi = i + 1;

    // print pivot value, returned index and the current array
    cout << "pivot = " << pivot << ", pi = " << pi << " -> ";
    printArray(arr);
    cout << endl;

    return pi;
}

// Recursive quick sort
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);      // left of pivot (pi excluded)
        quickSort(arr, pi + 1, high);     // right of pivot (pi excluded)
    }
}

int main() {
    char again = 'y';

    while (again == 'y' || again == 'Y') {
        //take input from the user
        cout << "Enter number of elements (n): ";
        cin >> totalSize;

        int *arr = new int[totalSize];
        if (totalSize > 0) {
            cout << "Enter " << totalSize << " elements: ";
            for (int i = 0; i < totalSize; i++) {
                cin >> arr[i];
            }
        }

        cout << "Original array: ";
        printArray(arr);
        cout << endl;

        //sort and count comparisons
        comparisons = 0;
        quickSort(arr, 0, totalSize - 1);

        cout << "Sorted array  : ";
        printArray(arr);
        cout << endl;
        cout << "Total comparisons (arr[j] <= pivot): " << comparisons << endl;

        delete[] arr;

        cout << "\nTest another array? (y/n): ";
        cin >> again;
        cout << endl;
    }
    return 0;
}
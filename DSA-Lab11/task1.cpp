#include <iostream>
using namespace std;

int totalSize = 0;   // total number of elements (used only to print the whole array)

// print the entire array
void printArray(int arr[]) {
    cout << "[ ";
    for (int i = 0; i < totalSize; i++) {
        cout << arr[i] << " ";
    }
    cout << "]";
}

// Merge two sorted halves: arr[low..mid] and arr[mid+1..high]
void merge(int arr[], int low, int mid, int high) {
    // sizes of the left and right halves
    int n1 = mid - low + 1;
    int n2 = high - mid;

    // temporary arrays (auxiliary memory)
    int *L = new int[n1];
    int *R = new int[n2];

    // copy data into the temporary arrays
    for (int i = 0; i < n1; i++) {
        L[i] = arr[low + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }

    // two-pointer merge
    int i = 0;       // index of L
    int j = 0;       // index of R
    int k = low;     // write position in arr

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {      // <= keeps the sort stable
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    // copy leftover elements of L
    while (i < n1) {
        arr[k++] = L[i++];
    }

    // copy leftover elements of R
    while (j < n2) {
        arr[k++] = R[j++];
    }

    // free the temporary memory
    delete[] L;
    delete[] R;

    // print the merged range and the entire array
    cout << "merge [" << low << ".." << high << "] -> ";
    printArray(arr);
    cout << endl;
}

// Recursive merge sort
void mergeSort(int arr[], int low, int high) {
    // base case: 0 or 1 element
    if (low >= high) {
        return;
    }

    int mid = low + (high - low) / 2;   // safe midpoint

    mergeSort(arr, low, mid);           // sort left half
    mergeSort(arr, mid + 1, high);      // sort right half
    merge(arr, low, mid, high);         // combine both halves
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

        //sort
        mergeSort(arr, 0, totalSize - 1);

        cout << "Sorted array  : ";
        printArray(arr);
        cout << endl;

        delete[] arr;

        cout << "\nTest another array? (y/n): ";
        cin >> again;
        cout << endl;
    }
    return 0;
}
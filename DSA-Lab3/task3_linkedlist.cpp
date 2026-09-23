#include <iostream>
using namespace std;

// Node structure
struct Node {
    int data;
    Node* next;
};

// LinkedList class
class LinkedList {
    Node* head;
public:
    //constructor to initialize head
    LinkedList() { head = nullptr; }
    // Destructor to free memory
    ~LinkedList() {
        Node* temp;
        while (head) {
            temp = head;
            head = head->next;
            delete temp;
        }
        cout << "All nodes freed!" << endl;
    }
    // Insert at head
    void insertAtHead(int val) {
        Node* newNode = new Node{val, head};
        head = newNode;
    }

    // Insert at 3rd position
    void insertAtThird(int val) {
        Node* newNode = new Node{val, nullptr};
        if (!head || !head->next) {
            cout << "List too short, inserting at head.\n";
            insertAtHead(val);
            return;
        }
        Node* temp = head;
        //loop is more useful in case of nth position 
        for (int i = 1; i < 2 && temp->next; i++) temp = temp->next;
        newNode->next = temp->next;
        temp->next = newNode;
    }

    // Display list
    void displayList() {
        Node* temp = head;
        while (temp) {
            cout << temp->data << " -> ";
            temp = temp->next;
        }
        cout << "NULL\n";
    }

    // Delete last node
    void deleteLast() {
        if (!head) return;
        if (!head->next) { delete head; head = nullptr; return; }
        Node* temp = head;
        while (temp->next->next) temp = temp->next;
        delete temp->next;
        temp->next = nullptr;
    }

    // Count nodes
    int countNodes() {
        int count = 0; Node* temp = head;
        while (temp) { count++; temp = temp->next; }
        return count;
    }
    // Reverse list
    void reverseList() {
        Node* prev = nullptr; Node* curr = head; Node* next = nullptr;
        while (curr) {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        head = prev;
    }

    // Search value
    void searchValue(int val) {
        Node* temp = head; int pos = 1;
        while (temp) {
            if (temp->data == val) {
                cout << "Found at position " << pos << "\n";
                return;
            }
            temp = temp->next; pos++;
        }
        cout << "Not found\n";
    }
};

// Main menu
int main() {
    LinkedList list;
    int choice, val;
    do {
        cout << "\nMenu:\n"
             << "1. Insert Head\n"
             << "2. Insert at 3rd\n"
             << "3. Display List\n"
             << "4. Delete Last\n"
             << "5. Count Nodes\n"
             << "6. Reverse List\n"
             << "7. Search Value\n"
             << "8. Exit\nChoice: ";
        cin >> choice;
        switch (choice) {
            case 1: cout << "Enter value: "; cin >> val; list.insertAtHead(val); break;
            case 2: cout << "Enter value: "; cin >> val; list.insertAtThird(val); break;
            case 3: list.displayList(); break;
            case 4: list.deleteLast(); break;
            case 5: cout << "Total nodes: " << list.countNodes() << endl; break;
            case 6: list.reverseList(); break;
            case 7: cout << "Enter value to search: "; cin >> val; list.searchValue(val); break;
        }
    } while (choice != 8);
    return 0;
}
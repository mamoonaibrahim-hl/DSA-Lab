/*
  Name: Mamoona Ibrahim
  Registration No: 542776
  Section: BSAI-2A

  DATA STRUCTURES USED

  1. Binary Search Tree (BST)
     - Stores student records using roll number as key.
     - Provides efficient searching and insertion.
     - Inorder traversal displays records in sorted order.

  2. Queue (Linked List)
     - Stores help requests.
     - Follows FIFO (First In First Out).
     - Front = first request to be served.
     - Rear = newest request.

  OVERALL COMPLEXITY

  Add Student      : Average O(log n), Worst O(n)
  Search Student   : Average O(log n), Worst O(n)
  Display Students : O(n)
  Add Help Request : O(log n) + O(1)
  Serve Request    : O(1) + O(log n)
  Show Queue       : O(r log n) average
  Space            : O(n + r)

  n = number of students
  r = number of help requests
*/

#include <iostream>
#include <cstring>
using namespace std;

const int MAX_NAME = 50;

//BST : Student Records

struct StudentNode {
    int roll;
    char name[MAX_NAME];
    StudentNode* left;
    StudentNode* right;
};

/*
   Iterative BST insertion.

   Time Complexity:
      Best    : O(1)
      Average : O(log n)
      Worst   : O(n)

   Space Complexity:
      O(1)
*/
bool addStudent(StudentNode*& root, int roll, const char* name)
{
    if (root == NULL)
    {
        root = new StudentNode;

        root->roll = roll;
        strcpy(root->name, name);

        root->left = NULL;
        root->right = NULL;

        return true;
    }

    StudentNode* parent = NULL;
    StudentNode* current = root;

    while (current != NULL)
    {
        if (roll == current->roll)
            return false;

        parent = current;

        if (roll < current->roll)
            current = current->left;
        else
            current = current->right;
    }

    StudentNode* newNode = new StudentNode;

    newNode->roll = roll;
    strcpy(newNode->name, name);

    newNode->left = NULL;
    newNode->right = NULL;

    if (roll < parent->roll)
        parent->left = newNode;
    else
        parent->right = newNode;

    return true;
}

/*
   Recursive BST search.

   Time Complexity:
      Average : O(log n)
      Worst   : O(n)

   Space Complexity:
      Average : O(log n)
      Worst   : O(n)
*/
StudentNode* searchStudent(StudentNode* root, int roll)
{
    if (root == NULL)
        return NULL;

    if (root->roll == roll)
        return root;

    if (roll < root->roll)
        return searchStudent(root->left, roll);

    return searchStudent(root->right, roll);
}

/*
   Recursive inorder traversal.

   Time Complexity:
      O(n)

   Space Complexity:
      O(h)
*/
void displayStudents(StudentNode* root)
{
    if (root == NULL)
        return;

    displayStudents(root->left);

    cout << "  Roll: "
         << root->roll
         << "  |  Name: "
         << root->name
         << endl;

    displayStudents(root->right);
}

/*
   Recursive tree deletion.

   Time Complexity:
      O(n)

   Space Complexity:
      O(h)
*/
void destroyTree(StudentNode* root)
{
    if (root == NULL)
        return;

    destroyTree(root->left);
    destroyTree(root->right);

    delete root;
}

//Queue : Help Requests

struct QueueNode
{
    int roll;
    QueueNode* next;
};

struct HelpQueue
{
    QueueNode* front;
    QueueNode* rear;
    int count;

    HelpQueue()
    {
        front = NULL;
        rear = NULL;
        count = 0;
    }
};

/*
   Check queue empty.

   Time Complexity:
      O(1)

   Space Complexity:
      O(1)
*/
bool isQueueEmpty(const HelpQueue& q)
{
    return q.front == NULL;
}

/*
   Insert at rear.

   Time Complexity:
      O(1)

   Space Complexity:
      O(1)
*/
void enqueue(HelpQueue& q, int roll)
{
    QueueNode* node = new QueueNode;

    node->roll = roll;
    node->next = NULL;

    if (q.rear == NULL)
    {
        q.front = node;
        q.rear = node;
    }
    else
    {
        q.rear->next = node;
        q.rear = node;
    }

    q.count++;
}

/*
   Remove from front.

   Time Complexity:
      O(1)

   Space Complexity:
      O(1)
*/
int dequeue(HelpQueue& q)
{
    QueueNode* temp = q.front;

    int roll = temp->roll;

    q.front = q.front->next;

    if (q.front == NULL)
        q.rear = NULL;

    delete temp;

    q.count--;

    return roll;
}

/*
   Display waiting requests.

   Time Complexity:
      O(r × h)

   Average:
      O(r log n)

   Worst:
      O(rn)

   Space Complexity:
      O(h)
*/
void displayQueue(const HelpQueue& q, StudentNode* root)
{
    int position = 1;

    QueueNode* current = q.front;

    while (current != NULL)
    {
        StudentNode* s =
            searchStudent(root, current->roll);

        cout << "  "
             << position
             << ". Roll "
             << current->roll
             << " ("
             << (s ? s->name : "?")
             << ")"
             << endl;

        current = current->next;
        position++;
    }
}

/*
   Delete all queue nodes.

   Time Complexity:
      O(r)

   Space Complexity:
      O(1)
*/
void destroyQueue(HelpQueue& q)
{
    while (!isQueueEmpty(q))
        dequeue(q);
}

//Menu

void printMenu()
{
    cout << "\nStudent Help Desk\n";
    cout << "1. Add student\n";
    cout << "2. Search student by roll number\n";
    cout << "3. Display all students\n";
    cout << "4. Add help request\n";
    cout << "5. Serve next help request\n";
    cout << "6. Show waiting requests\n";
    cout << "0. Exit\n";
    cout << "Choice: ";
}

//Main

int main()
{
    StudentNode* root = NULL;
    HelpQueue queue;

    int choice;

    do
    {
        printMenu();
        cin >> choice;

        if (choice == 1)
        {
            int roll;
            char name[MAX_NAME];

            cout << "Roll number: ";
            cin >> roll;

            cout << "Name: ";
            cin.ignore();
            cin.getline(name, MAX_NAME);

            if (addStudent(root, roll, name))
                cout << "Student added.\n";
            else
                cout << "Error: roll number "
                     << roll
                     << " already exists.\n";
        }

        else if (choice == 2)
        {
            int roll;

            cout << "Roll number to search: ";
            cin >> roll;

            StudentNode* s =
                searchStudent(root, roll);

            if (s)
            {
                cout << "Found -> Roll: "
                     << s->roll
                     << ", Name: "
                     << s->name
                     << endl;
            }
            else
            {
                cout << "Student with roll number "
                     << roll
                     << " not found.\n";
            }
        }

        else if (choice == 3)
        {
            if (root == NULL)
            {
                cout << "No students registered yet.\n";
            }
            else
            {
                cout << "Students (sorted by roll number):\n";
                displayStudents(root);
            }
        }

        else if (choice == 4)
        {
            int roll;

            cout << "Roll number requesting help: ";
            cin >> roll;

            if (searchStudent(root, roll) == NULL)
            {
                cout << "Rejected: roll number "
                     << roll
                     << " is not registered.\n";
            }
            else
            {
                enqueue(queue, roll);

                cout << "Request added. Waiting: "
                     << queue.count
                     << endl;
            }
        }

        else if (choice == 5)
        {
            if (isQueueEmpty(queue))
            {
                cout << "No requests waiting.\n";
            }
            else
            {
                int roll = dequeue(queue);

                StudentNode* s =
                    searchStudent(root, roll);

                cout << "Serving: Roll "
                     << roll
                     << " ("
                     << (s ? s->name : "?")
                     << ")\n";
            }
        }

        else if (choice == 6)
        {
            if (isQueueEmpty(queue))
            {
                cout << "No requests waiting.\n";
            }
            else
            {
                cout << "Waiting requests (front first):\n";
                displayQueue(queue, root);
            }
        }

        else if (choice != 0)
        {
            cout << "Invalid option.\n";
        }

    } while (choice != 0);

    destroyQueue(queue);
    destroyTree(root);

    cout << "Memory released. Goodbye!\n";

    return 0;
}
#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>
 
using namespace std;
 
/*
   GENERIC BINARY (MIN-)HEAP
   Cmp(a, b) must return true when a has HIGHER priority than b
   (i.e. a should be closer to the root).
     push        O(log n)
     pop         O(log n)
     replaceTop  O(log n)   (one sift instead of pop + push)
     build       O(n)       (Floyd's bottom-up heapify)
 */
template <typename T, typename Cmp>
class BinaryHeap {
    vector<T> h;
    Cmp cmp;
 
    void siftUp(size_t i) {
        T x = move(h[i]);
        while (i > 0) {
            size_t p = (i - 1) / 2;
            if (!cmp(x, h[p])) break;
            h[i] = move(h[p]);
            i = p;
        }
        h[i] = move(x);
    }
 
    void siftDown(size_t i) {
        size_t n = h.size();
        T x = move(h[i]);
        while (true) {
            size_t l = 2 * i + 1;
            if (l >= n) break;
            size_t r = l + 1;
            size_t c = (r < n && cmp(h[r], h[l])) ? r : l;  // better child
            if (!cmp(h[c], x)) break;
            h[i] = move(h[c]);
            i = c;
        }
        h[i] = move(x);
    }
 
public:
    explicit BinaryHeap(Cmp c = Cmp()) : cmp(c) {}
 
    // O(n) construction from an existing array
    explicit BinaryHeap(vector<T> items, Cmp c = Cmp()) : h(move(items)), cmp(c) {
        for (size_t i = h.size() / 2; i-- > 0;) siftDown(i);
    }
 
    bool empty() const { return h.empty(); }
    size_t size() const { return h.size(); }
    const T& top() const { return h[0]; }
 
    void push(T x) {
        h.push_back(move(x));
        siftUp(h.size() - 1);
    }
 
    T pop() {
        T r = move(h[0]);
        if (h.size() == 1) {
            h.pop_back();
            return r;
        }
        h[0] = move(h.back());
        h.pop_back();
        siftDown(0);
        return r;
    }
 
    // Replace the root with x and restore the heap in a single sift-down.
    void replaceTop(T x) {
        h[0] = move(x);
        siftDown(0);
    }
};

//TASK 2 : MERGE K SORTED LINKED LISTS (min-heap of list heads)

struct ListNode {
    int val;
    ListNode* next;
    explicit ListNode(int v) : val(v), next(nullptr) {}
};
 
struct ListNodeCmp {
    bool operator()(const ListNode* a, const ListNode* b) const { return a->val < b->val; }
};
 
// Time O(N log k) (N = total nodes), extra space O(k). Nodes are re-linked in
// place, no new nodes are allocated.
ListNode* mergeKLists(const vector<ListNode*>& lists) {
    vector<ListNode*> heads;
    heads.reserve(lists.size());
    for (ListNode* p : lists)
        if (p) heads.push_back(p);
    if (heads.empty()) return nullptr;
 
    BinaryHeap<ListNode*, ListNodeCmp> heap(move(heads));  // O(k)
 
    ListNode dummy(0);
    ListNode* tail = &dummy;
    while (!heap.empty()) {
        ListNode* smallest = heap.top();
        tail->next = smallest;
        tail = smallest;
        if (smallest->next) heap.replaceTop(smallest->next);  // one sift
        else heap.pop();
    }
    tail->next = nullptr;
    return dummy.next;
}
 
ListNode* buildList(const vector<int>& v) {
    ListNode dummy(0);
    ListNode* t = &dummy;
    for (int x : v) {
        t->next = new ListNode(x);
        t = t->next;
    }
    return dummy.next;
}
 
void printList(const ListNode* p) {
    if (!p) { cout << "(empty list)"; return; }
    for (; p; p = p->next) cout << p->val << (p->next ? " -> " : "");
}
 
void freeList(ListNode* p) {
    while (p) {
        ListNode* nx = p->next;
        delete p;
        p = nx;
    }
}
 
static bool readInt(const string& prompt, long long& out) {
    cout << prompt;
    if (cin >> out) return true;
    if (cin.eof()) return false;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid input.\n";
    return false;
}
 
 
static void task2Interactive() {
    long long k;
    if (!readInt("Enter k (number of linked lists): ", k) || k < 0) return;
    vector<ListNode*> lists;
    for (long long i = 0; i < k; ++i) {
        long long len;
        if (!readInt("List " + to_string(i + 1) + " - length: ", len) || len < 0) break;
        vector<int> v(len);
        cout << "  enter " << len << " values in ascending order: ";
        for (auto& x : v) cin >> x;
        if (!is_sorted(v.begin(), v.end())) cout << "  (warning: list is not sorted)\n";
        lists.push_back(buildList(v));
    }
    ListNode* merged = mergeKLists(lists);
    cout << "Merged list: ";
    printList(merged);
    cout << "\n";
    freeList(merged);
}

 
int main() {
    task2Interactive();
    return 0;
}

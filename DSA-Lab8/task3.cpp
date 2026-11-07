#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>
 
using namespace std;
 
/* 
   TASK 3 : SELF-HEALING AVL TREE WITH TOP-K QUERIES
 
   Each node stores (besides key/left/right):
     cnt    - multiplicity (duplicates are kept in ONE node)
     height - AVL height
     size   - total number of elements in the subtree (with duplicates)
     sum    - sum of all elements in the subtree
   size/sum let us answer "sum of k largest/smallest" in O(log n).
 */
enum class Criterion { MAX = 1, MIN = 2, SUM = 3 };
 
struct AvlNode {
    int key;
    int cnt;
    int height;
    long long size;
    long long sum;
    AvlNode *left, *right;
    explicit AvlNode(int k)
        : key(k), cnt(1), height(1), size(1), sum(k), left(nullptr), right(nullptr) {}
};
 
class AvlTopK {
    AvlNode* root = nullptr;
    vector<string> events;  // rotations performed during the current operation
 
    //helpers
    static int H(const AvlNode* n) { return n ? n->height : 0; }
    static long long SZ(const AvlNode* n) { return n ? n->size : 0; }
    static long long SM(const AvlNode* n) { return n ? n->sum : 0; }
    static int balance(const AvlNode* n) { return n ? H(n->left) - H(n->right) : 0; }
 
    static void pull(AvlNode* n) {  // recompute augmented data from children
        n->height = 1 + max(H(n->left), H(n->right));
        n->size = SZ(n->left) + SZ(n->right) + n->cnt;
        n->sum = SM(n->left) + SM(n->right) + (long long)n->key * n->cnt;
    }
 
    //rotations
    static AvlNode* rotateRight(AvlNode* y) {
        AvlNode* x = y->left;
        y->left = x->right;
        x->right = y;
        pull(y);
        pull(x);
        return x;
    }
    static AvlNode* rotateLeft(AvlNode* x) {
        AvlNode* y = x->right;
        x->right = y->left;
        y->left = x;
        pull(x);
        pull(y);
        return y;
    }
 
    // Restores the AVL property at n (children are already balanced).
    AvlNode* rebalance(AvlNode* n) {
        pull(n);
        int b = balance(n);
        if (b > 1) {                              // left heavy
            if (balance(n->left) < 0) {           // Left-Right case
                events.push_back("LR case at " + to_string(n->key) + ": rotateLeft(" +
                                 to_string(n->left->key) + "), rotateRight(" +
                                 to_string(n->key) + ")");
                n->left = rotateLeft(n->left);
            } else {                              // Left-Left case
                events.push_back("LL case at " + to_string(n->key) + ": rotateRight(" +
                                 to_string(n->key) + ")");
            }
            return rotateRight(n);
        }
        if (b < -1) {                             // right heavy
            if (balance(n->right) > 0) {          // Right-Left case
                events.push_back("RL case at " + to_string(n->key) + ": rotateRight(" +
                                 to_string(n->right->key) + "), rotateLeft(" +
                                 to_string(n->key) + ")");
                n->right = rotateRight(n->right);
            } else {                              // Right-Right case
                events.push_back("RR case at " + to_string(n->key) + ": rotateLeft(" +
                                 to_string(n->key) + ")");
            }
            return rotateLeft(n);
        }
        return n;
    }
 
    //insert / delete 
    AvlNode* insertRec(AvlNode* n, int key) {
        if (!n) return new AvlNode(key);
        if (key < n->key) n->left = insertRec(n->left, key);
        else if (key > n->key) n->right = insertRec(n->right, key);
        else n->cnt++;                            // duplicate -> bump counter
        return rebalance(n);
    }
 
    // all == true  : remove the whole node (used for successor relocation)
    // all == false : remove ONE occurrence of key
    AvlNode* removeRec(AvlNode* n, int key, bool all, bool& removed) {
        if (!n) return nullptr;
        if (key < n->key) {
            n->left = removeRec(n->left, key, all, removed);
        } else if (key > n->key) {
            n->right = removeRec(n->right, key, all, removed);
        } else {
            removed = true;
            if (!all && n->cnt > 1) {
                n->cnt--;
            } else if (!n->left || !n->right) {   // 0 or 1 child
                AvlNode* child = n->left ? n->left : n->right;
                delete n;
                return child;
            } else {                              // 2 children: use in-order successor
                AvlNode* s = n->right;
                while (s->left) s = s->left;
                n->key = s->key;
                n->cnt = s->cnt;
                bool dummy = false;
                n->right = removeRec(n->right, s->key, true, dummy);
            }
        }
        return rebalance(n);
    }
 
    //queries
    // reverse in-order, stops as soon as k elements were collected: O(log n + k)
    static void collectLargest(const AvlNode* n, size_t k, vector<int>& out) {
        if (!n || out.size() >= k) return;
        collectLargest(n->right, k, out);
        for (int c = 0; c < n->cnt && out.size() < k; ++c) out.push_back(n->key);
        collectLargest(n->left, k, out);
    }
    static void collectSmallest(const AvlNode* n, size_t k, vector<int>& out) {
        if (!n || out.size() >= k) return;
        collectSmallest(n->left, k, out);
        for (int c = 0; c < n->cnt && out.size() < k; ++c) out.push_back(n->key);
        collectSmallest(n->right, k, out);
    }
 
    static void printRec(const AvlNode* n, int depth) {
        if (!n) return;
        printRec(n->right, depth + 1);
        cout << string(depth * 6, ' ') << n->key;
        if (n->cnt > 1) cout << "(x" << n->cnt << ")";
        cout << "  [h=" << n->height << ", bf=" << balance(n) << "]\n";
        printRec(n->left, depth + 1);
    }
 
    static void freeRec(AvlNode* n) {
        if (!n) return;
        freeRec(n->left);
        freeRec(n->right);
        delete n;
    }
 
    // Validates BST order, heights, |bf| <= 1 and the augmented data.
    static bool validRec(const AvlNode* n, long long lo, long long hi) {
        if (!n) return true;
        if (n->key <= lo || n->key >= hi) return false;
        if (abs(balance(n)) > 1) return false;
        if (n->height != 1 + max(H(n->left), H(n->right))) return false;
        if (n->size != SZ(n->left) + SZ(n->right) + n->cnt) return false;
        if (n->sum != SM(n->left) + SM(n->right) + (long long)n->key * n->cnt) return false;
        return validRec(n->left, lo, n->key) && validRec(n->right, n->key, hi);
    }
 
    void flushEvents() {
        if (events.empty()) cout << "  [balanced - no rotation needed]\n";
        for (const string& e : events) cout << "  [rotation] " << e << "\n";
        events.clear();
    }
 
public:
    AvlTopK() = default;
    AvlTopK(const AvlTopK&) = delete;
    AvlTopK& operator=(const AvlTopK&) = delete;
    ~AvlTopK() { freeRec(root); }
 
    bool empty() const { return root == nullptr; }
    long long size() const { return SZ(root); }
    bool isValidAvl() const {
        return validRec(root, numeric_limits<long long>::min(), numeric_limits<long long>::max());
    }
 
    void print() const {
        cout << "  Tree (right subtree on top, root at left):\n";
        if (!root) { cout << "      (empty tree)\n"; return; }
        printRec(root, 0);
    }
 
    // Insert one element; rebalance automatically; print the tree afterwards.
    void insert(int key, bool show = true) {
        root = insertRec(root, key);
        if (show) {
            cout << "Inserted " << key << "\n";
            flushEvents();
            print();
        } else {
            events.clear();
        }
    }
 
    // Insert many elements, printing the tree after EACH insertion.
    void insertMany(const vector<int>& keys) {
        for (int k : keys) insert(k);
    }
 
    // Remove one occurrence of key. Returns false if it was not present.
    bool erase(int key, bool show = true) {
        bool removed = false;
        root = removeRec(root, key, false, removed);
        if (show) {
            if (removed) {
                cout << "Deleted " << key << "\n";
                flushEvents();
                print();
            } else {
                cout << "Element " << key << " not found.\n";
                events.clear();
            }
        } else {
            events.clear();
        }
        return removed;
    }
 
    // update(old -> new) = delete + insert (both self-balance)
    bool update(int oldKey, int newKey) {
        cout << "Update " << oldKey << " -> " << newKey << "\n";
        if (!erase(oldKey)) return false;
        insert(newKey);
        return true;
    }
 
    // Elements selected by the criterion:
    //   MAX, SUM -> the k largest (descending);  MIN -> the k smallest (ascending)
    vector<int> topK(size_t k, Criterion c) const {
        vector<int> out;
        k = min<long long>((long long)k, SZ(root));
        out.reserve(k);
        if (c == Criterion::MIN) collectSmallest(root, k, out);
        else collectLargest(root, k, out);
        return out;
    }
 
    // Sum of the k largest elements in O(log n) using the augmented sums.
    long long sumOfLargest(long long k) const {
        long long total = 0;
        const AvlNode* n = root;
        k = min(k, SZ(root));
        while (n && k > 0) {
            long long rs = SZ(n->right);
            if (k <= rs) {
                n = n->right;
            } else {
                total += SM(n->right);
                k -= rs;
                long long take = min<long long>(k, n->cnt);
                total += take * n->key;
                k -= take;
                n = n->left;
            }
        }
        return total;
    }
 
    // Query + print for the user-chosen criterion.
    void queryAndPrint(size_t k, Criterion c) const {
        if (!root) { cout << "Tree is empty.\n"; return; }
        vector<int> r = topK(k, c);
        const char* name = c == Criterion::MAX ? "MAX (k largest)"
                         : c == Criterion::MIN ? "MIN (k smallest)"
                                               : "SUM (k largest + their sum)";
        cout << "Top " << r.size() << " by " << name << ": ";
        for (size_t i = 0; i < r.size(); ++i) cout << r[i] << (i + 1 < r.size() ? ", " : "");
        if (c == Criterion::SUM) cout << "  | sum = " << sumOfLargest((long long)r.size());
        cout << "\n";
    }
 
    // Delete the top-k elements (same selection as topK), one at a time, each
    // deletion rebalances the tree and prints the updated structure.
    void deleteTopK(size_t k, Criterion c) {
        if (!root) { cout << "Tree is empty.\n"; return; }
        vector<int> victims = topK(k, c);  // O(log n + k)
        cout << "Deleting " << victims.size() << " element(s) selected by the criterion...\n";
        for (int v : victims) erase(v);
    }
};
 
static bool readInt(const string& prompt, long long& out) {
    cout << prompt;
    if (cin >> out) return true;
    if (cin.eof()) return false;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid input.\n";
    return false;
}
 
static bool askCriterion(Criterion& c) {
    long long x;
    if (!readInt("Criterion (1 = Maximum, 2 = Minimum, 3 = Sum of top-k): ", x)) return false;
    if (x < 1 || x > 3) { cout << "Invalid criterion.\n"; return false; }
    c = static_cast<Criterion>(x);
    return true;
}
 
static void task3Interactive() {
    AvlTopK t;
    while (true) {
        cout << "\nTask 3: AVL Top-K \n"
                "1. Insert multiple elements\n"
                "2. Query top-k\n"
                "3. Delete top-k\n"
                "4. Update an element\n"
                "5. Print tree\n"
                "0. Back\n";
        long long ch;
        if (!readInt("Choice: ", ch)) { if (cin.eof()) return; continue; }
        if (ch == 0) return;
        if (ch == 1) {
            long long m;
            if (!readInt("How many elements? ", m) || m <= 0) continue;
            vector<int> v(m);
            cout << "Enter " << m << " integers: ";
            for (auto& x : v) cin >> x;
            t.insertMany(v);
        } else if (ch == 2 || ch == 3) {
            long long k;
            Criterion c;
            if (!readInt("k = ", k) || k <= 0 || !askCriterion(c)) continue;
            if (ch == 2) t.queryAndPrint((size_t)k, c);
            else t.deleteTopK((size_t)k, c);
        } else if (ch == 4) {
            long long a, b;
            if (!readInt("Old value: ", a) || !readInt("New value: ", b)) continue;
            t.update((int)a, (int)b);
        } else if (ch == 5) {
            t.print();
        } else {
            cout << "Invalid choice.\n";
        }
    }
}
 
int main() {
    task3Interactive();
    return 0;
}
 


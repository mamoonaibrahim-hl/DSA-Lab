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

//TASK 1 : HUFFMAN CODING
struct HNode {
    char ch;           // meaningful only for leaves
    long long freq;
    int order;         // creation order -> deterministic tie-breaking
    HNode *left, *right;
    HNode(char c, long long f, int o, HNode* l = nullptr, HNode* r = nullptr)
        : ch(c), freq(f), order(o), left(l), right(r) {}
    bool isLeaf() const { return !left && !right; }
};
 
struct HNodeCmp {
    bool operator()(const HNode* a, const HNode* b) const {
        if (a->freq != b->freq) return a->freq < b->freq;
        return a->order < b->order;  // older node first on ties
    }
};
 
static void huffPreorder(const HNode* n, string& code, vector<pair<char, string>>& out) {
    if (!n) return;
    if (n->isLeaf()) {                       // visit (leaf = emit its code)
        out.emplace_back(n->ch, code.empty() ? string("0") : code);
        return;
    }
    code.push_back('0');                     // left edge  = 0
    huffPreorder(n->left, code, out);
    code.back() = '1';                       // right edge = 1
    huffPreorder(n->right, code, out);
    code.pop_back();
}
 
static void freeHuff(HNode* n) {
    if (!n) return;
    freeHuff(n->left);
    freeHuff(n->right);
    delete n;
}
 
// Builds the Huffman tree with a binary heap and returns (char, code) pairs
// in PREORDER of the tree.   Time O(N log N), space O(N).
vector<pair<char, string>> huffmanCodes(const string& S, const vector<long long>& f) {
    vector<pair<char, string>> result;
    size_t n = S.size();
    if (n == 0 || f.size() != n) return result;
 
    vector<HNode*> leaves;
    leaves.reserve(n);
    int order = 0;
    for (size_t i = 0; i < n; ++i) leaves.push_back(new HNode(S[i], f[i], order++));
 
    BinaryHeap<HNode*, HNodeCmp> heap(move(leaves));  // O(N) heapify
 
    while (heap.size() > 1) {
        HNode* a = heap.pop();  // smallest  -> left
        HNode* b = heap.pop();  // 2nd smallest -> right
        heap.push(new HNode('\0', a->freq + b->freq, order++, a, b));
    }
 
    HNode* root = heap.pop();
    string code;
    huffPreorder(root, code, result);
    freeHuff(root);
    return result;
}
 
static void printHuffman(const vector<pair<char, string>>& codes) {
    cout << "Huffman codes in preorder: ";
    for (size_t i = 0; i < codes.size(); ++i) cout << codes[i].second << (i + 1 < codes.size() ? " " : "");
    cout << "\nChar -> code (preorder):\n";
    for (const auto& p : codes) cout << "  '" << p.first << "' -> " << p.second << "\n";
}
 
 
int main() {
    long long n;
    cout << "Enter N (number of distinct characters): ";
    if (!(cin >> n) || n <= 0) { cout << "Invalid N.\n"; return 1; }
 
    string S;
    cout << "Enter the string S of " << n << " distinct characters (no spaces): ";
    cin >> S;
    if ((long long)S.size() != n) { cout << "Length of S must equal N.\n"; return 1; }
 
    string sorted = S;
    sort(sorted.begin(), sorted.end());
    if (adjacent_find(sorted.begin(), sorted.end()) != sorted.end()) {
        cout << "Characters must be distinct.\n";
        return 1;
    }
 
    vector<long long> f(n);
    cout << "Enter the " << n << " frequencies: ";
    for (auto& x : f)
        if (!(cin >> x) || x < 0) { cout << "Invalid frequency.\n"; return 1; }
 
    printHuffman(huffmanCodes(S, f));
    return 0;
}

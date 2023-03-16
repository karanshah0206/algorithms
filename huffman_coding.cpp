#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>

using namespace std;

// ADT for node in huffman tree
struct Node {
    char c;
    size_t freq;
    Node *left, *right;
    Node(char aC, size_t aFreq, Node* aLeft = nullptr, Node* aRight = nullptr) {
        c = aC;
        freq = aFreq;
        left = aLeft;
        right = aRight;
    }
};

// used to instruct priority queue to take frequency as the key for ordering
struct Compare { bool operator()(Node* a, Node* b) { return a->freq > b->freq; } };

// build huffman tree and return root node
Node* buildHuffmanTree(const string& s) {
    // get frequency of each alphabet in string
    unordered_map<char, size_t> frequencies;
    for (const char& c : s) frequencies[c]++;
    // create priority queue of nodes (ascending)
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto it = frequencies.begin(); it != frequencies.end(); it++)
        pq.push(new Node(it->first, it->second));
    // get the two nodes with lowest frequencies and combine them into one node until only root node left
    while (pq.size() > 1) {
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();
        pq.push(new Node('.', left->freq + right->freq, left, right));
    }
    // return the root node
    return pq.top();
}

// create a map of each character to its respective binary encoding
void getEncodings(Node* node, unordered_map<char, string>& encodings, string accumulated = "") {
    if (node->c != '.') encodings[node->c] = accumulated;
    else {
        if (node->left != nullptr) getEncodings(node->left, encodings, accumulated + "1");
        if (node->right != nullptr) getEncodings(node->right, encodings, accumulated + "0");
    }
}

// print out the encodings table
void showEncodings(const unordered_map<char, string>& encodings) {
    for (const pair<char, string>& encoding : encodings)
        cout << encoding.first << ":" << encoding.second << endl;
}

// encode the string using the encodings table and return the encoded string
string getEncodedString(const string& s, unordered_map<char, string>& encodings) {
    string res = "";
    for (const char& c : s) res += encodings[c];
    return res;
}

int main() {
    // get string from user
    string s; cin >> s;
    // create Huffman tree for string
    Node* root = buildHuffmanTree(s);
    // get encodings
    unordered_map<char, string> encodings;
    getEncodings(root, encodings);
    // print out the encoding table
    showEncodings(encodings);
    // show the encoded string
    cout << getEncodedString(s, encodings) << endl;
    return 0;
}

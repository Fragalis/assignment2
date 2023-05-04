#include "main.h"

class HuffNode {
private:
    char _value;
    int _weight;
    HuffNode *_hn_left;
    HuffNode *_hn_right;

public:
    HuffNode(char value = 0, int weight = 0) {
        this->_value = value;
        this->_weight = weight;
        this->_hn_left = NULL;
        this->_hn_right = NULL;
    }

    ~HuffNode() {
        this->_value = 0;
        this->_weight = 0;
        _hn_left = 0;
        _hn_right = 0;
    }

    char GetValue() {
        return this->_value;
    }

    int GetWeight() {
        return this->_weight;
    }

    HuffNode *GetLeftPtr() const {
        return this->_hn_left;
    }

    void SetPtrLeft(HuffNode *hn_left) {
        this->_hn_left = hn_left;
    }

    HuffNode *GetRightPtr() const {
        return this->_hn_right;
    }

    void SetPtrRight(HuffNode *hn_right) {
        this->_hn_right = hn_right;
    }

    bool isLeaf() {
        return (this->_hn_left == 0 && this->_hn_right == 0);
    }

    void Encode(HuffNode *ptr, unordered_map<char,string> &code, string char_str) {
        //cout << number << endl;
        if(ptr->GetLeftPtr()) Encode(ptr->GetLeftPtr(), code, char_str + "0");

        if(ptr->GetRightPtr()) Encode(ptr->GetRightPtr(), code, char_str + "1");

        if(ptr->isLeaf()) code[ptr->GetValue()] = char_str;
    }
};

void add(list<HuffNode*> &listNode, HuffNode* node) {
    if(listNode.empty()) {
        listNode.push_back(node);
        return;
    }
    for(auto it = listNode.begin(); it != listNode.end(); ++it) {
        if((*it)->GetWeight() > node->GetWeight()) {
            listNode.insert(it, node);
            return;
        }
    }
    listNode.push_back(node);
}

class Compare {
public:
    bool operator()(HuffNode *a, HuffNode *b) {
        return (a->GetWeight() < b->GetWeight());
    }
};


void Delete(HuffNode *ptr) {
    if(!ptr) return;
    if(ptr->GetLeftPtr()) Delete(ptr->GetLeftPtr());
    if(ptr->GetRightPtr()) Delete(ptr->GetRightPtr());
    // cout << "DELETE " << ptr->GetValue() << " : " << ptr->GetWeight() << endl; 
    delete ptr;
    ptr = NULL; 
}

HuffNode *GenerateHuffTree(list<HuffNode*> listNode) {
    while(listNode.size() > 1) {
        // Get Node with lowest Frequency, initalize it to left
        HuffNode *hn_left = listNode.front();
        listNode.pop_front();

        // Get Node with lowest Frequency, initalize it to right
        HuffNode *hn_right = listNode.front();
        listNode.pop_front();

        // CREATE NEW NODE:
        HuffNode *newNode = new HuffNode('.', hn_left->GetWeight() + hn_right->GetWeight());
        newNode->SetPtrLeft(hn_left);
        newNode->SetPtrRight(hn_right);

        // Push it back to heap:
        add(listNode, newNode);
    }
    return listNode.front();
}

int HuffmanEncoding(string name) {

    // An ARRAY to store FREQUENCY (A-Z) - (a-z) in "name"
    unordered_map<char, int> frequency;
    for(char c : name) frequency[c]++;
    if(frequency.size() == 1) return 1;

    // INITIALIZE MIN-HEAP FOR HUFFMAN NODE AS LIST
    list<HuffNode*> listNode;
    for(char c = 'A'; c <= 'Z'; ++c) {
        if(!frequency[c]) continue;
        HuffNode *newNode = new HuffNode(c, frequency[c]);
        add(listNode, newNode);
    }
    for(char c = 'a'; c <= 'z'; ++c) {
        if(!frequency[c]) continue;
        HuffNode *newNode = new HuffNode(c, frequency[c]);
        add(listNode, newNode);
    }

    // Generate Node
    HuffNode *root = GenerateHuffTree(listNode);

    // Huffman convert
    unordered_map<char,string> code;
    root->Encode(root, code, "");

    // RESULT CHECK:
    for(auto m : code) cout << m.first << " " << m.second << endl;

    // MAXIMUM BITS - refer to Assignment 2 Constraints
    const int MAX_BITS = 15;
    string huffmanCode_str = "";

    // Traverse every char in "name" && convert
    for(int i = 0; i < (int)name.length(); ++i) {
        char c = name[i];
        huffmanCode_str += code[c];

        // IF EXCEED: break;
        if(huffmanCode_str.length() >= MAX_BITS) break;
    }

    // RESULT CHECK:
    cout << huffmanCode_str << endl;

    int huffmanCode = 0;
    for(int i = 0; i < MAX_BITS; ++i) huffmanCode = huffmanCode * 2 + (huffmanCode_str[i] - '0');

    Delete(root);
    return huffmanCode;
}
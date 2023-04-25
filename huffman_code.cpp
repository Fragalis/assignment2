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

    void Encode(HuffNode *ptr, vector<int> &result, vector<int> &bits, int number, int bitCount, bool caseSensitive) {
        //cout << number << endl;
        if(ptr->GetLeftPtr()) Encode(ptr->GetLeftPtr(), result, bits, number*2, bitCount + 1, caseSensitive);

        if(ptr->GetRightPtr()) Encode(ptr->GetRightPtr(), result, bits, number*2+1, bitCount + 1, caseSensitive);

        if(ptr->isLeaf()) {
            char ptr_value = ptr->GetValue();
            if(caseSensitive) { // CASE SENSITIVE
                if('A' <= ptr_value && ptr_value <= 'Z') {
                    result[ptr_value - 'A'] = number;
                    bits[ptr_value - 'A'] = bitCount;
                }
                if('a' <= ptr_value && ptr_value <= 'z') {
                    result[ptr_value - 'a' + 26] = number;
                    bits[ptr_value - 'a' + 26] = bitCount;
                }  
            }
            else { // NO CASE SENSITIVE -> Therefore there's no need to check UPPERCASE-LETTER (AUTO UPPERCASE)
                result[ptr_value - 'A'] = number;         
                bits[ptr_value - 'A'] = bitCount;     
            }
        }
    }
};

class Compare {
public:
    bool operator()(HuffNode *a, HuffNode *b) {
        return a->GetWeight() > b->GetWeight();
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

HuffNode *GenerateHuffTree(priority_queue<HuffNode*, vector<HuffNode*>, Compare> pq) {
    while(pq.size() > 1) {

        // Get Node with lowest Frequency, initalize it to left
        HuffNode *hn_left = pq.top();
        pq.pop();

        // Get Node with lowest Frequency, initalize it to right
        HuffNode *hn_right = pq.top();
        pq.pop();

        // CREATE NEW NODE:
        HuffNode *newNode = new HuffNode('.', hn_left->GetWeight() + hn_right->GetWeight());
        newNode->SetPtrLeft(hn_left);
        newNode->SetPtrRight(hn_right);

        // Push it back to heap:
        pq.push(newNode);
    }
    return pq.top();
}

int HuffmanEncoding(string name, bool caseSensitive) {

    // An ARRAY to store FREQUENCY (A-Z) - (a-z) in "name"
    vector<int> freq(52, 0);
    string shortName = "";

    for(int i = 0; i < (int)name.length(); ++i) {
        char c = name[i];

        if(caseSensitive) { // CASE SENSITIVE
            if('a' <= c && c <= 'z') {
                if(!freq[c - 'a' + 26]) shortName += c;
                freq[c - 'a' + 26]++;
            }
            if('A' <= c && c <= 'Z') {
                if(!freq[c - 'A']) shortName += c;
                freq[c - 'A']++;
            }
        }
        else {
            if('a' <= c && c <= 'z') {
                if(!freq[c - 'a']) shortName += c;
                freq[c - 'a']++;
            }
            if('A' <= c && c <= 'Z') {
                if(!freq[c - 'A']) shortName += c;
                freq[c - 'A']++;
            }
        }
    }
    // INITIALIZE MIN-HEAP FOR HUFFMAN NODE
    priority_queue<HuffNode*, vector<HuffNode*>, Compare> pq;
    for(int i = 0; i < 52; ++i) {
        if(freq[i]) {
            if(i < 26) {
                HuffNode *newNode = new HuffNode(i + 'A', freq[i]);
                pq.push(newNode);
            }
            else {
                HuffNode *newNode = new HuffNode(i + 'a' - 26, freq[i]);
                pq.push(newNode);
            }
        }
    }

    HuffNode *root = GenerateHuffTree(pq);
    // An ARRAY to store RESULT (A-Z) - (a-z) after Encoding
    vector<int> code(52, 0);
    vector<int> bits(52, 0);
    root->Encode(root, code, bits, 0, 0, caseSensitive);
    
    // RESULT CHECK
    // for(int i = 0; i < 26; ++i) {
    //     cout << char(i + 'A') << ": " << code[i] << " \tBits = " << bits[i] << endl;
    // }

    int huffmanCode = 0;
    int huffmanBits = 0;

    // MAXIMUM BITS - refer to Assignment 2 Constraints
    const int MAX_BITS = 15;

    // Traverse every char in "name" && convert
    for(int i = 0; i < name.length(); ++i) {
        char c = name[i];
        int addBits = 0; // The bit amount to add in huffmanCode
        int excesssBits = 0; // The bit got overflowed
        if(caseSensitive) { // CASE SENSITIVE
            if('a' <= c && c <= 'z') addBits = bits[c - 'a' + 26];
            if('A' <= c && c <= 'Z') addBits = bits[c - 'A'];
        }
        else {
            if('a' <= c && c <= 'z') addBits = bits[c - 'a'];
            if('A' <= c && c <= 'Z') addBits = bits[c - 'A'];           
        }

        if(huffmanBits + addBits >= MAX_BITS) excesssBits = (huffmanBits + addBits) - MAX_BITS;

        // Shift left huffmanCode by (addBits - excessBits) bits
        huffmanCode = huffmanCode << addBits - excesssBits;

        // DO NOT ASK.
        if(caseSensitive) { // CASE SENSITIVE
            if('a' <= c && c <= 'z') huffmanCode = huffmanCode + (code[c - 'a' + 26] >> excesssBits);
            if('A' <= c && c <= 'Z') huffmanCode = huffmanCode + (code[c - 'A'] >> excesssBits);
        }
        else {
            if('a' <= c && c <= 'z') huffmanCode = huffmanCode + (code[c - 'a'] >> excesssBits);
            if('A' <= c && c <= 'Z') huffmanCode = huffmanCode + (code[c - 'A'] >> excesssBits);
        }

        // Update added bits so far
        huffmanBits = huffmanBits + addBits - excesssBits;
        // cout << "letter: " << c << " after: " << huffmanCode << " bits: " << huffmanBits << endl;
        
        // IF EXCEED: break;
        if(huffmanBits >= MAX_BITS) break;
    }

    Delete(root);
    return huffmanCode;
}
#include "huffman_code.cpp"

int main() {
    string name = "Johnuigfifbahjasbdfhjbasdhjf";
    map<char,int> mp;
    for(char c : name) mp[c]++;
    // for(auto m : mp) cout << m.first << " " << m.second << endl;
    cout << endl;
    cout << HuffmanEncoding(name);
}
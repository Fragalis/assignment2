#include "main.h"

class Table {
public:
    int id, result;
    string name;
    Table(int id = 0, int result = 0, string name = "") {
        this->id = id;
        this->result = result;
        this->name = name;
    }
    ~Table() {
        ResetTable();
    }

    void SetTable(const Table &table) {
        this->id = table.id;
        this->result = table.result;
        this->name = table.name;
    }

    void SetTable(const int &id, const int &result, const string &name) {
        this->id = id;
        this->result = result;
        this->name = name;
    }

    void ResetTable() { 
        id = 0;
        result = 0;
        name = "";
    }

    bool IsEmpty() {
        return (id == 0);
    }

    bool Equal(const int &id, const int &result) {
        return (this->id == id) && (this->result == result);
    }

    bool Equal(const int &id, const int &result, const string &name) {
        return (this->id == id) && (this->result == result) && (this->name == name);
    }

    void Print() {
        cout << "ID: " << id << " DATA: " << result << " NAME: " << name;
    }
};

class HashTable {
private:
    Table key[MAXSIZE];
    int size;

    // ASSUME HASHTABLE IS NEVER FULL
    int _findAddress(int address) { // This is called when Linear Probing is needed.
        int index = address;

        // Traverse the HASHMAP for empty table
        while(key[index%MAXSIZE].IsEmpty() == false) ++index;
        return (index%MAXSIZE);
    }

    string _findName(int id, int result) {
        for(int i = 0; i < MAXSIZE; ++i) {
            if(key[i].Equal(id, result)) return key[i].name;
        }
        return "";
    }

    void _clear() {
        for(int i = 0; i < MAXSIZE; ++i) key[i].ResetTable();
    }

    void _print() {
        if(size == 0) cout << "NO ELEMENT" << endl;
        for(int i = 0; i < MAXSIZE; ++i) {
            if(!key[i].IsEmpty()) {
                cout << "Address: " << i << "\t";
                key[i].Print();
                cout << endl;
            }          
        }
    }
public:
    HashTable() {
        this->size = 0;
        for(int i = 0; i < MAXSIZE; ++i) key[i].ResetTable();
    }

    ~HashTable() {
        Clear();
    }

    int GetSize() {
        return this->size;
    }

    bool IsFull() {
        return (this->size >= (MAXSIZE>>1));
    }

    void AddTable(Table table, int address) {
        if(IsFull() || address < 0 || address >= MAXSIZE) return;
        if(key[address].IsEmpty() == false) address = _findAddress(address);
        key[address].SetTable(table);
        ++size;
    }

    // Remove table which has the id (id)
    void RemoveTable(int id) {
        for(int i = 0; i < MAXSIZE; ++i) {
            if(id == key[i].id) {
                key[i].ResetTable();
                return;
            }
        }
    }

    string FindName(int id, int result) {
        return _findName(id, result);
    }

    void Clear() {
        _clear();
        this->size = 0;
    }

    // For testing purposes
    void PrintTable() {
        _print();
        cout << endl;
    }

    void Print_Command() {
        for(int i = 0; i < MAXSIZE; ++i) {
            if(!key[i].IsEmpty()) cout << key[i].id << "-" << key[i].result << "\n";
        }
    }
};
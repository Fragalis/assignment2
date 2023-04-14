#include "main.h"

class Table {
public:
    int id, result;
    Table(int id = 0, int result = 0) {
        this->id = id;
        this->result = result;
    }
    ~Table() {
        id = 0;
        result = 0;
    }

    void SetTable(const Table &table) {
        this->id = table.id;
        this->result = table.result;
    }

    void ResetTable() {
        id = 0;
        result = 0;
    }

    bool IsEmpty() {
        return (id == 0);
    }

    void Print() {
        cout << "ID: " << id << " DATA: " << result;
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

    void _print() {
        for(int i = 0; i < size; ++i) {
            cout << "Address: " << i << "\t";
            key[i].Print();
            cout << endl;
        }
    }
public:
    HashTable() {
        this->size = 0;
        for(int i = 0; i < MAXSIZE; ++i) key[i].ResetTable();
    }

    ~HashTable() {
        for(int i = 0; i < MAXSIZE; ++i) key[i].ResetTable();
        this->size = 0;
    }

    bool IsFull() {
        return (size >= MAXSIZE);
    }

    void AddTable(Table table, int address) {
        if(IsFull() || address < 0 || address >= MAXSIZE) return;
        if(key[address].IsEmpty() == false) address = _findAddress(address);
        key[address].SetTable(table);
        ++size;
    }

    void PrintTable() {
        _print();
    }
};
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

    void _clear() {
        for(int i = 0; i < MAXSIZE; ++i) key[i].ResetTable();
    }

    void _print() {
        if(size == 0) cout << "NO ELEMENT" << endl;
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
        Clear();
    }

    int GetSize() {
        return this->size;
    }

    bool IsFull() {
        return (this->size >= MAXSIZE);
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

    void Clear() {
        _clear();
        this->size = 0;
    }

    void PrintTable() {
        _print();
        cout << endl;
    }
};
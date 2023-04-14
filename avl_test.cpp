#include "avl.cpp"

int main() {
    srand(time(NULL));
    const int SIZE = 16;
    AVLTree *avl_sample = new AVLTree();
    int table_result[SIZE];
    for(int i = 0; i < SIZE; ++i) table_result[i] = 1;

    // CONSTRUCTOR
    for(int i = 0; i < SIZE; ++i) {
        Table table;
        table.id = (i*i + 16)%(SIZE*16) + 1;
        table.result = table_result[i];
        cout << "Add: "; table.Print(); cout << endl;
        avl_sample->InsertTable(table);
    }
    avl_sample->PrintTree(); cout << endl;

    // DESTRUCTOR
    for(int i = 0; i < 1; ++i) {
        Table table;
        table.id = 33;
        table.result = table_result[15];
        cout << "Delete: "; table.Print(); cout << endl;
        avl_sample->DeleteTable(table);
    }
    avl_sample->PrintTree();
    avl_sample->~AVLTree();
}
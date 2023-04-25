#include "avl.cpp"

int main() {
    srand(time(NULL));
    const int SIZE = 5;
    AVLTree *avl_sample = new AVLTree();
    int table_result[SIZE] = {5, 7, 10, 9, 15};

    // CONSTRUCTOR
    for(int i = 0; i < SIZE; ++i) {
        Table table;
        table.id = i;
        table.result = table_result[i];
        cout << "Add: "; table.Print(); cout << endl;
        avl_sample->InsertTable(table);
    avl_sample->PrintTree(); cout << endl;
    }
    Table atable;
    avl_sample->PrintTree();

    // DESTRUCTOR
    for(int i = 0; i < 1; ++i) {
        Table table;
        table.id = 3;
        table.result = table_result[0];
        cout << "Delete: "; table.Print(); cout << endl;
        avl_sample->DeleteTable(table);
    }
    avl_sample->PrintTree();
    avl_sample->~AVLTree();
}
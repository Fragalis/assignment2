#include "avl.cpp"

int main() {
    srand(time(NULL));
    const int SIZE = 16;
    AVLTree *avl_sample = new AVLTree();
    int table_result[SIZE];
    for(int i = 0; i < SIZE; ++i) table_result[i] = rand()%32768 + i;

    // CONSTRUCTOR
    for(int i = 0; i < SIZE; ++i) {
        Table table;
        table.id = rand()%MAXSIZE + 1;
        table.result = table_result[i];
        avl_sample->Insert(table);
    }
    avl_sample->PrintTree(); cout << endl;


    // DESTRUCTOR
    for(int i = 0; i < SIZE; ++i) {
        Table table;
        table.id = rand()%MAXSIZE + 1;
        table.result = table_result[i];
        avl_sample->Delete(table);
    }
    avl_sample->PrintTree();
    avl_sample->~AVLTree();
}
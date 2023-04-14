#include "avl.cpp"

int main() {
    srand(time(NULL));
    const int SIZE = 16;
    AVLTree *avl_sample = new AVLTree();
    int table_result[SIZE];
    for(int i = 0; i < SIZE; ++i) table_result[i] = 1<<i;
    for(int i = 0; i < SIZE; ++i) {
        Table table;
        table.id = rand()%MAXSIZE + 1;
        table.result = table_result[i];
        avl_sample->Insert(table);
    }
    printBT(avl_sample); cout << endl;


    for(int i = 0; i < SIZE>>2; ++i) {
        Table table;
        table.id = rand()%MAXSIZE + 1;
        table.result = table_result[rand()%SIZE];
        cout << "Delete table result: " << table.result << endl;
        avl_sample->Delete(table);
        printBT(avl_sample); cout << endl;
    }
    avl_sample->~AVLTree();
}
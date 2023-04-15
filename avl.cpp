#include "table.cpp"

class AVLNode {
public:
    Table table;
    AVLNode *left, *right;

    AVLNode(Table table) {
        this->table.SetTable(table);
        this->left = NULL;
        this->right = NULL;
    }
    ~AVLNode() {
        this->table.ResetTable();
        this->left = NULL;
        this->right = NULL;
        // cout << "NODE DESTRUCTOR" << endl;
    }
};

class AVLTree {
private:
    const int capacity = MAXSIZE>>1;
    AVLNode *root;
    int size;
    
    // UNBALANCE RIGHT TREE -> LEFT ROTATION
    AVLNode* _rotateLeft(AVLNode *node) {
        // cout << "ROTATE LEFT" << endl;

        AVLNode *rightChild = node->right;
        AVLNode *temp = rightChild->left;
        rightChild->left = node;
        node->right = temp;
        return rightChild;
    }
    
    // UNBALANCE RIGHT TREE -> LEFT ROTATION
    AVLNode* _rotateRight(AVLNode *node) { 
        // cout << "ROTATE RIGHT" << endl;        
        
        AVLNode *leftChild = node->left;
        AVLNode *temp = leftChild->right;
        leftChild->right = node;
        node->left = temp;
        return leftChild;
    }

    // INSERT Node
    AVLNode* _insertNode(AVLNode *root, Table table) {
        if(!root) {
            AVLNode *node = new AVLNode(table);
            return node;
        }
        if(table.result < root->table.result) root->left = _insertNode(root->left, table);
        if(table.result >= root->table.result) root->right = _insertNode(root->right, table);

        int balance = getBalance(root);
        
        // RIGHT ORIENTED -> ROTATE LEFT
        // cout << "BALANCE = " << balance << endl;
        if(balance > 1) {
            if(table.result < root->right->table.result) { // When too much right node
                root->right = _rotateRight(root->right);
            }
            return _rotateLeft(root);
        }

        // LEFT ORIENTED -> ROTATE RIGHT
        if(balance < -1) {
            if(table.result > root->left->table.result) { // When too much left node
                root->left = _rotateLeft(root->left);
            }
            return _rotateRight(root);   
        }
        return root;
    }

    // DELETE node which matchs Table table
    AVLNode* _deleteNode(AVLNode *root, Table table) {
        if(!root) return root;

        // Traverse left tree if result < root->result
        if(table.result < root->table.result || table.id != root->table.id) root->left = _deleteNode(root->left, table);

        // Traverse right tree if result > root->result
        if(table.result > root->table.result || table.id != root->table.id) root->right = _deleteNode(root->right, table);

        // Found Node to delete
        else {
            if(!root->left || !root->right) { // ROOT HAS AT LEAST 1 CHILD
                AVLNode *temp = (root->left)? root->left : root->right;

                if(temp == NULL) { // ROOT is LEAF
                    temp = root;
                    root = NULL;
                }
                else root->table.SetTable(temp->table);

                delete temp;
                if(root) { // RESET POINTER
                    root->left = NULL;
                    root->right = NULL;
                }
            }
            else { // ROOT HAS 2 CHILDREN
                Table newTable = getMaxNode(root->left);
                root->table.SetTable(newTable);
                root->left = _deleteNode(root->left, newTable);
            }
        }
        // Check if the noot deleted is a leaf -> refer to ROOT HAS AT LEAST 1 CHILD
        if(!root) return root;
        int balance = getBalance(root);
        // RIGHT ORIENTED -> ROTATE LEFT
        if(balance > 1) {
            if(getBalance(root->right) < -1) { // When too much left node on right child
                root->right = _rotateRight(root->right);
            }
            return _rotateLeft(root);
        }

        // LEFT ORIENTED -> ROTATE RIGHT
        if(balance < -1) {
            if(getBalance(root->left) > 1) { // When too much right node on left child
                root->left = _rotateLeft(root->left);
            }
            return _rotateRight(root);   
        }
        return root;
    }

    void _clear(AVLNode *node) {
        if(!node) return;
        if(node->left) _clear(node->left);
        if(node->right) _clear(node->right);
        delete node;
        node = NULL;
    }

    string _findName(AVLNode *node, int &id, int &result) {
        if(!node) return "";
        Table curr = node->table;
        if(curr.Equal(id, result)) return curr.name;
        if(curr.result > result || curr.id != id) return _findName(node->right, id, result);
        if(curr.result > result || curr.id != id) return _findName(node->left, id, result);
        return "";
    }

    void _printTree(const std::string& prefix, AVLNode* node, bool isLeft)
    {
        if(size == 0) cout << "NO ELEMENT" << endl;
        if (node != nullptr)
        {
            cout << prefix;
            cout << (isLeft ? "|------ (RIGHT:) " : "L------ (LEFT:) ");
            // print the value of the node
            node->table.Print(); cout << endl;
            // enter the next tree level - left and right branch
            _printTree(prefix + (isLeft ? "|       " : "        "), node->right, true);
            _printTree(prefix + (isLeft ? "|       " : "        "), node->left, false);
        }
    }

protected:
    int getHeightRecord(AVLNode *node) {
        if(!node) return 0;
        int leftHeight = (node->left)? this->getHeightRecord(node->left) : 0;
        int rightHeight = (node->right)? this->getHeightRecord(node->right) : 0;
        return ((leftHeight > rightHeight)? leftHeight : rightHeight) + 1;
    }
    
    Table getMaxNode(AVLNode *root) {
        while(root->right) root = root->right;
        return root->table;
    }

    int getBalance(AVLNode *root) {
        if(!root) return 0;
        return getHeightRecord(root->right) - getHeightRecord(root->left);
    }
public:

    AVLTree() {
        this->root = NULL;
        size = 0;
    }
    ~AVLTree() {
        Clear();
        // cout << "DESTRUCTOR" << endl;
    }
    
    // GET HEIGHT
    int GetHeight() {
        return getHeightRecord(root);
    }

    bool IsFull() {
        return (this->size >= capacity);
    }

    void InsertTable(Table table) {
        if(size < capacity) {
            root = _insertNode(root, table);
            ++size;
        }
    }

    void DeleteTable(Table table) {
        if(size > 0) {
            root = _deleteNode(root, table);
            --size;
        }
    }

    string FindName(int id, int result) {
        return _findName(this->root, id, result);
    }

    void Clear() {
        _clear(root);
        root = NULL;
        size = 0;
    }

    // For testing purposes
    void PrintTree() {
        _printTree("", root, false);
        cout << endl;
    }

    void Print_Command() {
        queue<AVLNode*> q;
        q.push(root);

        while(!q.empty()) {
            AVLNode *curr = q.front();
            q.pop();

            if(curr->left) q.push(curr->left);
            if(curr->right) q.push(curr->right);

            cout << curr->table.id << "-" << curr->table.result << "\n";
        }
    }
};
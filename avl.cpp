#include "table.cpp"

class AVLNode {
public:
    Table table;
    AVLNode *left, *right;

    AVLNode(Table table) {
        this->table = table;
        this->left = NULL;
        this->right = NULL;
    }
    ~AVLNode() {
        this->left = NULL;
        this->right = NULL;
        // cout << "NODE DESTRUCTOR" << endl;
    }
};

class AVLTree {
private:
    AVLNode* _rotateLeft(AVLNode *node) { // UNBALANCE RIGHT TREE -> LEFT ROTATION
        // cout << "ROTATE LEFT" << endl;

        AVLNode *rightChild = node->right;
        AVLNode *temp = rightChild->left;
        rightChild->left = node;
        node->right = temp;
        return rightChild;
    }
    
    AVLNode* _rotateRight(AVLNode *node) { // UNBALANCE RIGHT TREE -> LEFT ROTATION
        // cout << "ROTATE RIGHT" << endl;        
        
        AVLNode *leftChild = node->left;
        AVLNode *temp = leftChild->right;
        leftChild->right = node;
        node->left = temp;
        return leftChild;
    }

    AVLNode* _insertNode(AVLNode *root, Table table) { // INSERT NEW NODE
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

    AVLNode* _deleteNode(AVLNode *root, Table table) { // DELETE A NODE WHICH IS table TABLE
        if(!root) return root;

        // Traverse left tree if result < root->result
        if(table.result < root->table.result) root->left = _deleteNode(root->left, table);

        // Traverse right tree if result > root->result
        else if(table.result > root->table.result) root->right = _deleteNode(root->right, table);

        // Found Node to delete
        else {
            if(!root->left || !root->right) { // ROOT HAS AT LEAST 1 CHILD
                AVLNode *temp = (root->left)? root->left : root->right;

                if(temp == NULL) { // ROOT is LEAF
                    temp = root;
                    root = NULL;
                }
                else {
                    root->table.id = temp->table.id;
                    root->table.result = temp->table.result;
                }

                delete temp;

                if(root) { // RESET POINTER
                    root->left = NULL;
                    root->right = NULL;
                }
            }
            else { // ROOT HAS 2 CHILDREN
                Table newTable = getMaxNode(root->left);
                root->table.id = newTable.id;
                root->table.result = newTable.result;
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
        node->left = NULL;
        node->right = NULL;
    }

    void _printTree(const std::string& prefix, const AVLNode* node, bool isLeft)
    {
        if (node != nullptr)
        {
            std::cout << prefix;
            std::cout << (isLeft ? "|------ (RIGHT:) " : "L------ (LEFT:) ");
            // print the value of the node
            std::cout << node->table.result << std::endl;
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
    AVLNode *root;
    int size = 0;

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
        return size >= MAXSIZE;
    }

    void Insert(Table table) {
        if(size < MAXSIZE) {
            root = _insertNode(root, table);
            ++size;
        }
    }

    void Delete(Table table) {
        if(size > 0) {
            root = _deleteNode(root, table);
            --size;
        }
    }

    void Clear() {
        _clear(root);
        root = NULL;
        size = 0;
    }

    void PrintTree() {
        _printTree("", root, false);
    }
};
#include "table.cpp"

enum BalanceValue
{
    LH = -1,
    EH = 0,
    RH = 1
};

void printNSpace(int n)
{
    for (int i = 0; i < n - 1; i++)
        cout << " ";
}

void printInteger(int &n)
{
    cout << n << " ";
}

class AVLNode {
public:
    Table table;
    AVLNode *left, *right;
    BalanceValue balanceValue;
    AVLNode(Table table) {
        this->table = table;
        this->left = NULL;
        this->right = NULL;
        balanceValue = EH;
    }
    ~AVLNode(){}

    void SetLeft(AVLNode* left) {
        this->left = left;
    }
    void SetRight(AVLNode* right) {
        this->right = right;
    }
};

class AVLTree {
private:
    AVLNode* RotateLeft(AVLNode *node) { // UNBALANCE RIGHT TREE -> LEFT ROTATION
        AVLNode *rightChild = node->right;
        node->right = rightChild->left;
        rightChild->left = node->right;
        return rightChild;
    }
    
    AVLNode* RotateRight(AVLNode *node) { // UNBALANCE RIGHT TREE -> LEFT ROTATION
        AVLNode *leftChild = node->left;
        node->left = leftChild->right;
        leftChild->right = node->left;
        return leftChild;
    }

    AVLNode* InsertNode(AVLNode *root, Table table) { // INSERT NEW NODE
        if(!root) {
            AVLNode *node = new AVLNode(table);
            return node;
        }
        if(table.result < root->table.result) root->SetLeft(insert(root->left, table));
        if(table.result >= root->table.result) root->SetRight(insert(root->right, table));

        int balance = GetBalance(root);
        
        // RIGHT ORIENTED -> ROTATE LEFT
        if(balance > 1) {
            if(table.result < root->right->table.result) { // When too much right node
                root->right = RotateRight(root->right);
            }
            root->balanceValue = EH;
            return RotateLeft(root);
        }

        // LEFT ORIENTED -> ROTATE RIGHT
        if(balance < -1) {
            if(table.result > root->left->table.result) { // When too much left node
                root->left = RotateLeft(root->left);
            }
            root->balanceValue = EH;
            return RotateRight(root);   
        }
        return root;
    }


protected:
    int GetHeightRecord(AVLNode *node) {
        if(!node) return 0;
        int leftHeight = this->GetHeightRecord(node->left);
        int rightHeight = this->GetHeightRecord(node->right);
        return ((leftHeight > rightHeight)? leftHeight : rightHeight) + 1;
    }

    int GetBalance(AVLNode *root) {
        return GetHeightRecord(root->right) - GetHeightRecord(root->left);
    }
public:
    AVLNode *root;
    AVLTree() {
        this->root = NULL;
    }
    ~AVLTree(){}
    
    // GET HEIGHT
    int GetHeight() {
        return GetHeightRecord(root);
    }

    // PRINT TREE
    void printTreeStructure()
    {
        int height = this->GetHeight();
        if (this->root == NULL)
        {
            cout << "TREE EMPTY\n";
            return;
        }
        queue<AVLNode*> q;
        q.push(root);
        AVLNode *temp;
        int count = 0;
        int maxNode = 1;
        int level = 0;
        int space = 1<<height;
        printNSpace(space / 2);
        while (!q.empty())
        {
            temp = q.front();
            q.pop();
            if (temp == NULL)
            {
                cout << " ";
                q.push(NULL);
                q.push(NULL);
            }
            else
            {
                cout << "ID: " << temp->table.id << " Result: " << temp->table.result << "\n";
                q.push(temp->left);
                q.push(temp->right);
            }
            printNSpace(space);
            count++;
            if (count == maxNode)
            {
                cout << endl;
                count = 0;
                maxNode *= 2;
                level++;
                space /= 2;
                printNSpace(space / 2);
            }
            if (level == height)
                return;
        }
    }
    
    void Insert(Table table) {
        root = InsertNode(root, table);
    }
}
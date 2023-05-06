#include "main.h"

/* HUFFMAN ENCODING SECTION */
class HuffNode {
private:
    char _value;
    int _weight;
    HuffNode *_hn_left;
    HuffNode *_hn_right;

public:
    HuffNode(char value = 0, int weight = 0) {
        this->_value = value;
        this->_weight = weight;
        this->_hn_left = NULL;
        this->_hn_right = NULL;
    }

    ~HuffNode() {
        this->_value = 0;
        this->_weight = 0;
        _hn_left = 0;
        _hn_right = 0;
    }

    char GetValue() {
        return this->_value;
    }

    int GetWeight() {
        return this->_weight;
    }

    HuffNode *GetLeftPtr() const {
        return this->_hn_left;
    }

    void SetPtrLeft(HuffNode *hn_left) {
        this->_hn_left = hn_left;
    }

    HuffNode *GetRightPtr() const {
        return this->_hn_right;
    }

    void SetPtrRight(HuffNode *hn_right) {
        this->_hn_right = hn_right;
    }

    bool isLeaf() {
        return (this->_hn_left == 0 && this->_hn_right == 0);
    }

    void Encode(HuffNode *ptr, unordered_map<char,string> &code, string char_str) {
        //cout << number << endl;
        if(ptr->GetLeftPtr()) Encode(ptr->GetLeftPtr(), code, char_str + "0");

        if(ptr->GetRightPtr()) Encode(ptr->GetRightPtr(), code, char_str + "1");

        if(ptr->isLeaf()) code[ptr->GetValue()] = char_str;
    }
};

void add(list<HuffNode*> &listNode, HuffNode* node) {
    if(listNode.empty()) {
        listNode.push_back(node);
        return;
    }
    for(auto it = listNode.begin(); it != listNode.end(); ++it) {
        if((*it)->GetWeight() > node->GetWeight()) {
            listNode.insert(it, node);
            return;
        }
    }
    listNode.push_back(node);
}

class Compare {
public:
    bool operator()(HuffNode *a, HuffNode *b) {
        return (a->GetWeight() < b->GetWeight());
    }
};


void Delete(HuffNode *ptr) {
    if(!ptr) return;
    if(ptr->GetLeftPtr()) Delete(ptr->GetLeftPtr());
    if(ptr->GetRightPtr()) Delete(ptr->GetRightPtr());
    delete ptr;
    ptr = NULL; 
}
 
HuffNode *GenerateHuffTree(list<HuffNode*> listNode) {
    while(listNode.size() > 1) {
        // Get Node with lowest Frequency, initalize it to left
        HuffNode *hn_left = listNode.front();
        listNode.pop_front();

        // Get Node with lowest Frequency, initalize it to right
        HuffNode *hn_right = listNode.front();
        listNode.pop_front();

        // CREATE NEW NODE:
        HuffNode *newNode = new HuffNode('.', hn_left->GetWeight() + hn_right->GetWeight());
        newNode->SetPtrLeft(hn_left);
        newNode->SetPtrRight(hn_right);

        // Push it back to heap:
        add(listNode, newNode);
    }
    return listNode.front();
}

int HuffmanEncoding(string name) {
    // MAXIMUM BITS - refer to Assignment 2 Constraints
    const int MAX_BITS = 15;

    // An ARRAY to store FREQUENCY (A-Z) - (a-z) in "name"
    unordered_map<char, int> frequency;
    for(char c : name) frequency[c]++;
    if(frequency.size() == 1) return (1 << min(frequency[name[0]], MAX_BITS)) - 1;

    // INITIALIZE MIN-HEAP FOR HUFFMAN NODE AS LIST
    list<HuffNode*> listNode;
    for(char c = 'A'; c <= 'Z'; ++c) {
        if(!frequency[c]) continue;
        HuffNode *newNode = new HuffNode(c, frequency[c]);
        add(listNode, newNode);
    }
    for(char c = 'a'; c <= 'z'; ++c) {
        if(!frequency[c]) continue;
        HuffNode *newNode = new HuffNode(c, frequency[c]);
        add(listNode, newNode);
    }

    // Generate Node
    HuffNode *root = GenerateHuffTree(listNode);

    // Huffman convert
    unordered_map<char,string> code;
    root->Encode(root, code, "");

    // We don't need this anymore
    Delete(root);

    // String variant
    string huffmanCode_str = "";

    // Traverse every char in "name" && convert
    for(int i = (int)name.length() - 1; i >= 0; --i) {
        char c = name[i];
        huffmanCode_str = code[c] + huffmanCode_str;
        
        // IF EXCEED: break;
        if(huffmanCode_str.length() >= MAX_BITS) break;
    }

    // Decimal variant
    int bits = huffmanCode_str.length();
    int huffmanCode = 0;
    if(bits <= MAX_BITS) {
        for(int i = 0; i < bits; ++i) huffmanCode = (huffmanCode << 1) + (huffmanCode_str[i] - '0');
        return huffmanCode;
    }

    for(int i = bits - MAX_BITS; i < bits; ++i) huffmanCode = (huffmanCode << 1) + (huffmanCode_str[i] - '0');
    return huffmanCode;
}
/* END HUFFMAN ENCODING SECTION */

/* HASHTABLE SECTION */
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
};

class HashTable {
private:
    const int capacity = MAXSIZE>>1;
    Table key[MAXSIZE>>1];
    int size;

    // ASSUME HASHTABLE IS NEVER FULL
    int _findAddress(int address) { // This is called when Linear Probing is needed.
        int index = address;

        // Traverse the HASHMAP for empty table
        while(key[index%capacity].IsEmpty() == false) ++index;
        return (index%capacity);
    }

    void _clear() {
        for(int i = 0; i < capacity; ++i) key[i].ResetTable();
    }
public:
    HashTable() {
        this->size = 0;
        for(int i = 0; i < capacity; ++i) key[i].ResetTable();
    }

    ~HashTable() {
        Clear();
    }

    int GetSize() {
        return this->size;
    }

    int GetCapacity() {
        return this->capacity;
    }

    Table GetTable(int address) {
        return key[address];
    }

    bool IsFull() {
        return (this->size >= capacity);
    }

    void AddTable(Table table, int address) {
        if(IsFull() || address < 0 || address >= capacity) return;
        if(key[address].IsEmpty() == false) address = _findAddress(address);
        key[address].SetTable(table);
        ++size;
    }

    // Remove table which has the id (id)
    void RemoveTable(int id) {
        for(int i = 0; i < capacity; ++i) {
            if(id == key[i].id) {
                key[i].ResetTable();
                return;
            }
        }
    }

    bool FindTable(int id, int result, string name) {
        for(int i = 0; i < capacity; ++i) {
            if(key[i].Equal(id, result, name)) return true;
        }
        return false;
    }

    void Clear() {
        _clear();
        this->size = 0;
    }
};
/* END HASHTABLE SECTION */

/* AVL TREE SECTION */
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
                Table newTable = getMinNode(root->right);
                root->table.SetTable(newTable);
                root->right = _deleteNode(root->right, newTable);
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

    bool _search(AVLNode *node, int &id, int &result, string &name) {
        if(!node) return false;
        Table curr = node->table;
        if(curr.Equal(id, result, name)) return true;
        if(curr.result > result) return _search(node->left, id, result, name);
        else if(curr.result < result) return _search(node->right, id, result, name);
        else {
            if(curr.id != id)
                return _search(node->left, id, result, name) || _search(node->right, id, result, name);
        }
        return false;
    }

protected:
    int getHeightRecord(AVLNode *node) {
        if(!node) return 0;
        int leftHeight = (node->left)? this->getHeightRecord(node->left) : 0;
        int rightHeight = (node->right)? this->getHeightRecord(node->right) : 0;
        return ((leftHeight > rightHeight)? leftHeight : rightHeight) + 1;
    }
    
    Table getMinNode(AVLNode *root) {
        while(root->left) root = root->left;
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

    AVLNode* GetRoot() {
        return this->root;
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

    bool FindTable(int id, int result, string name) {
        return _search(this->root, id, result, name);
    }

    void Clear() {
        _clear(root);
        root = NULL;
        size = 0;
    }
};
/* END AVL TREE SECTION */

/* MIN HEAP SECTION */
struct order {
    int id;
    int count;
    void reset() {
        id = 0;
        count = 0;
    }
};

class MinHeap {
private:
    int capacity = MAXSIZE;
    order orders[MAXSIZE];
    int size;
    vector<int> time;
    // Always call when a new customer is assigned
    void _push(int id, int count) {
        // Heap is full
        if(IsFull()) return;
        
        // Else
        orders[size].id = id;
        orders[size].count = count;
        _reheapUp(size);
        ++size;
    }

    void _pop(int id) {
        // Heap is empty
        if(size <= 0) return;
    
        // Else
        int idx = 0;
        while(orders[idx].id != id) ++idx;
        orders[idx] = orders[size - 1];
        orders[size - 1].reset();
        --size;
        _reheapDown(idx);
    }

    void _reheapUp(int position) {
        if(position > 0) {
            int parent = (position - 1) / 2;
            if(orders[position].count < orders[parent].count) {
                swap(orders[position], orders[parent]);
                _reheapUp(parent);
            }
            else if(orders[position].count == orders[parent].count) {
                int step = find(time.begin(), time.end(), orders[position].id) - 
                            find(time.begin(), time.end(), orders[parent].id);
                if(step < 0) {
                    swap(orders[position], orders[parent]);
                    _reheapUp(parent);
                }
            }
        }
    }

    void _reheapDown(int position) {
        int left = 2 * position + 1;
        int right = 2 * position + 2;
        int smaller = -1;
        if(left < size) {
            if(right < size) {
                // IF right.count < left.count
                if(orders[right].count < orders[left].count) smaller = right;
                // IF right.count == left.count
                else if(orders[right].count == orders[left].count) {
                    int step = find(time.begin(), time.end(), orders[right].id) - 
                               find(time.begin(), time.end(), orders[left].id);
                    if(step < 0) smaller = right;
                    else smaller = left;
                }
                else smaller = left;
            }
            else smaller = left;
            if(orders[position].count > orders[smaller].count) {
                swap(orders[position], orders[smaller]);
                _reheapDown(smaller);
            }
            else if(orders[position].count == orders[smaller].count) {
                int step = find(time.begin(), time.end(), orders[position].id) - 
                            find(time.begin(), time.end(), orders[smaller].id);
                if(step > 0) {
                    swap(orders[position], orders[smaller]);
                    _reheapDown(smaller);
                }
            }
        }
    }

    int _getOrderCount(int id) {
        for(int i = 0; i < size; ++i) {
            if(orders[i].id == id) return orders[i].count;
        }
        return 0;
    }

    void _print(int position) {
        if(position >= size) return;
        // Print data at position
        cout << orders[position].id << "-" << orders[position].count << "\n";
        // Traverse to left
        _print(2 * position + 1);
        // Traverse to right
        _print(2 * position + 2);
    }
public:
    MinHeap() {
        size = 0;
        for(int i = 0; i < MAXSIZE; ++i) {
            orders[i].count = 0;
            orders[i].id = 0;
        }
    }

    ~MinHeap() {}

    bool IsFull() {
        return (size >= capacity);
    }

    // If the id is new, we add order{id, 1} to Heap
    // Else, we increment the order count by 1
    void Push(int id) {
        // We traverse the heap for id
        for(int i = 0; i < size; ++i) {
            // If we found the id
            if(orders[i].id == id) {
                // Increment the count, then reheapDown at this position and return
                int order_id = orders[i].id;
                int order_count = orders[i].count + 1;
                _pop(order_id);
                _push(order_id, order_count);
                return;
            }
        }
        // Here if the id is new <- we can't find it in that loop before
        time.push_back(id);
        _push(id, 1);
    }

    // This id will always be available due to this being called when removal is needed
    void Pop(int id) {
        time.erase(find(time.begin(), time.end(), id));
        _pop(id);
    }

    // Get the id of customer who orders the least (always be orders[0])
    int Front() {
        if(size <= 0) return -1;
        return orders[0].id;
    }

    int GetOrder(int id) {
        return _getOrderCount(id);
    }

    void Print_Command() {
        _print(0);
    }
};
/* END MIN HEAP SECTION*/

// Always call when there is a REG
void AddCustomer(const Table &customer, vector<int> &recordResult, vector<bool> &isHash,
				 HashTable &Location_HashTable,
				 AVLTree *&Location_AVL);

// Call when removal is needed
void RemoveCustomer(const Table &customer, vector<int> &recordResult, vector<bool> &isHash,
				 HashTable &Location_HashTable, 
				 AVLTree *&Location_AVL);

void PrintHT(HashTable &Location_HashTable, MinHeap &LFCO_Heap);
void PrintAVL(AVLTree *&Location_AVL, MinHeap &LFCO_Heap);
void PrintMH(MinHeap &LFCO_Heap);

void simulate(string filename) {
	ifstream input(filename);
	// CHECK FOR VALID FILE INPUT
	if(!input) return;
	
	// A VECTOR of Result formed by Huffman Encoding
	// in which 0 is always untouched (-1)
	vector<int> RecordResult(MAXSIZE + 1, -1);

	// A VECTOR of bool which locate the table id
	// in which 0 is always untouched
	// Set to true if table in HashTable Area
	// Set to false if table in AVL Area
	vector<bool> IsHash(MAXSIZE + 1, true);

	// LOCATION 1: Hash-Table
	HashTable Location_HashTable;

	// LOCATION 2: AVL Tree
	AVLTree *Location_AVL = new AVLTree();

	// A Queue to store customer id, refer to OPT = 1
	// First customer in is always in .front()
	list<int> FIFO_Queue;

	// A List to store customer id, refer to OPT = 2
	// Last customer to order is always in .front()
	list<int> LRCO_List;

	// A Min-Heap to store customer id and frequency, refer to OPT = 3
    // First customer to order the least is always on top
	MinHeap LFCO_Heap;

	// INITIAL COMMAND LINE INPUT
	string line = "";
	while(getline(input, line)) {
		int index = 0;
		string keyword = "";

		// READ KEYWORD
		for(int i = 0; i < (int)line.length() and line[i] != ' '; ++i) keyword += line[i];
		
		// PUSH INDEX TO NEXT ELEMENT
		index += ((int)keyword.length() + 1);

		if(keyword == "REG") {
			bool isFull = Location_AVL->IsFull() && Location_HashTable.IsFull();
			bool isOrder = false;
			string name = "";
			// READ NAME
			for(int i = index; i < (int)line.length() and line[i] != ' '; ++i) {
				char c = line[i];
				// If character is valid (A-Z)(a-z)
				if(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) name += c;
				
				// If not valid -> break;
				else break;
			}
            index += ((int)name.length());

			// INVALID NAME (No space between, no space at end, no non-alphabet character) -> to next line
			if(index < (int)line.length()) continue;
			int customerResult = HuffmanEncoding(name);
			int customerID = customerResult % MAXSIZE + 1;

			// If Result Match, we need to check if name matches
			for(int id = 1; id <= MAXSIZE; ++id) {

				// If result matches, there's a chance that the name matches
				if(RecordResult[id] == customerResult) {
					// If the name matches
					int result = customerResult;
					if(IsHash[id]) {
						if(Location_HashTable.FindTable(id, result, name)) isOrder = true;
					}
					else {
						if(Location_AVL->FindTable(id, result, name)) isOrder = true;
					}
				}
				// If it's an order
				if(isOrder) {
					// Reset customerID to id
					customerID = (id - 1) % MAXSIZE + 1;
					break;
				}
			}
			// If it's not an order
			// We traverse the table vector to find empty table
			if(!isOrder) {
				
				// If both the locations are full
				if(isFull) {
					// Calculating OPT value - Refer to Assignment 2 Spec
					int OPT = customerResult%3;
					// cout << "OPT: " << OPT << endl;
					// Create Table for removal
					Table removal;
					int removalID = -1;
					// If OPT = 0 -> FIFO (a queue is enough)
					if(OPT == 0) {
						removalID = FIFO_Queue.front();
						FIFO_Queue.pop_front();
					}
					
					// If OPT = 1 -> LRCO (a list should be enough)
					if(OPT == 1) {
						removalID = LRCO_List.front();
						LRCO_List.pop_front();
					}

					// If OPT = 2 -> LFCO (a heap will be enough)
					if(OPT == 2) {
						removalID = LFCO_Heap.Front();
						LFCO_Heap.Pop(removalID);
					}
					// Call RemoveCustomer(param) method
					removal.SetTable(removalID, RecordResult[removalID], "");
					RemoveCustomer(removal, RecordResult, IsHash, Location_HashTable, Location_AVL);
				}
				
				// REMOVAL SUCCESS

				while((customerID <= 2 * MAXSIZE) && RecordResult[(customerID - 1)%MAXSIZE + 1] != -1) {
					++customerID;
				}
				customerID = (customerID - 1)%MAXSIZE + 1;

				// Create new table representing customer
				Table customer;
				customer.SetTable(customerID, customerResult, name);

				// Call AddCustomer(param) methods
				AddCustomer(customer, RecordResult, IsHash, Location_HashTable, Location_AVL);

                // Push to queues
				FIFO_Queue.push_back(customerID);
				LRCO_List.push_back(customerID);
				LFCO_Heap.Push(customerID);
			}

			// If it's an order -> update (OPT = 1) and Min-Heap (OPT = 2);
			else {
				// If the customer order -> push that customerID to back
				LRCO_List.remove(customerID);
				LRCO_List.push_back(customerID);
				LFCO_Heap.Push(customerID);
			}
		}

		else if(keyword == "CLE") {
			bool isNegative = false;
			string temp = "";
			
			// Check if that number is negative:
			if(line[index] == '-') {
				isNegative = true;
				++index;
			}

			for(int i = index; i < (int)line.length() && line[i] != ' '; ++i) {
				char c = line[i];
				// If character is valid (0-9)
				if('0' <= c && c <= '9') temp += c;
				// If not valid -> break;
				else break;
			}
            index += temp.length();
			// INVALID NUMBER (No space between, no space at end, no non-numeric character) -> to next line
			if(index < (int)line.length()) continue;
			int number = stoi(temp) * (isNegative? -1 : 1); // If negative -> multiply it with -1

			if(number < 1) { // Delete all Hash-Table content
				for(auto it = FIFO_Queue.begin(); it != FIFO_Queue.end(); ++it) {
					if(IsHash[*it]) {
						int id = *it;
						RecordResult[id] = -1;
						LRCO_List.remove(id);
						LFCO_Heap.Pop(id);
						auto newIt = --it;
						FIFO_Queue.remove(id);
						it = newIt;
					}
				}
				Location_HashTable.Clear();
			}

			else if(number > MAXSIZE) { // Delete all AVL content
				for(auto it = FIFO_Queue.begin(); it != FIFO_Queue.end(); ++it) {
					if(!IsHash[*it]) {
						cout << *it << endl;
						int id = *it;
						RecordResult[id] = -1;
						LRCO_List.remove(id);
						LFCO_Heap.Pop(id);
						auto newIt = --it;
						FIFO_Queue.remove(id);
						it = newIt;
					}
				}
				Location_AVL->Clear();
			}
			else { // Delete Specific Table
				if(RecordResult[number] == -1) continue;
				Table customer;
				customer.SetTable(number, RecordResult[number], "");
				RemoveCustomer(customer, RecordResult, IsHash, Location_HashTable, Location_AVL);
				FIFO_Queue.remove(customer.id);
				LFCO_Heap.Pop(customer.id);
				LRCO_List.remove(customer.id);
			}
		}

		else if(keyword == "PrintHT") {
			PrintHT(Location_HashTable, LFCO_Heap);
		}

		else if(keyword == "PrintAVL") {
			PrintAVL(Location_AVL, LFCO_Heap);
		}
		else if(keyword == "PrintMH") {
			PrintMH(LFCO_Heap);
		}

		// INVALID KEYWORD
		else {
			continue;
		}
	}
	delete Location_AVL;
}

void AddCustomer(const Table &customer, vector<int> &recordResult, vector<bool> &isHash,
				 HashTable &Location_HashTable, 
				 AVLTree *&Location_AVL) {

	// Take elements from customer
	int customerResult = customer.result;
	int customerID = customer.id;

	// Record customerResult
	recordResult[customerID] = customerResult;

	// IF Customer is located in AVL || Hash-Table Area is Full.
	if(customerResult%2 == 0 || Location_HashTable.IsFull()) {
		Location_AVL->InsertTable(customer);
		isHash[customerID] = false;
	}

	// IF Customer is located in Hash-Table || AVL Area is Full
	else if(customerResult%2 == 1 || Location_AVL->IsFull()) {
		Location_HashTable.AddTable(customer, customerResult%(MAXSIZE>>1));
		isHash[customerID] = true;
	}
}

void RemoveCustomer(const Table &customer, vector<int> &recordResult, vector<bool> &isHash,
				 HashTable &Location_HashTable, 
				 AVLTree *&Location_AVL) {
	
	// Take elements from customer
	int customerID = customer.id;

	// The table we delete is in HashTable Area
	if(isHash[customerID]) {
		Location_HashTable.RemoveTable(customerID);
	}
	// The table we delete is in AVL Area
	else {
		Location_AVL->DeleteTable(customer);
		isHash[customerID] = true;
	}
    recordResult[customerID] = -1;
}

void PrintHT(HashTable &Location_HashTable, MinHeap &LFCO_Heap) {
	for(int i = 0; i < Location_HashTable.GetCapacity(); ++i) {
		Table table = Location_HashTable.GetTable(i);
        if(table.IsEmpty()) continue; // If empty, continue
		int orderCount = LFCO_Heap.GetOrder(table.id); // NUM
		// ID-RESULT-NUM
		cout << table.id << "-" << table.result << "-" << orderCount << "\n";
    }
}

void PrintAVL(AVLTree *&Location_AVL, MinHeap &LFCO_Heap) {
	queue<AVLNode*> q;
	q.push(Location_AVL->GetRoot());

	while(!q.empty()) {
		AVLNode *curr = q.front();
		q.pop();
		if(curr->left) q.push(curr->left);
		if(curr->right) q.push(curr->right);
		// ID-RESULT-NUM
		int orderCount = LFCO_Heap.GetOrder(curr->table.id); // NUM
		cout << curr->table.id << "-" << curr->table.result << "-" << orderCount << "\n";
	}
}

void PrintMH(MinHeap &LFCO_Heap) {
	LFCO_Heap.Print_Command();
}
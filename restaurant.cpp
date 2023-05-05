#include "main.h"
#include "huffman_code.cpp"
#include "avl.cpp"
#include "minheap.cpp"

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
	ifstream input("test.txt");
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
	MinHeap LFCO_Heap;

	// INITIAL COMMAND LINE INPUT
	string line = "";
	while(getline(input, line)) {

		// Testing input
		// cout << line << endl;

		int index = 0;
		string keyword = "";

		// READ KEYWORD
		for(index; index < line.length() and line[index] != ' '; ++index) keyword += line[index];
		
		// PUSH INDEX TO NEXT ELEMENT
		++index;

		if(keyword == "REG") {
			bool isFull = Location_AVL->IsFull() && Location_HashTable.IsFull();
			bool isOrder = false;
			string name = "";
			// READ NAME
			for(index; index < line.length() and line[index] != ' '; ++index) {
				char c = line[index];
				// If character is valid (A-Z)(a-z)
				if(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) name += c;
				
				// If not valid -> break;
				else break;
			}

			// INVALID NAME (No space between, no space at end, no non-alphabet character) -> to next line
			if(index < line.length()) continue;
			// HuffmanEncoding(string name, bool caseSensitive)
			int customerResult = HuffmanEncoding(name);
			int customerID = customerResult%MAXSIZE + 1;
			/*
				If we were to order food
				The code must be here to check 
				if REG <NAME> is actually an order, not a register command
			 */

			// If Result Match, we need to check if name matches
			for(int id = customerID; id <= customerID + MAXSIZE; ++id) {

				// If result matches, there's a chance that the name matches
				if(RecordResult[(id - 1) % MAXSIZE + 1] == customerResult) {
					// If the name matches
					int result = customerResult;
					if(IsHash[id]) {
						if(Location_HashTable.FindName(id, result) == name) isOrder = true;
					}
					else {
						if(Location_AVL->FindName(id, result) == name) isOrder = true;
					}
				}
				
				// If it's an order
				if(isOrder) {
					// Reset customerID to id
					customerID = (id - 1) % MAXSIZE + 1;
					break;
				}
			}

			// EXECUTING REG COMMAND

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
					// Ignore the customer's arrival time for now
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

				// Add customer to FIFO queue
				FIFO_Queue.push_back(customerID);

				// Add customer to LRCO list
				LRCO_List.push_back(customerID);

				// Add customer to LFCO heap
				LFCO_Heap.Push(customerID);
			}

			// If it's an order -> update (OPT = 1) and Min-Heap (OPT = 2);
			else {
				// If the customer order -> push that customerID to back
				LRCO_List.remove(customerID);
				LRCO_List.push_back(customerID);

				// If the customer order -> use that Push(id) method
				LFCO_Heap.Push(customerID);
				LFCO_Heap.Print_Command();
			}
		}

		else if(keyword == "CLE") {
			bool isNegative = false;
			string temp = "";
			// READ NAME
			
			// Check if that number is negative:
			if(line[index] == '-') {
				isNegative = true;
				++index;
			}

			for(index; index < line.length() and line[index] != ' '; ++index) {
				char c = line[index];
				// If character is valid (0-9)
				if('0' <= c && c <= '9') temp += c;
				// If not valid -> break;
				else break;
			}

			// INVALID NUMBER (No space between, no space at end, no non-numeric character) -> to next line
			if(index < line.length()) continue;
			int number = stoi(temp) * (isNegative? -1 : 1); // If negative -> multiply it with -1

			if(number < 1) { // Delete all Hash-Table content
				for(auto it = FIFO_Queue.begin(); it != FIFO_Queue.end(); ++it) {
					// For every table which has id (i)
					// If table is in HashTable Area (IsHash[i] = true)
					// We reset all the query (FIFO) (LRCO) (LFCO) and RecordResult;
					if(IsHash[*it]) {
						// cout << *it << endl;
						int id = *it;
						RecordResult[id] = -1;
						LRCO_List.remove(id);
						LFCO_Heap.Pop(id);
						auto newIt = --it;
						FIFO_Queue.remove(id);
						it = newIt;
					}
				}

				// We resetted everything
				// Now we Clear the Location
				Location_HashTable.Clear();
			}

			else if(number > MAXSIZE) { // Delete all AVL content
				for(auto it = FIFO_Queue.begin(); it != FIFO_Queue.end(); ++it) {
					// For every table which has id (i)
					// If table is in AVL Area (IsHash[i] = false)
					// We reset all the query (FIFO) (LRCO) (LFCO) and RecordResult;
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

				// We resetted everything
				// Now we Clear the Location
				Location_AVL->Clear();
			}
			else { // Delete Specific Table
				// If table is empty -> continue;
				if(RecordResult[number] == -1) continue;

				// If not continue, call the RemoveCustomer(param) method
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
		// cout << "AVL TREE" << endl;
		// Location_AVL->PrintTree();
	}

	// IF Customer is located in Hash-Table || AVL Area is Full
	else if(customerResult%2 == 1 || Location_AVL->IsFull()) {
		Location_HashTable.AddTable(customer, customerResult%(MAXSIZE>>1));
		isHash[customerID] = true;
		// cout << "HASH TABLE" << endl;
		// Location_HashTable.PrintTable();
	}
}

void RemoveCustomer(const Table &customer, vector<int> &recordResult, vector<bool> &isHash,
				 HashTable &Location_HashTable, 
				 AVLTree *&Location_AVL) {
	
	// Take elements from customer
	int customerResult = customer.result;
	int customerID = customer.id;

	// The table we delete is in HashTable Area
	if(isHash[customerID]) {
		Location_HashTable.RemoveTable(customerID);
		recordResult[customerID] = -1;
	}
	// The table we delete is in AVL Area
	else {
		Location_AVL->DeleteTable(customer);
		recordResult[customerID] = -1;
		isHash[customerID] = true;
	}
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
#include "main.h"
#include "huffman_code.cpp"
#include "avl.cpp"

// Always call when there is a REG
void AddCustomer(const Table &customer, vector<int> &recordResult, vector<bool> &isHash,
				 HashTable &Location_HashTable,
				 AVLTree *&Location_AVL);

// Call when removal is needed
void RemoveCustomer(const Table &customer, vector<int> &recordResult, vector<bool> &isHash,
				 HashTable &Location_HashTable, 
				 AVLTree *&Location_AVL);

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
	queue<int> FIFO_Queue;

	// A List to store customer id, refer to OPT = 2
	// Last customer to order is always in .front()
	list<int> LRCO_List;

	// A Min-Heap to store customer id and frequency, refer to OPT = 3
	// TO BE IMPLEMENTED, REFER TO "min-heap.cpp"

	// INITIAL COMMAND LINE INPUT
	string line = "";
	while(getline(input, line)) {

		// Testing input
		cout << line << endl;

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
			int customerResult = HuffmanEncoding(name, 0);

			// If customerResult % MAXSIZE = 0 -> set ID to MAXSIZE
			int customerID = (customerResult % MAXSIZE)? customerResult % MAXSIZE : MAXSIZE;
			
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
					cout << "OPT: " << OPT << endl;
					// Create Table for removal
					Table removal;

					// If OPT = 0 -> FIFO (a queue is enough)
					if(OPT == 0) {
						int removalID = FIFO_Queue.front();
						removal.SetTable(removalID, RecordResult[removalID], "");
						FIFO_Queue.pop();
					}
					
					// If OPT = 1 -> LRCO (a list should be enough)
					if(OPT == 1) {
						int removalID = LRCO_List.front();
						removal.SetTable(removalID, RecordResult[removalID], "");
						LRCO_List.pop_front();
					}

					if(OPT == 2) continue;
					// Call RemoveCustomer(param) method
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
				FIFO_Queue.push(customerID);

				// Add customer to LRCO list
				LRCO_List.push_back(customerID);
			}

			// If it's an order -> update (OPT = 1) and Min-Heap (OPT = 2);
			else {
				// TO BE IMPLEMENTED
				// If the customer order -> push that customerID to back
				for(auto it = LRCO_List.begin(); it != LRCO_List.end(); ++it) {
					cout << *it << " ";
				} cout << endl;
				LRCO_List.remove(customerID);
				LRCO_List.push_back(customerID);
				for(auto it = LRCO_List.begin(); it != LRCO_List.end(); ++it) {
					cout << *it << " ";
				} cout << endl;
				// If the customer order -> HEAP
				cout << "IS ORDER" << endl;
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
				for(int i = 1; i <= MAXSIZE; ++i) {
					// For every table which has id (i)
					// If table is in HashTable Area (IsHash[i] = true)
					// We reset RecordResult[i];
					if(IsHash[i]) RecordResult[i] = -1;
				}

				// We resetted every table in RecordResult
				// Now we Clear the Location
				Location_HashTable.Clear();
			}

			else if(number > MAXSIZE) { // Delete all AVL content
				for(int i = 1; i <= MAXSIZE; ++i) {
					// For every table which has id (i)
					// If table is in AVL Area (IsHash[i] = false)
					// We reset RecordResult[i];
					if(!IsHash[i]) RecordResult[i] = -1;
				}

				// We resetted every table in RecordResult
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
			}
		}

		else if(keyword == "PrintHT") {
			Location_HashTable.Print_Command();
		}

		else if(keyword == "PrintAVL") {
			Location_AVL->Print_Command();
		}
		else if(keyword == "PrintMH") {
			// TO BE IMPLEMENTED
		}

		// INVALID KEYWORD
		else {
			continue;
		}
	}
	Location_AVL->~AVLTree();
	Location_HashTable.~HashTable();
	return;
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
		Location_HashTable.AddTable(customer, customerResult%3);
		isHash[customerID] = true;
		// cout << "HASH TABLE" << endl;
		// Location_HashTable.PrintTable();
	}
	return;
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
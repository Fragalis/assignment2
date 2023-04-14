#include "main.h"
#include "huffman_code.cpp"
#include "avl.cpp"

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

	// INITIAL COMMAND LINE INPUT
	string line = "";
	while(getline(input, line)) {
		cout << line << endl;
		int index = 0;
		string keyword = "";

		// READ KEYWORD
		for(index; index < line.length() and line[index] != ' '; ++index) keyword += line[index];
		
		// PUSH INDEX TO NEXT ELEMENT
		++index;

		if(keyword == "REG") {
			bool isFull = false;
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

			// name = "aabbcba";
			// cout << name << endl;

			// HuffmanEncoding(string name, bool caseSensitive)
			int customerResult = HuffmanEncoding(name, 0);

			customerResult = customerResult + rand()%2; // Generating equal odd-even prob
			// cout << customerResult << endl;

			// If customerResult % MAXSIZE = 0 -> set ID to MAXSIZE
			int customerID = (customerResult % MAXSIZE)? customerResult % MAXSIZE : MAXSIZE;

			// We traverse the table vector to find empty table
			while((customerID <= 2 * MAXSIZE) && RecordResult[(customerID - 1)%MAXSIZE + 1] != -1) {
				++customerID;
			}
			
			// If there's no empty table
			if(customerID > 2 * MAXSIZE) {
				isFull = true;
			}
			// Else there's at least 1 empty table
			else {
				customerID = (customerID - 1)%MAXSIZE + 1;
				RecordResult[customerID] = customerResult;
				// cout << customerID << endl;
			}

			// EXECUTING REG COMMAND
			
			// Remove a customer if isFull
			if(isFull) {
				// TO BE IMPLEMENTED
				cout << "FULL SIZE" << endl;
			}
			// Add new customer if not full
			else {
				Table newTable{(customerID - 1)%MAXSIZE + 1, customerResult};

				if(customerResult%2 == 0 || Location_HashTable.IsFull()) { // IF Customer is located in AVL || Hash-Table Area is Full.
					Location_AVL->InsertTable(newTable);
					IsHash[customerID] = false;
					cout << "AVL TREE" << endl;
					Location_AVL->PrintTree();
				}
				else if(customerResult%2 == 1 || Location_AVL->IsFull()) { // IF Customer is located in Hash-Table || AVL Area is Full
					Location_HashTable.AddTable(newTable, customerResult%3);
					IsHash[customerID] = true;
					cout << "HASH TABLE" << endl;
					Location_HashTable.PrintTable();
				}

				// We will push current table into customer query for removal
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
			cout << number << endl;

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
				for(int i = i; i <= MAXSIZE; ++i) {
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

				// Empty Table Case
				if(RecordResult[number] == -1) continue;

				// We got a table to delete
				else {
					// The table we delete is in HashTable Area
					if(IsHash[number]) {
						Location_HashTable.RemoveTable(number);
						RecordResult[number] = -1;
					}
					// The table we delete is in AVL Area
					else {
						Table delTable{(number, RecordResult[number])};
						Location_AVL->DeleteTable(delTable);
						RecordResult[number] = -1;
						IsHash[number] = true;
					}
				}
			}

			Location_AVL->PrintTree();
			Location_HashTable.PrintTable();
		}
	}

	return;
}

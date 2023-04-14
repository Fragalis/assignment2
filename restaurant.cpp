#include "main.h"
#include "huffman_code.cpp"
#include "avl.cpp"

void simulate(string filename) {
	ifstream input("test.txt");
	// CHECK FOR VALID FILE INPUT
	if(!input) return;
	
	// A VECTOR TO KEEP RECORD OF TABLE ID, which id 0 is untouched;
	vector<bool> RecordID(MAXSIZE + 1, false);

	// LOCATION 1: Hash-Table

	// LOCATION 2: AVL Tree
	AVLTree *Location_AVL = new AVLTree();

	// INITIAL COMMAND LINE INPUT
	string line = "";
	while(getline(input, line)) {
		int index = 0;
		string keyword = "";

		// READ KEYWORD
		for(index; index < line.length() and line[index] != ' '; ++index) {
			keyword += line[index];
		}

		//cout << keyword << endl;
		
		// PUSH INDEX TO NEXT ELEMENT
		++index;

		if(keyword == "REG") {
			bool isFull = false;
			string name = "";
			// READ NAME
			for(index; index < line.length() and line[index] != ' '; ++index) {
				name += line[index];
			}
			// name = "aabbcba";
			cout << name << endl;

			// HuffmanEncoding(string name, bool caseSensitive)
			int customerResult = HuffmanEncoding(name, 0);
			cout << customerResult << endl;

			// If customerResult % MAXSIZE = 0 -> set ID to MAXSIZE
			int customerID = (customerResult % MAXSIZE)? customerResult % MAXSIZE : MAXSIZE;

			// We traverse the table vector to find empty table
			while((customerID <= 2 * MAXSIZE) && RecordID[(customerID - 1)%MAXSIZE + 1]) {
				++customerID;
			}
			
			if(customerID > 2 * MAXSIZE) { // NO TABLE LEFT
				isFull = true;
			}
			else { // THERE IS AT LEAST 1 TABLE
				customerID = (customerID - 1)%MAXSIZE + 1;
				RecordID[customerID] = true;
				// cout << customerID << endl;
			}

			// EXECUTING REG COMMAND
			
			if(isFull) { // IF There's no table, remove a customer
				// TO BE IMPLEMENTED
				cout << "FULL SIZE" << endl;
			}
			else { // There is at least 1 table, add a customer
				Table newTable{(customerID - 1)%MAXSIZE + 1, customerResult};
				if(customerResult%2 == 0 || false) { // IF Customer is located in AVL || Hash-Table Area is Full.
					Location_AVL->Insert(newTable);
				}
			}


			// FOR TABLE ID TESTING
			// for(int i = 1; i <= MAXSIZE; ++i) cout << RecordID[i] << " ";
			// cout << endl;
			
		}
	}

	return;
}

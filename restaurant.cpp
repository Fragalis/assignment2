#include "main.h"
#include "huffman_code.cpp"

void simulate(string filename) {
	ifstream input("test.txt");
	// CHECK FOR VALID FILE INPUT
	if(!input) return;
	
	// A VECTOR TO KEEP RECORD OF TABLE ID
	vector<bool> RecordID(MAXSIZE + 1, false);

	// LOCATION 1: Hash-Table

	// LOCATION 2: AVL Tree
	// To be implemented

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
			// cbool isFull = false;
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
			while((customerID == 2 * MAXSIZE) && RecordID[(customerID - 1)%MAXSIZE + 1]) {
				++customerID;
			}
			
			// If there's no empty table:
			if(customerID > 2 * MAXSIZE) cout << "FULL SIZE" << endl;
			else {
				customerID = (customerID - 1)%MAXSIZE + 1;
				RecordID[customerID] = true;
				cout << customerID << endl;
			}

			
		}
	}

	return;
}

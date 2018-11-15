// Digital Design 1
// Project 1
// Quine-Mccluskey
// Ahmed Refaay (900141806) & Lotfy Hussein (900143801)
# include <iostream>
# include <vector>
# include <cmath>
# include <string>
using namespace std;

struct implicant {
	int element = {}; // If implicant covers more than an element, then this is the smallest of them
	int dashesNo = {}; // Number of dashes and also number of column
	int dashes = -1; // Actual dashes places as a hexa-decimal number of descending places, initialized as -1 because 0 indicates a dash at the 0-bit
	int ticked = {}; // 1 if ticked, 0 if not
};

void input(int &varNo, int &minNo, int &dcNo){ // To receive the input constraints
	do {
		cout << "Enter number of variables: ";
		cin >> varNo;
		if (varNo > 16 || varNo <= 0) cout << "Re-"; // If out of our project rubrics' scope
	} while (varNo > 16 || varNo <= 0); // Validation
	do {
		cout << "Enter number of minterms: ";
		cin >> minNo;
		if (minNo > pow(2.0, varNo) || minNo < 0) cout << "Re-"; // If out of function's scope
	} while (minNo > pow(2.0, varNo) || minNo < 0); // Validation
	do {
		cout << "Enter number of don't cares: ";
		cin >> dcNo;
		if (dcNo > pow(2.0, varNo) - minNo || dcNo < 0) cout << "Re-"; // If out of function's scope taking minterms into considiration
	} while (dcNo > pow(2.0, varNo) - minNo || dcNo < 0); // Validation
}

void input2(int *minterms, vector<implicant>& impTable, const int& varNo, const int& minNo, const int& dcNo){ // Receives minterms and don't care terms
	implicant temp; // Temporary implicant to push into implication table vector
	int i = 0, input; // Iterator & temporary input for cin
	bool wrong, repeated; // Booleans if input is not valid or was already entered
	if (minNo != 0) cout << "Enter all minterms each in a line: "; // If there are minterms
	for (i; i < minNo; i++){ // Looping on all minterms
		wrong = false, repeated = false; // Let it be not a wrong nor a repeated input
		do{
			repeated = false;  // To check each input
			cin >> input;
			if (wrong = input < 0) cout << "Re-Enter the last minterm because it is negative: "; // If negative
			if (!wrong) if (wrong = input > pow(2.0, varNo)) cout << "Re-Enter the last minterm because it is out of the scope of the function: "; // If out of scope
			if (!wrong) for (int j = 0; j < i && !repeated; j++){ // Search for repeatance
				repeated = input == impTable[j].element;
			}
			if (repeated) { cout << "Re-Enter the last minterm because it is repeated: "; wrong = repeated; } // If repeated
		} while (wrong); // Till it is a right input
		minterms[i] = input;  // Push input into minterms' dynamic array
		temp.element = minterms[i];
		impTable.push_back(temp);  // Push input into implication table vector of implicants
	}
	if (dcNo != 0) cout << "Enter all don't cares each in a line: "; // If there are don't care terms
	for (i; i < minNo + dcNo; i++){ // Looping on all don't care terms and adding after the minterms
		wrong = false, repeated = false; // Let it be not a wrong or a repeated input
		do{
			repeated = false;  // To check each input
			cin >> input;
			if (wrong = input < 0) cout << "Re-Enter the last don't care term because it is negative: "; // If negative
			if (!wrong) if (wrong = input > pow(2.0, varNo)) cout << "Re-Enter the last don't care term because it is out of the scope of the function: "; // If out of scope
			if (!wrong) for (int j = 0; j < i && !repeated; j++){ // Search for repeatance
				repeated = input == impTable[j].element;
			}
			if (repeated) { cout << "Re-Enter the last don't care term because it is repeated: "; wrong = repeated; } // If repeated
		} while (wrong); // Till it is a right input
		temp.element = input;
		impTable.push_back(temp);  // Push input into implication table vector of implicants
	}
}

int calculateDashes(int tempi, int prev){ // Calculating the places of the dashes using the prevoius dashes found in the implicant and the new bit to be a dash
	if (prev == -1) return tempi; // If first dash
	if (prev == 0) return tempi; // Base case for recursion
	int d = prev % 16; // Last hexa-decimal digit
	if (d > tempi){ // Descendingly
		prev *= 16;
		prev += tempi;
		return prev;
	}
	prev = calculateDashes(tempi, prev / 16); // Recurse
	prev *= 16;
	prev += d;
}

void getPrimeImp(vector<implicant>& impTable, vector<implicant>& primeImp, int *minterms, const int& varNo, const int& minNo, const int& dcNo){ // Gets implication table and discovers prime implicants
	implicant tempImp; // Temporary implicant to push into implication table vectors
	int tempi, count = minNo + dcNo; // tempi for checking integer log2, count to loop on all implication table
	float tempf; //  tempf for checking integer log2
	int currentE; // To calculate all elements in an implicant
	bool containsMin; // Boolean to check whether a prime impliacant contains minterms or just covers don't care terms
	while (count != 0){ // Loop on the whole table's column
		vector<implicant> nextColumn; // To store the implicants moved to the next column
		for (int i = 0; i < count; i++){ // Compare each implicant with the following implicants
			for (int j = i + 1; j < count; j++){
				tempi = tempf = log2(impTable[i].element ^ impTable[j].element); // Calculate weither change is in only one bit 
				if (tempi == tempf && impTable[i].dashes == impTable[j].dashes) { // If these two implicants can make a cover together by having a 1 bit difference
					impTable[i].ticked = impTable[j].ticked = 1; // Tick them
					tempImp.dashesNo = impTable[i].dashesNo + 1; // Dashes +1
					tempImp.element = fmin(impTable[i].element, impTable[j].element); // Next implicant's element representative is the smallest in the cover, that is: has zeroes in the places of dashes
					if (impTable[i].dashes == 0)  tempImp.dashes = tempi * 16; // If previously a dash at the 0 bit, special case because of the recursion in calculateDashes function
					else tempImp.dashes = calculateDashes(tempi, impTable[i].dashes); // Calculate the dashes positions into hexa-decimal descendingly
					bool repeated = false; // To check if the result implicant is already found in the next column
					for (int k = 0; k < nextColumn.size() && !repeated; k++){ // Loop on all next column's implicants
						repeated = (tempImp.element == nextColumn[k].element) && (tempImp.dashes == nextColumn[k].dashes); // Repeated only if same smallest element and same dashes places
					}
					if (!repeated)
						nextColumn.push_back(tempImp); // Add result implicant to next column in the implication table
				}
			}
		}
		for (int i = 0; i < count; i++) // Moving all none ticked implicants in the current column to the prime implicants vector
			if (impTable[i].ticked == 0) { // This implicant may be a prime one
			containsMin = false;
			for (int j = 0; j < pow(2.0, impTable[i].dashesNo) && !containsMin; j++){ // Loop to get all elements of a certain prime implicant
				currentE = impTable[i].element;
				int a = j, b = impTable[i].dashes;
				if (b == 0) currentE += (a % 2)*pow(2.0, (b % 16));
				else while (b != 0){
					currentE += (a % 2)*pow(2.0, (b % 16));
					a /= 2;
					b /= 16;
				}
				for (int k = 0; k < minNo && !containsMin; k++){
					containsMin = (currentE == minterms[k]);
				}
			}
			if (containsMin) primeImp.push_back(impTable[i]); }
		impTable = nextColumn;
		count = nextColumn.size();
	}
}

void getEssentialImp(const vector<implicant>& primeImp, int *minterms, int *mintRep, int *whichPI, const int& minNo) {
	int currentE; // To calculate all elements in an implicant
	for (int i = 0; i < minNo; i++) mintRep[i] = 0; // Initializing all repeatances to 0
	for (int i = 0; i < primeImp.size(); i++){ // Loop on all prime implicants in primeImp vector
		for (int j = 0; j < pow(2.0, primeImp[i].dashesNo); j++){ // Loop to get all elements of a certain prime implicant
			currentE = primeImp[i].element;
			int a = j, b = primeImp[i].dashes;
			if (b == 0) currentE += (a % 2)*pow(2.0, (b % 16));
			else while (b != 0){
				currentE += (a % 2)*pow(2.0, (b % 16));
				a /= 2;
				b /= 16;
			}
			for (int k = 0; k < minNo; k++){
				if (currentE == minterms[k]){
					mintRep[k]++;
					whichPI[k] = i;
				}
			}
		}
	}
}

string printImp(const implicant& tempImp, const int& varNo){ // ABC...
	int a = tempImp.dashes, b = a % 16, c = tempImp.element, d;
	string output;
	for (int i = 0; i < varNo; i++){
		d = c % 2;
		c /= 2;
		if (i == b){ a /= 16; b = a % 16; }
		else if (d == 1) output.insert(0, 1, char(64 + varNo - i));
		else{
			output.insert(0, "'");
			output.insert(0, 1, char(64 + varNo - i));
		}
	}
	return output;
}

string printImp2(const implicant& tempImp, const int& varNo){ // XnXn-1....X1X0
	int a = tempImp.dashes, b = a % 16, c = tempImp.element, d;
	string output;
	for (int i = 0; i < varNo; i++){
		d = c % 2;
		c /= 2;
		if (i == b){ a /= 16; b = a % 16; }
		else if (d == 1) { output.insert(0, 1, char(48 + i)); output.insert(0, "X"); }
		else{
			output.insert(0, "'");
			output.insert(0, 1, char(48 + i)); output.insert(0, "X");
		}
	}
	return output;
}

void main(){
	// Inputs
	int varNo, minNo, dcNo; // Number of variables, minterms, and don't care terms
	int *minterms; // Dynamic array of minterms
	vector<implicant> impTable; // Implication table as a vector of implicants
	input(varNo, minNo, dcNo); // Receive inputs' contraints
	minterms = new int[minNo];
	input2(minterms, impTable, varNo, minNo, dcNo); // Receive minterms and don't care terms

	vector<implicant> primeImp; // Prime implicants vector
	getPrimeImp(impTable, primeImp, minterms, varNo, minNo, dcNo); // Discover prime implicants
	int *mintRep; // Count number of repeatance of minterms in prime implicants
	mintRep = new int[minNo];
	int *whichPI; // Count number of repeatance of minterms in prime implicants
	whichPI = new int[minNo];
	getEssentialImp(primeImp, minterms, mintRep, whichPI, minNo); // Discover essential prime implicants

	cout << "Minterms: "; // Output minterms all in one line
	for (int i = 0; i < minNo; i++){
		cout << minterms[i] << " ";
	}
	cout << endl;
	int c = 0;
	cout << "Prime implicants: \n";
	for (int i = 0; i < primeImp.size(); i++){ // Output all prime implicants
		c++;
		cout /*<< dec << primeImp[i].element << " " << hex << primeImp[i].dashes << " " */ << printImp(primeImp[i], varNo) << endl; // Each line has the smallest element in the prime implicant and the places of the dashes combined in a hexa-decimal number decsendingly
	}
	if (c == 0) cout << "There are no prime implicants nor essential prime implicants." << endl;
	else {
		cout << "Number of prime implicants: " << dec << c << endl;
		c = 0;
		cout << "Essential prime implicants: \n";
		for (int i = 0; i < minNo; i++){
			if (mintRep[i] == 1) {
				primeImp[whichPI[i]].ticked++; // To print it once
				if (primeImp[whichPI[i]].ticked == 1) {
					c++;
					cout /* << dec << primeImp[whichPI[i]].element << " " << hex << primeImp[whichPI[i]].dashes << " "*/ << printImp(primeImp[whichPI[i]], varNo) << endl;
				}
			}
		}
		if (c == 0) cout << "There are no essential prime implicants." << endl;
		else cout << "Number of essential prime implicants: " << dec << c << endl;
	}
	system("pause");
}
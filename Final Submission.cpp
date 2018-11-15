// Digital Design 1
// Project 1
// Quine-Mccluskey
// Ahmed Refaay (900141806) & Lotfy Hussein (900143801)
# include <iostream>
# include <vector>
# include <cmath>
# include <string>
# include<iomanip>
using namespace std;

struct implicant {
	int element = {}; // Minterms or don't care terms initially
	vector<int> DecMinterms; // Stores minterms/don't cares when more than 1
	string ele = " "; // Implicant as a string of binary bits and dashes
	int ticked = 0; // 1 if ticked, 0 if not
};

void input(int &varNo, int &minNo, int &dcNo) { // To receive the input constraints
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

void input2(int*minterms, vector<implicant>& impTable, const int& varNo, const int& minNo, const int& dcNo) { // Receives minterms and don't care terms
	implicant temp; // Temporary implicant to push into implication table vector
	int i = 0, input; // Iterator & temporary input for cin
	bool wrong, repeated; // Booleans if input is not valid or was already entered
	if (minNo != 0) cout << "Enter all minterms each in a line: "; // If there are minterms
	for (i; i < minNo; i++) { // Looping on all minterms
		wrong = false, repeated = false; // Let it be not a wrong or a repeated input
		do {
			repeated = false;  // To check each input
			cin >> input;
			if (wrong = input < 0) cout << "Re-Enter the last minterm because it is negative: "; // If negative
			if (!wrong) if (wrong = input > pow(2.0, varNo)) cout << "Re-Enter the last minterm because it is out of the scope of the function: "; // If out of scope
			if (!wrong) for (int j = 0; j < i && !repeated; j++) { // Search for repeatance
				repeated = input == impTable[j].element;
			}
			if (repeated) { cout << "Re-Enter the last minterm because it is repeated: "; wrong = repeated; } // If repeated
		} while (wrong); // Till it is a right input
		minterms[i] = input;  // Push input into minterms' dynamic array
		temp.element = minterms[i];
		impTable.push_back(temp);  // Push input into implication table vector of implicants
	}
	if (dcNo != 0) cout << "Enter all don't cares each in a line: "; // If there are don't care terms
	for (i; i < minNo + dcNo; i++) { // Looping on all don't care terms and adding after the minterms
		wrong = false, repeated = false; // Let it be not a wrong or a repeated input
		do {
			repeated = false;  // To check each input
			cin >> input;
			if (wrong = input < 0) cout << "Re-Enter the last don't care term because it is negative: "; // If negative
			if (!wrong) if (wrong = input > pow(2.0, varNo)) cout << "Re-Enter the last don't care term because it is out of the scope of the function: "; // If out of scope
			if (!wrong) for (int j = 0; j < i && !repeated; j++) { // Search for repeatance
				repeated = input == impTable[j].element;
			}
			if (repeated) { cout << "Re-Enter the last don't care term because it is repeated: "; wrong = repeated; } // If repeated
		} while (wrong); // Till it is a right input
		temp.element = input;
		impTable.push_back(temp);  // Push input into implication table vector of implicants
	}
}

string DecToBin(const int& number) // Translates decimal minterm or don't care term into a string of binary digits equivalent
{
	if (number == 0) return "0"; // Base case if least significant binary digit is 0
	if (number == 1) return "1"; // Base case if least significant binary digit is 1

	if (number % 2 == 0) // If least significant binary digit is 0
		return DecToBin(number / 2) + "0"; // Add "0" and recur on the other bits
	else // If least significant binary digit is 1
		return DecToBin(number / 2) + "1"; // Add "1" and recur on the other bits
}

void getPrimeImplicants(vector<implicant>& impTable, int*minterms, vector<implicant>& primeImp, const int& varNo, const int& minNo, const int& dcNo) {
	for (int i = 0; i < minNo + dcNo; i++) { // Translate into string of binary bits 
		impTable[i].ele = (DecToBin(impTable[i].element)); // Original translation
		while (impTable[i].ele.size() < varNo) { // Unsigned extention to the birany representation of the implicants
			impTable[i].ele = "0" + impTable[i].ele;
		}
	}
	int elementsNo = minNo + dcNo;
	for (int h = 0; h < elementsNo; h++) // Push the initial minterms/don't cares
		impTable[h].DecMinterms.push_back(impTable[h].element);
	while (elementsNo != 0) { // Loop on all minterms/don't cares
		vector <implicant> next; // Next column in implication table
		for (int i = 0; i < elementsNo; i++) { // Nested loops on all combinations of elements
			for (int j = i + 1; j < elementsNo; j++) {
				implicant temp; // Temporary implicant
				int countDiff = 0; // Counts the bit difference between two implicants
				char r; // To know the place of the dash
				for (int k = 0; k < varNo; k++) { // Loop on all bits in the two current implicants i&j
					if (impTable[i].ele.at(k) != impTable[j].ele.at(k)) { // If a different bit
						countDiff++;
						r = k;
					}
				}
				if (countDiff == 1){ // If difference is in one bit only
					impTable[i].ticked = 1; // Tick i
					impTable[j].ticked = 1; // Tick j
					temp.ele = impTable[i].ele; // Temporary's element is one of them
					temp.ele.at(r) = '-'; // Place a dash at the difference bit
					for (int p = 0; p < impTable[i].DecMinterms.size(); p++) // Get all elements in i to temp
						temp.DecMinterms.push_back(impTable[i].DecMinterms[p]);
					for (int t = 0; t < impTable[j].DecMinterms.size(); t++) // Get all elements in j to temp
						temp.DecMinterms.push_back(impTable[j].DecMinterms[t]);
					bool repeated = false; // If already pushed into next column
					for (int y = 0; y < next.size() && !repeated; y++) {
						if (temp.ele == next[y].ele)
							repeated = true;
					}
					if (repeated == false) // If not found in next column, push
						next.push_back(temp);
				}
			}
		}
		for (int i = 0; i < elementsNo; i++){ // Push prime implicants
			if (impTable[i].ticked == 0){ // If prime
				bool flag = true;
				for (int v = 0; v < minNo && flag; v++){ // Loop to check whether the implicant covers only don't cares
					for (int w = 0; w < impTable[i].DecMinterms.size(); w++){
						if (impTable[i].DecMinterms[w] == minterms[v]){ // If at least a minterm is found in this implicant, push
							primeImp.push_back(impTable[i]);
							flag = false;
						}
					}
				}
			}
		}
		impTable = next; // Go to next column
		elementsNo = next.size();
	}
}

void Output(vector<implicant>& primeImp){ // To make alphabetical output
	for (int i = 0; i < primeImp.size(); i++){ // loop on all prime/essential prime implicants in the vector
		char y = 'A';
		cout << primeImp[i].ele << " "; // cout the binary representation string
		for (int j = 0; j < primeImp[i].ele.size(); j++){ // cout the alphabitical representation
			if (primeImp[i].ele.at(j) == '1'){
				cout << y << " ";
				y++;
			}
			else if (primeImp[i].ele.at(j) == '0'){
				cout << y << "'";
				y++;
			}
			else
				y++; // If a dash, no alphabitical output
		}
		cout << endl;
	}
	cout << endl;
}

void getEssPrime(vector<implicant> &Ess, vector<implicant> &primeImp, int *minterms, vector<vector<int>> &chart, const int& minNo, const int& varNo){
	vector<implicant> temp = primeImp; // Temporary vector of prime implicants
	for (int i = 0; i < temp.size(); i++) { // Make chart of zeroes
		vector<int>Vtemp; // For each row
		for (int j = 0; j < minNo; j++){
			Vtemp.push_back(0);
		}
		chart.push_back(Vtemp); // Push into chart
	}
	for (int i = 0; i < temp.size(); i++){ // Make actual chart, loop on all implicants
		for (int j = 0; j < minNo; j++){ // On all minterms
			for (int k = 0; k < temp[i].DecMinterms.size(); k++){ // On all elements in this implicant
				if (temp[i].DecMinterms[k] == minterms[j]) // If they match
					chart[i][j] = 1;
			}
		}
	}
	for (int j = 0; j < minNo; j++){ // Get essential prime implicants
		int count = 0;
		implicant temp2; // Store the essential implicant
		for (int i = 0; i < temp.size(); i++){ // Counts the ones in each column
			if (chart[i][j] == 1){ // If there is one x in the column
				count++;
				temp2 = temp[i]; // Store the essential prime implicant
			}
		}
		if (count == 1){ // If minterm is found only in one implicant
			bool repeated = false; // If not already found in essential prime implicants
			for (int y = 0; y < Ess.size() && !repeated; y++){
				if (temp2.ele == Ess[y].ele)
					repeated = true;
			}
			if (repeated == false) // If not found in essential prime implicants, push
				Ess.push_back(temp2);
		}
	}
	// Print essentials
	cout << endl;
	cout << "Essential Prime Implicants: " << endl << endl;
	Output(Ess);
	if (Ess.size() == 0)
		cout << "There are no Essential Prime Implicants " << endl;
	else
		cout << "Number of Essential prime Implicants: " << Ess.size() << endl << endl;

	// Draw the coverage chart
	cout << "====================================" << endl;
	cout << "The Coverage Table" << endl;
	cout << "====================================" << endl;
	cout << endl;
	for (int m = 0; m < varNo; m++)
		cout << " ";
	cout << " ";
	for (int a = 0; a < minNo; a++)
		cout << setw(log10(pow(2.0, varNo) - 1) + 1) << minterms[a] << " "; // Set width to the max digits' number of the decimal representation of the minterms
	cout << endl;
	for (int i = 0; i < temp.size(); i++){
		cout << endl;
		cout << primeImp[i].ele << " "; // cout the implicant of the current row
		for (int j = 0; j < minNo; j++){
			cout << setw(log10(pow(2.0, varNo) - 1) + 1) << chart[i][j] << " "; // cout the coverage row
		}
	}
	cout << endl;
}

void main() {
	// Inputs
	cout << "========================================" << endl;
	cout << "Logic Optimization using Quine-McCluskey " << endl;
	cout << "========================================" << endl;

	int varNo, minNo, dcNo; // Number of variables, minterms, and don't care terms
	int *minterms; // Dynamic array of minterms
	vector<implicant> impTable; // Implication table as a vector of implicants
	input(varNo, minNo, dcNo); // Receive inputs' constraints
	minterms = new int[minNo];
	input2(minterms, impTable, varNo, minNo, dcNo); // Receive minterms and don't care terms

	
	if (minNo  == 0) // If f always equal zero
		cout << "The function is always 0 'Contradiction' " << endl;
	else { // Print minterms
		cout << endl;
		cout << "Minterms: " << endl;
		for (int i = 0; i < minNo; i++) {
			cout << impTable[i].element << " ";
		}
		cout << endl;
		cout << endl;

		// Get and print the prime implicants
		if (minNo == pow(2, varNo)) // If all minterms make the output one
			cout << "The function is always 1 'Tautology' " << endl;
		else{
			cout << "Prime Implicants: " << endl;
			vector<implicant> primeImp;

			getPrimeImplicants(impTable, minterms, primeImp, varNo, minNo, dcNo);
			Output(primeImp);
			if (primeImp.size() == 0)
				cout << "There are no Prime Implicants nor Essential Prime Implicants." << endl;
			else{
				cout << "Number of prime Implicants: " << primeImp.size();
				cout << endl;

				// Get and print the essential prime implicants
				vector<implicant> Ess;
				vector<vector<int>> chart;
				getEssPrime(Ess, primeImp, minterms, chart, minNo, varNo);
			}
		}
	}
	delete[] minterms;
	system("pause");
}
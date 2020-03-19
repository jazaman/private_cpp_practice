//============================================================================
// Name        : ThreeN_Conjecture.cpp
// Author      : Jamil Anwar Zama
// Version     :
// Copyright   : Copyright Jamil Anwar Zaman
// Description : run the program ThreeN_conjecture < input, where input is
//               pair of integers per line.
//============================================================================

#include <iostream>
#include <sstream>
using namespace std;
int conjecture_formula(int number, int level);

int main(int argc, char* argv[]) {
	int n1, n2;
	int maxlevel = 0;
	string input;
	for(;getline(cin, input);) {

		maxlevel = 0;

		stringstream sstream(input);

		if(sstream >> n1  && sstream >> n2) {
			int start = (n1 < n2)?n1:n2;
			int end = (n1 < n2) ? n2:n1;
			for(int j = start; j <= end; j++) {
				int temp = conjecture_formula(j,1);
				maxlevel = (temp > maxlevel ? temp:maxlevel);
			}
		} else {
			cerr << "Invalid input" << endl;
		}
		cout << n1 << " " << n2 << " " << maxlevel << endl;
	}


		return 0;
}

int conjecture_formula (int number, int level) {

	if(number == 1) {
		return level;
	}

	if(number % 2) {
		return conjecture_formula((3 * number) + 1, level +1);
	} else {
		return conjecture_formula(number /2, level +1);
	}
}

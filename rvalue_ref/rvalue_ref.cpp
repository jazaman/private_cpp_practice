//============================================================================
// Name        : rvalue_ref.cpp
// Author      : Jamil Anwar Zaman
// Version     :
// Copyright   : Copyright Jamil Anwar Zaman
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
using namespace std;

class A{
private: 

public: 
	A(){ 
		cout <<"default constructor" << endl;
	}
	~A(){ 
		cout <<"destructor" << endl;
	}
	
	A(const A& ) {
		cout <<"default copy constructor" << endl;
	}
	
};

int main() {
    cout << "RValue Reference" << endl; // prints RValue Reference
	std::vector<A> items;
	//A&& rvalue = A();
	items.push_back(std::move(A()));
	//A a;
	//items.push_back(a);
    return 0;
}

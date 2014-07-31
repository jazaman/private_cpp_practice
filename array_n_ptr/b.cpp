#include <iostream>

extern int t[];
// extern int *t; // will cause segmentation fault
using namespace std;

int g( int *par)
{
    cout << par[2] << endl;     // ok
    cout <<   t[2] << endl;     // runtime error
    return 0;
}

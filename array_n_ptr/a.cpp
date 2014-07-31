#include <iostream>

using namespace std;

int t[] = { 1, 2, 3, 4, 5};

extern int g(int* par);

int main()
{
    cout << t[2] << endl;   // ok
    
    int *p = t;
    cout << p[2] << endl;   // ok
    
    g(p);
    g(t);
    
    return 0;
}

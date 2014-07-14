#include <iostream>

using namespace std;

struct A      { virtual ~A() { }
                 virtual string Name() { return "A";  } };
struct B1 : virtual A { string Name() { return "B1"; } };
struct B2 : virtual A { string Name() { return "B2"; } };

struct D  : B1, B2    { string Name() { return "D";  } };

void f1( A&  x ) { cout << "f1:" << x.Name() << endl; }
void f2( B1& x ) { cout << "f2:" << x.Name() << endl; }
void f3( B2& x ) { cout << "f3:" << x.Name() << endl; }

void g1( A   x ) { cout << "g1:" << x.Name() << endl; }
void g2( B1  x ) { cout << "g2:" << x.Name() << endl; }
void g3( B2  x ) { cout << "g3:" << x.Name() << endl; }

int main() {
    D   d;
    B1* pb1 = &d;   // D* -> B* conversion
    B2* pb2 = &d;
    B1& rb1 = d;    // D& -> B& conversion
    B2& rb2 = d;

    f1( d );        // polymorphism
    f2( d );
    f3( d );

    g1( d );        // slicing
    g2( d );
    g3( d );
                    // dynamic_cast/RTTI
    cout << ( (dynamic_cast<D*>(pb1) != 0)
            ? "ok " : "bad " ) << endl;
    cout << ( (dynamic_cast<D*>(pb2) != 0)
            ? "ok " : "bad " )<< endl;

    try {
        dynamic_cast<D&>(rb1);
        cout << "ok "<< endl;
    } catch(...) {
        cout << "bad "<< endl;
    }
    try {
        dynamic_cast<D&>(rb2);
        cout << "ok "<< endl;
    } catch(...) {
        cout << "bad "<< endl;
    }
}

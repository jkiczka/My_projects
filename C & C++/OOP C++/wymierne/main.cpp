#include <iostream>
#include "wymierne.hpp"
#include "wyjatki_wymierne.hpp"
using namespace std;
using namespace obliczenia;

int main()
{
    try
    {
    wymierna x(1,10);
    wymierna y(7,5);
    cout<<"x: "<<x<<endl<<"y: "<<y<<endl;
    cout<<"x+y: "<<x+y<<endl<<"x-y: "<<x-y<<endl<<"y-x: "<<y-x<<endl<<"x*y: "<<x*y<<endl<<"x/y: "<<x/y<<endl<<"y/x: "<<y/x<<endl;

    wymierna z(987656799,100000000);

    wymierna a(7,12);
    wymierna b(1,3);
    wymierna c(1,7);

    cout<<endl<<a<<endl<<b<<endl<<c<<endl;
    cout<<endl<<z<<endl;
    cout<<"int: "<<int(z)<<endl;
    cout<<"double: "<<double(z)<<endl;

    //cout<<z+z;
    //wymierna k(5,0);



    }

    catch  (exception &e)
    {
        cout<<e.what()<<endl;
    }
    return 0;
}

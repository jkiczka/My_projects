#include <iostream>
#include "l8.hpp"
#include "wyjatki_macierze.hpp"
using namespace std;

int main()
{
    try{

    wektor w0(5);
    w0[1]=3;
   w0[3]=2;
   cout<<w0<<endl;
   wektor w1 {1, 2, 3, 4, 5};
   wektor w2 {5, 4, 3, 2, 1};



   cout<<"w1:"<<w1<<endl<<"w2:"<<w2<<endl;
   cout<<"iloczyn skalarny "<<w1*w2<<endl;
   cout<<"dodawanie "<<w1+w2<<endl;
   cout<<"odejmowanie "<<w1-w2<<endl;
   w1=w2;
   cout<<"w1=w2"<<endl<<w1<<endl<<w2<<endl;
   w1+=w0;
   cout<<"w1+=w0"<<endl<<w1<<endl<<w0<<endl;
   w1*=0.5;
   cout<<"w1*=0.5"<<endl<<w1<<endl;




   macierz m(3,4);
   m[1][1]=1;
   m[2][2]=4;
   m[0][0]=5;
   m[0][1]=7;
   macierz k(3,4);
   k[0][0]=6;
   k[0][1]=1;
   k[1][0]=2;
   k[1][1]=3;
   k[2][3]=7;
      cout<<endl<<"m"<<endl<<m<<endl<<"k"<<endl<<k<<endl<<"m-k:"<<endl<<m-k<<endl;



macierz m1 {
 {1, 0,0},
 {-1, 3,4},
 {
     3,5,2
 },
};
//
macierz m2 {
 {3,1,8},
 {2,1,1},
 {
     7,8,5
 },

};
cout<<endl<<"m1"<<endl<<m1<<endl<<"m2"<<endl<<m2<<endl;
m2+=m1;
cout<<"m2+=m1"<<endl<<m2;

cout<<"m2*m1"<<endl<<m2*m1<<endl;


cout<<"~m1"<<endl<<~m1<<endl;

w0[300];

    }
    catch  (exception &e)
    {
        cout<<e.what()<<endl;
    }




    return 0;
}

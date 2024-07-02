
#include <iostream>
#include<string>
#include <initializer_list>

using namespace std;

class wielomian
{
    private:
    int st;
    double *tab;


public:
 wielomian (int st=0, double wsp=1.0); // konstruktor jednomianu
 wielomian (int st, const double wsp[]); // konstruktor wielomianu
 wielomian (initializer_list<double> lst); // lista wsp�czynnik�w
 wielomian (int s);
 wielomian (const wielomian &w); // konstruktor kopiuj�cy
 wielomian (wielomian &&w); // konstruktor przenosz�cy
 wielomian& operator = (const wielomian &right); // przypisanie kopiuj�ce
 wielomian& operator = (wielomian &&temp); // przypisanie przenosz�ce



 friend wielomian operator + (const wielomian &u, const wielomian &v);
 friend wielomian operator - (const wielomian &u, const wielomian &v);
 friend wielomian operator * (const wielomian &u, const wielomian &v);
 friend wielomian operator * (const wielomian &u,double c);
 friend istream& operator >> (istream &we, wielomian &w);
 friend ostream& operator << (ostream &wy, const wielomian &w);



 wielomian& operator += (const wielomian &v);
 wielomian& operator -= (const wielomian &v);
 wielomian& operator *= (const wielomian &v);
 wielomian& operator *= (double c);


 double operator () (double x) const; // warto�� wielomianu dla x
 double operator [] (int i) const; // do odczytu wsp�czynnika
 double& operator [] (int i); // do zapisu wsp�czynnika
 ~wielomian (); // destruktor
 int stopien();



};






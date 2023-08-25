
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
 wielomian (initializer_list<double> lst); // lista wspó³czynników
 wielomian (int s);
 wielomian (const wielomian &w); // konstruktor kopiuj¹cy
 wielomian (wielomian &&w); // konstruktor przenosz¹cy
 wielomian& operator = (const wielomian &right); // przypisanie kopiuj¹ce
 wielomian& operator = (wielomian &&temp); // przypisanie przenosz¹ce



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


 double operator () (double x) const; // wartoœæ wielomianu dla x
 double operator [] (int i) const; // do odczytu wspó³czynnika
 double& operator [] (int i); // do zapisu wspó³czynnika
 ~wielomian (); // destruktor
 int stopien();



};






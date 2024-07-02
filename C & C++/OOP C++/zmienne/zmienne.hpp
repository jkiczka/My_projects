#include<string>
#include<iostream>
using namespace std;
class zmienna
{
private:
    string nazwa;
    double wartosc;
public:
    zmienna();
    zmienna(string n);
    zmienna(string n, double w);
    string get_n();
    double get_w();
    void set_w(double x);
};
class zbior_zmiennych
{
public:
    const int n;
    zmienna *tab;
    zbior_zmiennych(int k);
    ~zbior_zmiennych();
    void wstawienie_zmiennej(zmienna z);
    int czy_w_zbiorze(string name);
    void usun(zmienna z);
    double odczyt(zmienna z);
    void zmien_wartosc(zmienna, double x);
    void wypisz_zbior();
};


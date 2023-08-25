#ifndef PUNKT_HPP_INCLUDED
#define PUNKT_HPP_INCLUDED
#include <iostream>
#include<string>
#include <initializer_list>

using namespace std;

class kolejka
{
    private:
    int pojemnosc, poczatek = 0, ile = 0;

    string *tab;
    public:
    void wstaw(string);
    string usun();
    string zprzodu();
    int dlugosc();
    kolejka(); //bezparametrowy i delegatowy (domysla poj=1)
    kolejka(int k); //z pojemnoscia
    kolejka(initializer_list<string> lst); //z lista inicjaliz
    kolejka(const kolejka &wzorzec); //kopiujacy
    const kolejka operator=(const kolejka &right);
    kolejka(kolejka&& temp); //przenoszacy
    kolejka& operator=(kolejka&& right);
    ~kolejka();
};


#endif

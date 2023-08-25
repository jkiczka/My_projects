#include "kolejka.hpp"
#include <exception>
#include <new>
#include<string>
#include <initializer_list>
#include <iostream>
#include<algorithm>
kolejka::~kolejka()
{
    //if(tab!=nullptr)
    delete [] tab;
}


kolejka::kolejka(int k) //z pojemnoscia
:pojemnosc((k>0)?k:0)
{
    if(k>0)
    tab= new string[k];
    else
        {

                std::string rozmiar="Podaj rozmiar kolejki  > 0";
                throw rozmiar;
            }

}

kolejka::kolejka():kolejka(1){}; //bezparametrowy i delegatowy (domysla poj=1)


kolejka::kolejka(initializer_list<string> lst) //z lista inicjaliz
{
    pojemnosc=lst.size();
    ile=lst.size();
    tab=new string [pojemnosc];
    poczatek=0;
    for(int i=0; i<pojemnosc; i++)
        tab[i]=*(lst.begin()+i);
    for(int i=0; i<pojemnosc; i++)
        cerr<<tab[i];
}

kolejka::kolejka(const kolejka &wzorzec) //kopiujacy
{

    pojemnosc=wzorzec.pojemnosc;
    poczatek=wzorzec.poczatek;
    ile=wzorzec.ile;
    tab=new string[pojemnosc];
    for(int i=0; i<pojemnosc; i++)
        tab[i]=wzorzec.tab[i];

}

const kolejka kolejka::operator=(const kolejka &right)
{
    if (this!=&right)
    {
        delete[]tab;
        pojemnosc=right.pojemnosc;
        ile=right.ile;
        poczatek=right.poczatek;
        tab=new string[pojemnosc];
        for(int i=0; i<pojemnosc; i++)
            tab[i]=right.tab[i];
    }
    return *this;
}

kolejka::kolejka(kolejka&& temp)
:ile(temp.ile),poczatek(temp.poczatek),pojemnosc(temp.pojemnosc),tab(temp.tab)
{
//    ile=temp.ile;
//    poczatek=temp.poczatek;
//    pojemnosc=temp.pojemnosc;
//    tab=temp.tab;
    temp.ile=0;
    temp.poczatek=0;
    temp.pojemnosc=0;
    temp.tab=nullptr;

}

kolejka& kolejka::operator=(kolejka&& right)
{
    if(this!=&right)
    {

        swap(poczatek, right.poczatek);
        swap(ile, right.ile);
        swap(pojemnosc, right.pojemnosc);
        swap(tab, right.tab);
//delete[]tab;
////*tab=tab.right;
//ile=right.ile;
//poczatek=right.poczatek;
//pojemnosc=right.pojemnosc;
//right.poczatek=0;
//right.pojemnosc=0;
//right.tab=nullptr;

    }
    return *this;
}

void kolejka::wstaw(string s)
{
    if(ile==pojemnosc)
            {

                std::string pelna="Kolejka jest pelna";
                throw pelna;
            }
    if(poczatek+ile<=pojemnosc-1)
    {
        if(poczatek+ile==pojemnosc-1)
        {

                tab[0]=s;

        }
        else
        {

                tab[poczatek+ile]=s;


        }
        ile++;


    }
    else
    {

          tab[poczatek+ile-pojemnosc]=s;

        ile++;

    }

}

string kolejka::usun()
{
    string s;
    if (ile>0)
    {
      s=tab[poczatek];
      ile--;
      tab[poczatek]="";
      poczatek=poczatek+1;
      if(poczatek>=pojemnosc)
        poczatek=poczatek-pojemnosc;
    }
    else
    {

                std::string pusta="Kolejka jest pusta";
                throw pusta;
    }
    if(ile==0)
    {
        poczatek=0;
    }

    return s;

}
string kolejka::zprzodu()
{
    if(ile==0)
        {

                std::string pusta="Kolejka jest pusta";
                throw pusta;
        }
        return tab[poczatek];
}
int kolejka::dlugosc()
{
    return ile;
}

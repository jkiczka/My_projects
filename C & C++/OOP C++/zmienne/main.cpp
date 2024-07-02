#include <iostream>
#include "zmienne.hpp"
#include <exception>
#include <new>
using namespace std;

int main()
{   try{
    zmienna z1,z2,z3,z4;
    z1=zmienna();
    cout<<z1.get_n()<<" "<<z1.get_w()<<endl;
    z2=zmienna("ananas01_"); //poprawnosc nazwy
    z2.set_w(234);

    cout<<z2.get_n()<<" "<<z2.get_w()<<endl;


    zbior_zmiennych zbiorek=zbior_zmiennych(5);
    zbiorek.wstawienie_zmiennej(z2);
    int g=zbiorek.czy_w_zbiorze(z2.get_n());

    cout<<"Pozycja w zbiorze to: "<<g<<endl;
    zbiorek.wypisz_zbior();
    cout<<"Odczyt"<<endl;
    zbiorek.odczyt(z2);
    cout<<"Zmiana wartosci"<<endl;
    zbiorek.zmien_wartosc(z2,555.028);
    cout<<"Odczyt"<<endl;
    zbiorek.odczyt(z2);
    cout<<"Usuwanie"<<endl;
    zbiorek.usun(z2);
    zbiorek.wypisz_zbior();
    g=zbiorek.czy_w_zbiorze(z2.get_n());

    cout<<"Pozycja w zbiorze to: "<<g<<endl;
    z1=zmienna("inne_1",0.99);
    z3=zmienna("inne_2", 2.34);
    z4=zmienna("inne_3", 2.45);
    zbiorek.wstawienie_zmiennej(z1);
    zbiorek.wstawienie_zmiennej(z3);
    zbiorek.wstawienie_zmiennej(z2); //dodawanie tej samej zmiennej
    zbiorek.wypisz_zbior();

    g=zbiorek.czy_w_zbiorze(z3.get_n());
    cerr<<g;
    zbiorek.usun(z3);

    zbiorek.wypisz_zbior();




}
    catch(string w)
    {
        cerr<<w<<endl;

        return -1;
    }
    catch(std::invalid_argument& e)
    {
        cerr<<e.what()<<endl;
        return -1;
    };
    return 0;
}

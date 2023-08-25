
#include <iostream>
#include <vector>
#include "wyrazenia.hpp"
using namespace std;

int main()
{


for(int i=0; i<=10; i++)
{
    string str=to_string(i*0.1);
    int offset = 1;
    if (str.find_last_not_of('0') == str.find('.'))
        offset = 0;
    str.erase(str.find_last_not_of('0') + offset, string::npos);
    zmienna::dodaj_zmienna(str, i*0.1);
    wyrazenie *w1 = new dziel(
        new mnoz(
            new odejmij(
                new zmienna(str),
                new liczba(1)),
            new zmienna(str)),
        new liczba(2));
    cout << w1->oblicz()<<"\t\t"<< w1->zapis() << endl;
    //zmienna::usun_zmienna(str);
}

cout<<endl<<endl;

for(int i=0; i<=10; i++)
{
    string str=to_string(i*0.1);
    int offset = 1;
    if (str.find_last_not_of('0') == str.find('.'))
        offset = 0;
    str.erase(str.find_last_not_of('0') + offset, string::npos);
    //zmienna::dodaj_zmienna(str, i*0.1);
    wyrazenie *w2 = new dziel(
        new dodaj(new liczba(3), new liczba(5)),
        new dodaj(new liczba(2),
                  new mnoz(new zmienna(str), new liczba(7))));
    cout << w2->oblicz()<<"\t\t"<< w2->zapis() << endl;
    //zmienna::usun_zmienna(str);
}


cout<<endl<<endl;


for(int i=0; i<=10; i++)
{
    string str=to_string(i*0.1);
    int offset = 1;
    if (str.find_last_not_of('0') == str.find('.'))
        offset = 0;
    str.erase(str.find_last_not_of('0') + offset, string::npos);

    wyrazenie *w3 = new odejmij(
        new dodaj(
            new liczba(2),
            new mnoz(new zmienna(str), new liczba(7))),
        new dodaj(
            new mnoz(new zmienna(str), new liczba(3)),
            new liczba(5)));
    //zmienna::dodaj_zmienna(str, i*0.1);

    cout << w3->oblicz()<<"\t\t"<< w3->zapis() << endl;
    //zmienna::usun_zmienna(str);
}


cout<<endl<<endl;

for(int i=0; i<=10; i++)
{
    string str=to_string(i*0.1);
    int offset = 1;
    if (str.find_last_not_of('0') == str.find('.'))
        offset = 0;
    str.erase(str.find_last_not_of('0') + offset, string::npos);


    wyrazenie *w4 = new dziel(
        new Cos(new mnoz(
            new dodaj(new zmienna(str), new liczba(1)),
            new zmienna(str))),
        new potega(
            new potega(new e(), new zmienna(str)),
            new liczba(2))

    );


    //zmienna::dodaj_zmienna(str, i*0.1);

    cout << w4->oblicz()<<"\t\t"<< w4->zapis() << endl;
    zmienna::usun_zmienna(str);
}

    return 0;
}

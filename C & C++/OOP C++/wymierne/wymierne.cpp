#include "wymierne.hpp"
#include "wyjatki_wymierne.hpp"
#include <vector>
#include <iostream>


using namespace obliczenia;

int wymierna::NWD (int a, int b)
{
    if (b==0) return a;
    return NWD(b, a%b);
}

int wymierna::get_licznik()
{
    return licz;
}

int wymierna::get_mianownik()
{
    return mian;
}


void wymierna::normalize()
{
    int licznik  = licz;
    int mianownik = mian;
    int nwd=NWD(licznik, mianownik);

    licznik/=nwd;
    mianownik/=nwd;

    if (mianownik < 0)
    {
        licznik*=(-1);
        mianownik*=(-1);
    }

    licz=licznik;
    mian=mianownik;
}

wymierna::operator int() const
{
    return licz/mian;
}

wymierna::operator double() const
{
    return (double)licz/mian;
}

wymierna::wymierna (int licznik, int mianownik)
{
    if (mianownik==0)
        throw dzielenie_przez_zero("");

    licz=licznik;
    mian=mianownik;
    normalize();
}

wymierna::wymierna (int x)
{
    licz   = x;
    mian = 1;
}

wymierna obliczenia::operator - (wymierna a)
{
    a.licz *= (-1);
    return a;
}

wymierna obliczenia::operator ! (wymierna a)
{
    if (a.licz == 0)
        throw dzielenie_przez_zero("");

    std::swap(a.licz, a.mian);

    a.normalize();

    return a;
}

wymierna obliczenia::operator+(const wymierna &a, const wymierna &b)
{
    long long int nowy_licz=(long long int)a.licz*b.mian+(long long int)b.licz*a.mian;
    long long int nowy_mian=(long long int)a.mian*(long long int)b.mian;


    if (nowy_licz>INT_MAX or nowy_licz<INT_MIN or nowy_mian>INT_MAX or nowy_mian<INT_MIN)
        throw zakres("");

    wymierna wynik(a.licz*b.mian+b.licz*a.mian,a.mian*b.mian);
    wynik.normalize();


    return wynik;
}



wymierna obliczenia::operator-(const wymierna &a, const wymierna &b)
{
    long long int nowy_licz=(long long int)a.licz*b.mian-(long long int)b.licz*a.mian;
    long long int nowy_mian=(long long int)a.mian*(long long int)b.mian;
    if (nowy_licz>INT_MAX or nowy_licz<INT_MIN or nowy_mian>INT_MAX or nowy_mian<INT_MIN)
        throw zakres("");

    wymierna wynik(a.licz*b.mian-b.licz*a.mian,a.mian*b.mian);
    wynik.normalize();

    return wynik;
}


wymierna obliczenia::operator*(const wymierna &a, const wymierna &b)
{
    long long int nowy_licz=(long long int)a.licz*(long long int)b.licz;
    long long int nowy_mian=(long long int)a.mian*(long long int)b.mian;
    if (nowy_licz>INT_MAX or nowy_licz<INT_MIN or nowy_mian>INT_MAX or nowy_mian<INT_MIN)
        throw zakres("");

    wymierna wynik(a.licz*b.licz,a.mian*b.mian);
    wynik.normalize();

    return wynik;
}


wymierna obliczenia::operator/(const wymierna &a, const wymierna &b)
{
    if(b.licz==0)
        throw dzielenie_przez_zero("");

    long long int nowy_licz=(long long int)a.licz*(long long int)b.mian;
    long long int nowy_mian=(long long int)a.mian*(long long int)b.licz;
    if (nowy_licz>INT_MAX or nowy_licz< INT_MIN or nowy_mian>INT_MAX or nowy_mian<INT_MIN)
       throw zakres("");

    wymierna wynik(a.licz*b.mian, a.mian*b.licz);
    wynik.normalize();

    return wynik;
}



std::ostream& obliczenia::operator << (std::ostream &wyj, const wymierna &w)
{
    std::vector <int> reszty;
    std::vector <int> liczby;
    unsigned int i, j;
    int reszta;

    if (w.licz/w.mian==0 and w.licz<0) //gdy ulamek to -0 i cos po przecinku
        wyj<<"-";

    wyj<<w.licz/w.mian;
    reszta=w.licz%w.mian;

    if (reszta==0)
        return wyj;
    reszta=abs(reszta);
    reszty.push_back (reszta);

    while(true)
    {
        liczby.push_back (reszta*10/w.mian);
        reszta=reszta*10%w.mian;

        if (reszta==0) // rozwiniecie jest skonczone
        {
            wyj << ".";
            for (i=0; i<liczby.size(); i++)
                wyj<<liczby[i];
            return wyj;
        }

        for (j=0; j<reszty.size(); j++) // zaczyna sie powtarzac
        {
            if (reszta==reszty[j])
            {
                wyj << ".";
                for (i=0; i<j; i++)
                    wyj << liczby[i];

                wyj<<"(";
                for (i=j; i<liczby.size(); i++)
                    wyj<<liczby[i];
                wyj <<")";

                return wyj;
            }
        }
        reszty.push_back (reszta);
    }
}

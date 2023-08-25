#include "tabbit.hpp"
#include <iostream>
#include <cmath>

tab_bit::tab_bit (int rozm) //wyzerowana tablica bitow
{
    dl = rozm;
    int rozm_tab = ceil(static_cast<double>(dl)/rozmiarSlowa);
    tab = new slowo[rozm_tab];

    for(int i = 0; i < rozm_tab; i++)
    {
        tab[i] =0;
    }
}

tab_bit::tab_bit (slowo tb) //tablica bitow zainicjalizowana wzorcem
{
    dl = rozmiarSlowa;
    tab = new slowo[1];
    tab[0] = tb;
}

tab_bit::tab_bit (const tab_bit &tb) // konstruktor kopiuj¹cy
{
    dl = tb.dl;
    int rozm_tab = ceil(static_cast<double>(dl)/rozmiarSlowa);
    tab = new slowo[rozm_tab];
    for(int i = 0; i < rozm_tab; i++)
    {
        tab[i] = tb.tab[i];
    }
}

tab_bit::tab_bit (tab_bit &&tb) // konstruktor przenosz¹cy
{
    std::swap(dl, tb.dl);
    std::swap(tab, tb.tab);
    tb.tab = nullptr;
}


tab_bit::tab_bit(std::initializer_list<bool> list)
{
    dl=list.size();
    int k=ceil(static_cast<double>(dl)/rozmiarSlowa);
    tab = new slowo[k];
    int i = 0;
    for(auto x : list)
    {
        pisz(i,x);
        i++;
    }
}


tab_bit & tab_bit::operator = (const tab_bit &tb)  // przypisanie kopiuj¹ce
{
    dl = tb.dl;
    int tabSize = ceil(static_cast<double>(dl)/rozmiarSlowa);
    tab = new slowo[tabSize];
    for(int i = 0; i < tabSize; i++)
    {
        tab[i] = tb.tab[i];
    }
    return *this;
}

tab_bit & tab_bit::operator = (tab_bit &&tb) // przypisanie przenosz¹ce
{
    if(&tb!=this)
    {
    delete[] tab;
    dl = tb.dl;
    tab = tb.tab;
    tb.tab = nullptr;
    }
    return *this;

}

tab_bit::~tab_bit()
{
    delete[] tab;
}

std::ostream & operator << (std::ostream &wy, const tab_bit &tb)
{
    for(int i = 0 ; i<tb.dl; i++)
    {
        wy << tb.czytaj(i);
        if (i%8==7)
            wy<<" ";

    }
    return wy;
}

std::istream & operator >> (std::istream &we, tab_bit &tb)
{
    we >> tb.dl;
    int rozm_tab = ceil(static_cast<double>(tb.dl)/tb.rozmiarSlowa);
    for(int i = 0; i < rozm_tab; i++)
    {
        we >> tb.tab[i];
    }
    return we;
}



bool tab_bit::czytaj (int i) const
{
    int rozm_tab = ceil(static_cast<double>(dl)/rozmiarSlowa);
    slowo s = tab[rozm_tab-1];
    int indeks = i%rozmiarSlowa;
    bool b = s>>indeks & 1;
    return b;
}

bool tab_bit::pisz (int i, bool b)
{
    int rozm_tab = ceil(static_cast<double>(dl)/rozmiarSlowa);
    int indeks = i%rozmiarSlowa;
    //zmiana n-tego bitu: number ^= (-x ^ number) & (1 << n);
    tab[rozm_tab-1] ^= (-b ^ tab[rozm_tab-1]) & (1 << indeks);


    return b;
}

bool tab_bit::operator[] (int i) const
{
    bool b = czytaj(i);
    return b;
}

tab_bit::ref tab_bit::operator[] (int i)
{
    int indeks_bitu = i%rozmiarSlowa;
    int indeks_slowa = ceil(static_cast<double>(dl)/rozmiarSlowa) -1;
    ref wyn(tab[indeks_slowa], indeks_bitu);
    return wyn;
}



tab_bit::ref::ref(slowo& s,const int i)
{
    indeks = i;
    st = &s;
}

tab_bit::ref::ref(const ref& r)
{
    indeks = r.indeks;
    st = new slowo;
    *st = *(r.st);
}

tab_bit::ref & tab_bit::ref::operator= (const ref& r)
{
    slowo b = *(r.st) >> r.indeks & 1;
    slowo wyn = *st;
    //zmiana n-tego bitu: number ^= (-x ^ number) & (1 << n);
    wyn ^= ((-b ^ wyn) & ((slowo)1 << indeks));
    *st = wyn;
    return *this;
}

tab_bit::ref & tab_bit::ref::operator= (const bool& r)
{
    slowo b = r;
    slowo wyn = *st;
    //zmiana n-tego bitu: number ^= (-x ^ number) & (1 << n);
    wyn ^= ((-b ^ wyn) & ((slowo)1 << indeks));
    *st = wyn;
    return *this;
}


tab_bit::ref & tab_bit::ref::operator= (const int& r)
{
    bool b=(slowo)!!r;
    return *this=b;
}




tab_bit::ref::operator bool() const
{
    bool b = (*st) >> indeks & 1;
    return b;
}



tab_bit operator|(const tab_bit& tb1, const tab_bit& tb2)
{
    tab_bit wynik(tb1.dl > tb2.dl ? tb1 : tb2);
    int mini = tb1.dl > tb2.dl ? tb2.dl : tb1.dl;

    for (int i = 0; i < mini; i++)
        wynik[i] = tb1[i] | tb2[i];

    return wynik;
}

tab_bit operator&(const tab_bit& tb1, const tab_bit& tb2)
{
    tab_bit wynik(tb1.dl > tb2.dl ? tb1 : tb2);
    int mini = tb1.dl > tb2.dl ? tb2.dl : tb1.dl;

    for (int i = 0; i < mini; i++)
        wynik[i] = tb1[i] & tb2[i];

    return wynik;
}

tab_bit operator^(const tab_bit& tb1, const tab_bit& tb2)
{
    tab_bit wynik(tb1.dl > tb2.dl ? tb1 : tb2);
    int mini = tb1.dl > tb2.dl ? tb2.dl : tb1.dl;

    for (int i = 0; i < mini; i++)
        wynik[i] = tb1[i] ^ tb2[i];

    return wynik;
}

tab_bit tab_bit::operator!()
{
    int ile = ceil(static_cast<double>(dl)/rozmiarSlowa);
    tab_bit ret(dl);
    for(int i = 0; i < ile; i++)
    {
        ret.tab[i] = ~tab[i];
    }
    return ret;
}


tab_bit& tab_bit::operator|=(const tab_bit& tb)
{
    const tab_bit& wynik = (*this | tb);
    *this = std::move(wynik);

    return *this;
}


tab_bit& tab_bit::operator&=(const tab_bit& tb)
{
    const tab_bit& wynik = (*this & tb);
    *this = std::move(wynik);

    return *this;
}


tab_bit& tab_bit::operator^=(const tab_bit& tb)
{
    const tab_bit& wynik = (*this ^ tb);
    *this = std::move(wynik);

    return *this;
}


#include<iostream>

class tab_bit
{
    typedef uint64_t slowo;
    // komorka w tablicy
    static const int rozmiarSlowa = sizeof(slowo) * 8; // rozmiar slowa w bitach
    class ref // klasa pomocnicza do adresowania bitów
    {
    private:
        slowo* st;
        int indeks;
    public:
        ref(slowo& s,const int i);
        ref(const ref& r);
        ref & operator= (const ref& r);
        ref & operator= (const bool& r);
        ref & operator= (const int& r);
        operator bool() const;
    };

protected:
    int dl; // liczba bitów
    slowo *tab; // tablica bitów

public:
    explicit tab_bit (int rozm); // wyzerowana tablica bitow [0...rozm]
    explicit tab_bit (slowo tb); // tablica bitów [0...rozmiarSlowa]
    // zainicjalizowana wzorcem
    tab_bit (const tab_bit &tb); // konstruktor kopiuj¹cy
    tab_bit (tab_bit &&tb); // konstruktor przenosz¹cy
    tab_bit (std::initializer_list<bool> list);
    tab_bit & operator = (const tab_bit &tb); // przypisanie kopiuj¹ce
    tab_bit & operator = (tab_bit &&tb); // przypisanie przenosz¹ce
    ~tab_bit(); // destruktor

private:
    bool czytaj (int i) const; // metoda pomocnicza do odczytu bitu
    bool pisz (int i, bool b); // metoda pomocnicza do zapisu bitu

public:
    bool operator[] (int i) const; // indeksowanie dla sta³ych tablic bitowych
    ref operator[] (int i); // indeksowanie dla zwyk³ych tablic bitowych
    inline int rozmiar () const
    {
        return dl;
    }; // rozmiar tablicy w bitach

public:
    friend tab_bit operator& (const tab_bit& tb1, const tab_bit& tb2);
    friend tab_bit operator| (const tab_bit& tb1, const tab_bit& tb2);
    friend tab_bit operator^ (const tab_bit& tb1, const tab_bit& tb2);

    tab_bit& operator&=(const tab_bit& tb);
    tab_bit& operator|=(const tab_bit& tb);
    tab_bit& operator^=(const tab_bit& tb);

    tab_bit operator!();

public:
    friend std::istream & operator >> (std::istream &we, tab_bit &tb);
    friend std::ostream & operator << (std::ostream &wy, const tab_bit &tb);

};


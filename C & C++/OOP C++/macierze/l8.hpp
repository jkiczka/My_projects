
#include <iostream>
#include<string>
#include <initializer_list>


class wektor
{

    double *tab = nullptr; // macierz liczb zmiennopozycyjnych
    const int k; // rozmiar tablicy: k pozycji

public:
    wektor(int rozm);



    friend wektor operator-(const wektor &v); // zmiana znaku
    friend wektor operator+(const wektor &x, const wektor &y);
    friend wektor operator-(const wektor &x, const wektor &y);
    friend wektor operator*(const wektor &v, double d);
    friend wektor operator*(double d, const wektor &v);
    friend std::istream & operator >> (std::istream &we, wektor &w);
    friend std::ostream & operator << (std::ostream &wy, const wektor &w);

// iloczyn skalarny x*y
    friend double operator*(const wektor &x, const wektor &y);
    friend class macierz;
    wektor& operator+=(const wektor &v);
    wektor& operator-=(const wektor &v);
    wektor& operator*=(double d);

    wektor(const wektor &w);
    wektor& operator=(const wektor &right);
    wektor(wektor&& temp);
    wektor& operator=(wektor&& right);
    double& operator [](int i);
    double operator [] (int i) const;
    wektor(std::initializer_list<double> list);
    ~wektor();
};

class macierz
{

    wektor **tab = nullptr; // macierz wskaŸników na wektory
    const int w, k; // rozmiar tablicy: w wierszy i k kolumn
public:
    macierz(int wiersze, int kolumny);

    friend macierz operator-(const macierz &m); // zmiana znaku
    friend macierz operator+(const macierz &p, const macierz &q);
    friend macierz operator-(const macierz &p, const macierz &q);
    friend macierz operator*(const macierz &m, double d);
    friend macierz operator*(double d, const macierz &m);
    friend std::istream & operator >> (std::istream &we, macierz &m);
    friend std::ostream & operator << (std::ostream &wy, const macierz &m);


    friend macierz operator*(const macierz &p, const macierz &q);
    friend macierz operator~(const macierz &m); // transpozycja
    macierz& operator+=(const macierz &m);
    macierz& operator-=(const macierz &m);
    macierz& operator*=(double d);



    ~macierz();
    macierz(const macierz &m);
    macierz& operator=(const macierz &right);
    macierz(macierz&& temp);
    macierz& operator=(macierz&& right);
    wektor& operator [](int i);
    wektor operator [] (int i) const;
    macierz(std::initializer_list<wektor> list);




};

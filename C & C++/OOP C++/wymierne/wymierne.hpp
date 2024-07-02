#include <iostream>

namespace obliczenia
{
    class wymierna
    {
        private:
            int licz,mian;

        public:
            int get_licznik();
            int get_mianownik();

            wymierna(int l, int m);
            wymierna (int x);

            int NWD (int a, int b);
            void normalize();

            friend wymierna operator - (wymierna a);// zmiana znaku na przeciwny
            friend wymierna operator ! (wymierna);  // zamiana licznika z mianownikiem z pozostawieniem znaku liczby w liczniku
            friend wymierna operator + (const wymierna &a, const wymierna &b);
            friend wymierna operator - (const wymierna &a, const wymierna &b);
            friend wymierna operator * (const wymierna &a, const wymierna &b);
            friend wymierna operator / (const wymierna &a, const wymierna &b);

            explicit operator int() const;
            operator double() const;

            friend std::ostream& operator << (std::ostream &wyj, const wymierna &w);


    };

    wymierna operator - (wymierna a);
    wymierna operator ! (wymierna);
    wymierna operator + (const wymierna &a, const wymierna &b);
    wymierna operator - (const wymierna &a, const wymierna &b);
    wymierna operator * (const wymierna &a, const wymierna &b);
    wymierna operator / (const wymierna &a, const wymierna &b);
    std::ostream& operator << (std::ostream &wyj, const wymierna &w);


}


#include "zmienne.hpp"
#include <exception>
#include <new>
zmienna::zmienna()
{
    nazwa="_";
}
 zmienna::zmienna(string n):zmienna(n,0){};
 zmienna::zmienna(string n, double w)
 {if(n=="" or (n[0]>='0' and n[0]<='9'))
                {
                   std::string zlanazwa="Klucz powinien byc niepustym ciagiem zlozonym z liter, cyfr i znaku _ , nierozpoczynajacym sie od cyfry.\n";
                        throw zlanazwa;
               }
     else
        {
            int cyfry=0, litery=0, podkreslenia=0;
            for (int i=0; i<n.length(); i++)
            {
                if(n[i]>='0' and n[i]<='9')
                    cyfry++;
                else
                    {
                        if(n[i]=='_')
                            podkreslenia++;
                        else
                        {   if((n[i]>='a' and n[i]<='z') or (n[i]>='A' and n[i]<='Z'))
                                litery++;
                        }

                    }


            }

            if (litery+podkreslenia+cyfry!=n.length() or (litery==0) or (cyfry==0) or (podkreslenia==0) )
               {
                   std::string zlanazwa="Klucz powinien byc niepustym ciagiem zlozonym z liter, cyfr i znaku _ , nierozpoczynajacym sie od cyfry.\n";
                 throw zlanazwa;
               }
            else
            {
                    nazwa=n;
                    wartosc=w;
            }



        }

 }
 string zmienna::get_n()
 {
     return nazwa;
 }
 double zmienna::get_w()
 {
     return wartosc;
 }
 void zmienna::set_w(double x)
 {
     wartosc=x;
 }

 zbior_zmiennych::zbior_zmiennych(int k)
 :n((k>0)?k:0)
 {
        if (k<=0)
            throw std::invalid_argument("Podaj dodatni rozmiar zbioru \n");

    tab= new zmienna[k];
    //cerr<<n<<endl;
    //for(int i=0; i<k; i++)
        //tab[i]=zmienna();
//    for(int i=0; i<k; i++)
//{
//    string a=tab[i].get_n(); double b=tab[i].get_w();
//    cerr<<a<<" "<<b<<endl;
//}
 }

zbior_zmiennych:: ~zbior_zmiennych()
 {
 delete [] tab;
 //tab = NULL;
 }

void zbior_zmiennych:: wstawienie_zmiennej(zmienna z)
{
    int i=0;
    //cerr<<tab[0].get_n();
    while (tab[i].get_n()!="_" and i<n)
    {
        i++;
    }
    //cerr<<i;
    if(i==n)
        {
                std::string zbiorpelny="Zbior zmiennych jest pelny\n";
                 throw zbiorpelny;
        }
    else
    {
        if(czy_w_zbiorze(z.get_n())==(-1))
            tab[i]=z;
        else
            {
                std::string duplikat="W zbiorze istnieje juz zmienna o tej nazwie\n";
                 throw duplikat;
            }


    }
}
int zbior_zmiennych::czy_w_zbiorze(string name) //zwraca pozycje w zbiorze lub -1
{
    for (int i=0; i<n; i++)
        {
            if(tab[i].get_n()==name)
               {
                   return i;
               }
        }

        return -1;
}
void zbior_zmiennych::usun(zmienna z)
{
    int q=czy_w_zbiorze(z.get_n());
    if(q==(-1))
     {
                std::string niema="W zbiorze nie ma zmiennej o tej nazwie\n";
                 throw niema;
    }
     else
     {
        int i=0;
        string k=tab[i].get_n();
        while (k!="_" and i<n)
            {i++;
            k=tab[i].get_n();
            }
        i--;

        for(int j=q+1; j<=i; j++)
            tab[j-1]=tab[j];

        tab[i]=zmienna();

     }

}
double zbior_zmiennych::odczyt(zmienna z)
{
     int q=czy_w_zbiorze(z.get_n());

    if(q==(-1))
       {
                std::string niema="W zbiorze nie ma zmiennej o tej nazwie\n";
                 throw niema;
        }
     else
     {
        double num=tab[q].get_w();
         return num;
     }
}
void zbior_zmiennych:: zmien_wartosc(zmienna z, double x)
{
    int q=czy_w_zbiorze(z.get_n());
    if(q==(-1))
       {
                std::string niema="W zbiorze nie ma zmiennej o tej nazwie\n";
                 throw niema;
        }
     else
     {
         (tab[q]).set_w(x);
     }
}

void zbior_zmiennych::wypisz_zbior()
{
    cout<<"Elementy zbioru zmiennych to:"<<endl;
    int i=0;
    while (i<n and tab[i].get_n()!="_")
    {
        string na=tab[i].get_n();
        double wal=tab[i].get_w();
        cout<<na<<" "<<wal<<endl;
        i++;

    }
    if (i==0)
        cout<<"Zbior zmiennych jest pusty \n";
}


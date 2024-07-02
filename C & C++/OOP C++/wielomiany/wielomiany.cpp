#include "wielomiany.hpp"
#include <exception>
#include <new>
#include<string>
#include <initializer_list>
#include <iostream>
#include<algorithm>

wielomian::~wielomian()
{
    delete [] tab;
}

wielomian::wielomian (int s, double wsp)
:st((s==0)?s:1)
{
    if(s!=0)
            {
                std::string rozmiar="Stopien jednomianu musi byc zerowy";
                throw rozmiar;
            }

    tab=new double[1];
    tab[0]=wsp;


}
wielomian::wielomian (int s, const double wsp[])
:st((s>=0)?s:0)
{
    if(s>0)
    tab=new double[s+1];
    else
        {

                std::string rozmiar="Podaj nieujemny stopien wielomianu";
                throw rozmiar;
            }


    for (int i=0; i<=s; i++)
        tab[i]=wsp[i];
    if(tab[st]==0 and st!=0)
    {

                std::string rozmiar="Wspolczynnik przy najwyzszej potedze nie moze byc zerowy";
                throw rozmiar;
    }

}

wielomian::wielomian (int s)
:st((s>=0)?s:0)
{
    if(s>0)
    tab=new double[s+1];
    else
        {

                std::string rozmiar="Podaj nieujemny stopien wielomianu";
                throw rozmiar;
            }
    tab[s]=1;

}

wielomian::wielomian(initializer_list<double> lst) //z lista inicjaliz
{
    st=lst.size()-1;
    tab=new double [lst.size()];
    for(int i=0; i<lst.size(); i++)
        tab[i]=*(lst.begin()+i);

    if(tab[st]==0 and st!=0)
    {

                std::string rozmiar="Wspolczynnik przy najwyzszej potedze nie moze byc zerowy";
                throw rozmiar;
    }
//    for(int i=0; i<lst.size(); i++)
//        cerr<<tab[i];
}

wielomian::wielomian(const wielomian &w) //kopiujacy
{

    st=w.st;
    tab=new double[st+1];
    for(int i=0; i<=st; i++)
        tab[i]=w.tab[i];

}

 wielomian& wielomian::operator=(const wielomian &right) // przypisanie kopiuj¹ce
{
    if (this!=&right)
    {
        delete[]tab;
        st=right.st;
        tab=new double[st+1];
        for(int i=0; i<=st; i++)
            tab[i]=right.tab[i];

    }
    return *this;
}

wielomian::wielomian(wielomian&& temp)
:st(temp.st),tab(temp.tab)
{

//    temp.ile=0;
//    temp.poczatek=0;
//    temp.pojemnosc=0;
    temp.tab=nullptr;

}

wielomian& wielomian::operator=(wielomian&& right)
{
    if(this!=&right)
    {

        swap(st, right.st);
        swap(tab, right.tab);
    }
    return *this;
}

double& wielomian::wielomian::operator [](int i)
{
    if(i>st)
    {
        std::string zakres="Nie istnieje wspolczynnik o numerze wiekszym niz stopien wielomianu";
        throw zakres;
    }

if(tab[st]==0 and st!=0)
    {

                std::string rozmiar="Wspolczynnik przy najwyzszej potedze nie moze byc zerowy";
                throw rozmiar;
    }
    return tab[i];


}

double wielomian::wielomian::operator [] (int i) const
{
    return tab[i];
}

double wielomian::wielomian::operator () (double x) const
{
    double wartosc=0;

    for (int i=st; i>0; i--)
        wartosc=(wartosc+tab[i])*x;
    return wartosc+tab[0];
}

int wielomian::wielomian::stopien()
{
    return st;

}

wielomian operator + (const wielomian &u, const wielomian &v)
 {
     wielomian w;
    if(u.st>v.st)
    {
        double x[u.st+1];
        for (int i=0; i<=v.st; i++)
            x[i]=u.tab[i]+v.tab[i];
        for(int i=v.st+1; i<=u.st; i++)
            x[i]=u.tab[i];
        int stopa=u.st;
        while (x[stopa]==0)
            stopa--;
        w=wielomian(stopa,x);
    }
    else
    {
        double x[v.st+1];
        for (int i=0; i<=u.st; i++)
            x[i]=u.tab[i]+v.tab[i];
        for(int i=u.st+1; i<=v.st; i++)
            x[i]=v.tab[i];
        int stopa=v.st;
        while (x[stopa]==0)
            stopa--;
        w=wielomian(stopa,x);
    }


    return w;

 }

 wielomian operator - (const wielomian &u, const wielomian &v)
 {
      wielomian w;
    if(u.st>v.st)
    {
        double x[u.st+1];
        for (int i=0; i<=v.st; i++)
            x[i]=u.tab[i]-v.tab[i];
        for(int i=v.st+1; i<=u.st; i++)
            x[i]=u.tab[i];
        int stopa=u.st;
        while (x[stopa]==0)
            stopa--;
        w=wielomian(stopa,x);
    }
    else
    {
        double x[v.st+1];
        for (int i=0; i<=u.st; i++)
            x[i]=u.tab[i]-v.tab[i];
        for(int i=u.st+1; i<=v.st; i++)
            x[i]=-v.tab[i];
        int stopa=v.st;
        while (x[stopa]==0)
            stopa--;
        w=wielomian(stopa,x);
    }


    return w;

 }

wielomian operator * (const wielomian &u, const wielomian &v)
{
    if(u.st==0 and v.st==0)
        {
            int z=u[0]*v[0];
            wielomian w=wielomian(0,z);
            return w;
            }
    int k=u.st+v.st;
    double tabela[k+1];
    for(int i=0; i<=k; i++)
        tabela[i]=0;
    for (int i=0; i<=u.st; i++)
        for(int j=0; j<=v.st; j++)
            tabela[i+j]=tabela[i+j]+u.tab[i]*v.tab[j];
    wielomian w=wielomian(k, tabela);

    return w;
}

wielomian operator * (const wielomian &u, double c)
{
    wielomian w=u;
    for (int i=0; i<=w.stopien(); i++)
        {double z=u.tab[i];
            w.tab[i]=z*c;}
return w;
}




istream& operator >> (istream &we, wielomian &w)
{
    cerr<<"Stopien: ";
    cerr<<w.st<<endl;

    for(int i = 0; i <= w.st; i++)
    {
        cout<<"Wspolczynnik przy X^"<<i<<"=";
        we>>w.tab[i];
    }
    if(w.tab[w.st]==0 and w.st!=0)
    {

                std::string rozmiar="Wspolczynnik przy najwyzszej potedze nie moze byc zerowy";
                throw rozmiar;
    }
    return we;
}
ostream& operator << (ostream &wy, const wielomian &w)
{


    for (int i =w.st; i>0; i--)
    {if(w.tab[i]>0)
       wy << w.tab[i] << "*X^"<<i<<" + ";
    if(w.tab[i]<0)
        wy <<"("<< w.tab[i]<<")" << "*X^"<<i<<" + ";}
       wy <<w.tab[0]<<"\n";
    return wy;
}

wielomian& wielomian::wielomian::operator += (const wielomian &v)
{

    wielomian w;
    if(st>v.st)
    {
        double* x=new double[st+1];
        for (int i=0; i<=v.st; i++)
            x[i]=this->tab[i]+v.tab[i];
        for(int i=v.st+1; i<=st; i++)
            x[i]=this->tab[i];
        int stopa=st;
        while (x[stopa]==0)
            stopa--;
        st=stopa;
        tab=x;
    }
    else
    {
          double* x=new double[v.st+1];
        for (int i=0; i<=st; i++)
            x[i]=this->tab[i]+v.tab[i];
        for(int i=st+1; i<=v.st; i++)
            x[i]=v.tab[i];
        int stopa=v.st;
        while (x[stopa]==0)
            stopa--;
        st=stopa;
        tab=x;
    }

   return *this;
}
 wielomian& wielomian::wielomian::operator -= (const wielomian &v)
 {
 wielomian w;
    if(st>v.st)
    {
        double* x=new double[st+1];
        for (int i=0; i<=v.st; i++)
            x[i]=this->tab[i]-v.tab[i];
        for(int i=v.st+1; i<=st; i++)
            x[i]=this->tab[i];
        int stopa=st;
        while (x[stopa]==0)
            stopa--;
        st=stopa;
        tab=x;
    }
    else
    {
          double* x=new double[v.st+1];
        for (int i=0; i<=st; i++)
            x[i]=this->tab[i]-v.tab[i];
        for(int i=st+1; i<=v.st; i++)
            x[i]=-v.tab[i];
        int stopa=v.st;
        while (x[stopa]==0)
            stopa--;
        st=stopa;
        tab=x;
    }
    return *this;
 }
 wielomian& wielomian::wielomian::operator *= (const wielomian &v)
 {
if(st==0 and v.st==0)
        {
            this->tab[0]=this->tab[0]*v.tab[0];
            return *this;
            }
    int k=st+v.st;
    double*tabela=new double[k+1];
    for(int i=0; i<=k; i++)
        tabela[i]=0;
    for (int i=0; i<=st; i++)
        for(int j=0; j<=v.st; j++)
            tabela[i+j]=tabela[i+j]+this->tab[i]*v.tab[j];
    tab=tabela;
    st=k;

    return *this;
 }
 wielomian& wielomian::wielomian::operator *= (double c)
 {

    for (int i=0; i<=st; i++)
        {
            this->tab[i]=this->tab[i]*c;
        }
    return *this;
 }

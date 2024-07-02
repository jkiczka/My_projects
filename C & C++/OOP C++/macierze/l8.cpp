#include "l8.hpp"
#include "wyjatki_macierze.hpp"
#include <iostream>
#include<stdio.h>
#include <string>

wektor::wektor(int rozm)
    :k((rozm>=0)?rozm:0)
{
    if(k>0)
        tab=new double[k+1] {};
    else
    {
        throw Dodatni_rozmiar("");
    }

}

wektor operator-(const wektor &v)
{
    wektor wynik=wektor(v.k);
    wynik*(-1.0);
    return wynik;
}

wektor operator+(const wektor &x, const wektor &y)
{
    wektor wynik=wektor(x.k);
    if (x.k==y.k)
    {
        for (int i=0; i<x.k; i++)
            wynik.tab[i]=x.tab[i]+y.tab[i];
    }
    else
    {
        throw Niewlasciwe_wymiary(x.k,y.k, "");
    }



    return wynik;
}

wektor operator-(const wektor &x, const wektor &y)
{
    wektor wynik=wektor(x.k);
    if (x.k==y.k)
    {

        for (int i=0; i<x.k; i++)
            wynik.tab[i]=x.tab[i]-y.tab[i];
    }
    else
    {

        throw Niewlasciwe_wymiary(x.k,y.k, "");
    }



    return wynik;

}
wektor operator*(const wektor &v, double d)
{
    wektor wynik(v.k);

    for (int i=0; i<v.k; i++)
        wynik.tab[i]=d*v.tab[i];

    return wynik;
}
wektor operator*(double d, const wektor &v)
{
    return v*d;
}

double operator*(const wektor &x, const wektor &y)
{
    double wynik=0;
    if (x.k==y.k)
    {

        for (int i=0; i<x.k; i++)
            wynik=wynik+x.tab[i]*y.tab[i];
    }
    else
    {
        throw Niewlasciwe_wymiary(x.k,y.k, "");
    }

    return wynik;
}





std::istream & operator >> (std::istream &we, wektor &w)
{
    std::cerr<<"Rozmiar wektora"<<std::endl;
    std::cerr<<w.k<<std::endl;
    for (int i=0; i<w.k; i++)
    {
        we>>w.tab[i];
    }

    return we;



}
std::ostream & operator << (std::ostream &wy, const wektor &w)
{
    wy<<"(";
    for (int i=0; i<w.k; i++)
    {

        wy<<w.tab[i];
        if(i!=w.k-1)
            wy<<" ";
    }
    wy<<")";
    return wy;
}

wektor::~wektor()
{
    delete []tab;
}



wektor::wektor(const wektor &w)
    :k(w.k)
{

    tab=new double[k];
    for(int i=0; i<k; i++)
        tab[i]=w.tab[i];

}

wektor& wektor::operator=(const wektor &right)
{
    if (this!=&right)
    {

        if(k!=right.k)
        {
            throw Niewlasciwe_wymiary(k,right.k, "");
        }

        for(int i=0; i<k; i++)
            tab[i]=right.tab[i];

    }
    return *this;
}

wektor::wektor(wektor&& temp)
    :tab(temp.tab),k(temp.k)
{

    temp.tab=nullptr;

}

wektor& wektor::operator=(wektor&& right)
{
    if(this!=&right)
    {

        if (k!=right.k)
        {
            throw Niewlasciwe_wymiary(k,right.k, "");
        }
        else
            std::swap(tab, right.tab);
    }
    return *this;
}

double& wektor::wektor::operator [](int i)
{
    if(i>=k)
    {


        throw Index(k,i,"");
    }


    return tab[i];


}

double wektor::wektor::operator [] (int i) const
{
    if(i>=k)
    {

        throw Index(k,i, "");
    }


    return tab[i];
}

wektor::wektor(std::initializer_list<double> list)
    :k(list.size())
{

    tab = new double[k];
    int i = 0;
    for(auto x : list)
    {
        tab[i]=x;
        i++;
    }
}


wektor& wektor::operator+=(const wektor &v)
{
    const wektor& wynik=(*this+v);
    *this=std::move(wynik);
    return *this;

}
wektor& wektor::operator-=(const wektor &v)
{
    const wektor& wynik=(*this-v);
    *this=std::move(wynik);
    return *this;
}
wektor& wektor::operator*=(double d)
{
    const wektor& wynik=(*this*d);
    *this=std::move(wynik);
    return *this;
}


macierz::macierz(int wiersze, int kolumny)

    :w((wiersze>=0)?wiersze:0),
     k((kolumny>=0)?kolumny:0)
{
    if(k>0 and w>0)
    {


        tab=new wektor*[w];
        for (int i=0; i<w; i++)
            tab[i]=new wektor(k);
    }
    else
    {
        throw Dodatni_rozmiar("");
    }

}


macierz operator-(const macierz &m) // zmiana znaku
{

    macierz wynik=macierz(m.w,m.k);
    wynik*(-1.0);
    return wynik;

}
macierz operator+(const macierz &p, const macierz &q)
{
    macierz wynik=macierz(p.w, p.k);
    if (p.w==q.w and p.k==q.k)
    {
        for (int i=0; i<p.w; i++)
            *(wynik.tab[i])=*(p.tab[i])+*(q.tab[i]);
    }
    else
    {
        throw Niewlasciwe_wymiary2(p.k,q.k,p.w,q.w, "");
    }
    return wynik;
}
macierz operator-(const macierz &p, const macierz &q)
{
    macierz wynik=macierz(p.w, p.k);
    if (p.w==q.w and p.k==q.k)
    {
        for (int i=0; i<p.w; i++)
            *(wynik.tab[i])=*(p.tab[i])-*(q.tab[i]);
    }
    else
    {
        throw Niewlasciwe_wymiary2(p.k,q.k,p.w,q.w, "");
    }
    return wynik;
}
macierz operator*(const macierz &m, double d)
{
    macierz wynik=macierz(m.w, m.k);


    for (int i=0; i<m.w; i++)
        *(wynik.tab[i])=*(m.tab[i])*d;

    return wynik;
}
macierz operator*(double d, const macierz &m)
{
    return m*d;

}
std::istream & operator >> (std::istream &we, macierz &m)
{
    for(int i=0; i<m.w; i++)
        for (int j=0; j<m.k; j++)
            we>>m.tab[i][j];

    return we;
}
std::ostream & operator << (std::ostream &wy, const macierz &m)
{

    for(int i=0; i<m.w; i++)
    {
        wy<<"|";

        wy<<m.tab[i][0];


        wy<<"| \n";


    }

    return wy;
}


macierz operator*(const macierz &p, const macierz &q)
{
    macierz wynik=macierz(p.w, q.k);

    if(p.k==q.w)
    {

        for (int i=0; i<p.w; i++)
        {
            for (int j=0; j<q.k; j++)
            {
                double wyn=0;
                for (int l=0; l<p.k; l++)
                    wyn=wyn+p[i][l]*q[l][j];
                wynik[i][j]=wyn;
            }
        }

    }
    else
    {
        throw Niewlasciwe_wymiary2(p.k,q.k,p.w,q.w, "");
    }

    return wynik;

}
macierz operator~(const macierz &m) // transpozycja
{
    macierz wynik=macierz(m.k,m.w);
    if (m.k!=m.w)
    {
        throw Niekwadratowa("");
    }
    for (int i=0; i<m.k; i++)
        for (int j=0; j<m.w; j++)
        {
            wynik[i][j]=m[j][i];
        }

    return wynik;
}



macierz::~macierz()
{
    for (int i=0; i<this->w; i++)
        delete tab[i];

    delete []tab;
}



macierz::macierz(const macierz &m)
    : w(m.w),k(m.k)
{
    if(k<=0 or w<=0)
    {
        throw Niewlasciwe_wymiary2(k,m.k,w,m.w, "");
    }
    else
    {

        tab=new wektor*[w];
        for (int i=0; i<w; i++)
            tab[i]=new wektor(k);
        for (int i=0; i<w; i++)
            tab[i]=m.tab[i];
    }

}

macierz& macierz::operator=(const macierz &right)
{
    if (this!=&right)
    {
        if(k!=right.k or w!=right.w)
        {
            throw Niewlasciwe_wymiary2(k,right.k,w,right.w, "");
        }


        for (int i=0; i<w; i++)
            tab[i]=right.tab[i];

    }
    return *this;
}

macierz::macierz(macierz&& temp)
    :tab(temp.tab), w(temp.w),k(temp.k)
{

    temp.tab=nullptr;

}

macierz& macierz::operator=(macierz&& right)
{
    if(this!=&right)
    {
//    this->k=right.k;
        if (k!=right.k or w!=right.w)
        {
            throw Niewlasciwe_wymiary2(k,right.k,w,right.w, "");
        }
        else
            std::swap(tab, right.tab);
    }
    return *this;
}

wektor& macierz::macierz::operator [](int i)
{
    if(i>=w)
    {

        throw Index(w,i, "");
    }


    return *tab[i];


}

wektor macierz::macierz::operator [] (int i) const
{
    if(i>=w)
    {

        Index(w,i, "");
    }


    return *tab[i];
}


macierz::macierz(std::initializer_list<wektor> list)
    :w(list.size()), k((list.begin())->k)
{
    tab=new wektor*[w];

    int i = 0;
    for(auto x : list)
    {
        if(x.k!=k)
        {
            throw LI_dl("");
        }
        tab[i]=new wektor(x);
        i++;

    }
}


macierz& macierz::operator+=(const macierz &m)
{
    const macierz& wynik=(*this+m);
    *this=std::move(wynik);
    return *this;
}
macierz& macierz::operator-=(const macierz &m)
{
    const macierz& wynik=(*this-m);
    *this=std::move(wynik);
    return *this;
}
macierz& macierz::operator*=(double d)
{
    const macierz& wynik=(*this*d);
    *this=std::move(wynik);
    return *this;
}



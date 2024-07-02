#include <iostream>
#include "kolory.hpp"
#include <iomanip>
using namespace std;

int main()
{
    try
    {



        kolor k1,k2,k3;
        k1=kolor(100,20,30);
        k2=kolor (50,50,50);
        k3=kolor(20,30,20);

        cerr<<"KOLOR:"<<k1;
        cerr<<"PRZYCIEMNIJ o 0.5:"<<endl;
        k1.ciemniejszy(0.5);
        cout<<k1;
        cerr<<"ROZJASNIJ o 0.25:"<<endl;
        k1.jasniejszy(0.25);
        cout<<k1;
        cerr<<"KOLOR 2:"<<k2;
        cerr<<"KOLOR 3:"<<k3;
        cerr<<"SUMA KOLOROW:"<<kolor::suma_kolorow_f()<<endl;

        kolortransparentny kt;
        kt=kolortransparentny(10,210,30,40);
        cerr<<"KOLOR TRANSPARENTNY:"<<kt;

        kolornazwany kn;
        kn=kolornazwany(5,10,15,20,"szaroniebieski");
        cerr<<"KOLOR NAZWANY:"<<kn;
        punkt p1;
        p1=punkt(1,1);
        cerr<<"PUNKT 1:"<<p1;
        punkt p2;
        p2=punkt(2,2);

        cerr<<"PUNKT 2:"<<p2;

        cerr<<"ODLEGLOSC P1, P2: "<<p1.odleglosc(p2)<<endl;
        punkt p3;
        p3=punkt(3,3);
        cerr<<"PUNKT 3:"<<p3;

        cerr<<"WSPOLLINIOWE P1, P2, P3: "<<punkt::wspolliniowe(p2,p1,p3)<<endl;

        punktnazwany pn;
        pn=punktnazwany(3,3,"punkcik");

        cerr<<"PUNKT NAZWANY:"<<pn;

        punktkolorowy pk=punktkolorowy(1,3,kt);
        cerr<<"PUNKT KOLOROWY (oparty na transparentnym):"<<pk;

        punktkolorowynazwany pkn=punktkolorowynazwany(1.0,2.3,"nazywam",3,6,4,5);
        cerr<<"PUNKT KOLOROWY NAZWANY:"<<pkn;
        punkt2d p12d=punkt2d(1,2);
        wektor2d w1=wektor2d(3,4);
        cerr<<"PUNKT 2D:"<<p12d;
        p12d.transpozycja(w1);
        cerr<<"przesuniety o wektor:"<<w1<<"to: "<<p12d;
        punkt3d p13d=punkt3d(1,2,3);
        wektor3d w3=wektor3d(5,-1,4);
        cerr<<"PUNKT 3D:"<<p13d;
        p13d.transpozycja(w3);
        cerr<<"przesuniety o wektor:"<<w3<<"to: "<<p13d;
        punkt3d de1=punkt3d(2,4,6);
        punkt3d de2=punkt3d(0.25,0.5,0.75);
        punkt3d de3=punkt3d(-1,-2,-3);
        cerr<<"PUNKTY 3D:"<<endl<<de1<<de2<<de3;

        cerr<<"WSPOLLINIOWE P1, P2, P3: "<<punkt::wspolliniowe(p2,p1,p3)<<endl;
    }


    catch(string w)
    {
        cerr<<w<<endl;

        return -1;
    }
    return 0;
}

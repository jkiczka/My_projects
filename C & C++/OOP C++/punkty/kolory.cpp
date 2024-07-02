#include "kolory.hpp"
#include<algorithm>
#include <stdio.h>
#include <math.h>


kolor::kolor ()
{
    r=g=b=0;
}


kolor::kolor(int x, int y, int z)
{
    if (((x>255 or x<0) or (y>255 or y<0) )or (z>255 or z<0))
    {
        std::string zakres="Zakres koloru to 0-255";
        throw zakres;
    }
    else
    {
        r=x;
        g=y;
        b=z;
    }


    suma_kolorow+=(r+g+b)/3;
}


int kolor::get_r () const
{
    return r;
};
int kolor::get_g () const
{
    return g;
};
int kolor::get_b () const
{
    return b;
};

void kolor::set_r (int x)
{
    if (x>255 or x<0)
    {
        std::string zakres="Zakres koloru to 0-255";
        throw zakres;
    }
    r=x;
};
void kolor::set_g (int x)
{
    if (x>255 or x<0)
    {
        std::string zakres="Zakres koloru to 0-255";
        throw zakres;
    }
    g=x;
};
void kolor::set_b (int x)
{
    if (x>255 or x<0)
    {
        std::string zakres="Zakres koloru to 0-255";
        throw zakres;
    }
    b=x;
};



ostream& operator << (ostream &wy, const kolor &k)
{
    return wy << "(" << k.r << ", " << k.g <<", "<< k.b <<")"<<endl;
}

ostream& operator << (ostream &wy, const kolortransparentny &k)
{
    return wy << "(" << k.r << ", " << k.g <<", "<< k.b <<", "<< k.alfa <<")"<<endl;
}
ostream& operator << (ostream &wy, const kolornazwany &k)
{
    return wy << "(" << k.r << ", " << k.g <<", "<< k.b <<", "<< k.alfa <<", "<< k.nazwa <<")"<<endl;
}

ostream& operator << (ostream &wy, const punkt &p)
{
    return wy << "(" << p.x << ", " << p.y<<")"<<endl;
}


ostream& operator << (ostream &wy, const punktnazwany &p)
{
    return wy << "(" << p.x << ", " << p.y<<", " << p.nazwa<<")"<<endl;
}

ostream& operator << (ostream &wy, const punktkolorowy &p)
{
    return wy << "(" << p.x << ", " << p.y<<", " << p.r<<", " << p.g<<", " << p.b<<", " << p.alfa<<")"<<endl;
}

ostream& operator << (ostream &wy, const punktkolorowynazwany &p)
{
    return wy << "(" << p.x << ", " << p.y<<", " << p.r<<", " << p.g<<", " << p.b<<", " << p.alfa<<", "<<p.nazwa<<")"<<endl;
}

ostream& operator << (ostream &wy, const punkt2d &p)
{
    return wy << "(" << p.x << ", " << p.y<<")"<<endl;
}




ostream& operator << (ostream &wy, const wektor2d &p)
{
    return wy << "[" << p.dx << ", " << p.dy<<"]"<<endl;
}



ostream& operator << (ostream &wy, const wektor3d &p)
{
    return wy << "[" << p.dx << ", " << p.dy<<", " << p.dz<<"]"<<endl;
}


ostream& operator << (ostream &wy, const punkt3d &p)
{
    return wy << "(" << p.x << ", " << p.y<<", " << p.z<<")"<<endl;
}

void kolor::jasniejszy (double q)
{
    if(q>1 or q<0)
    {
        std::string zakres="Podaj wspolczynnik z zakresu 0-1";
        throw zakres;
    }
    else
    {
        r += ((255 - r)*q);
        g += ((255 - g)*q);
        b += ((255 - b)*q);
    }

};
void kolor::ciemniejszy(double q)
{
    if(q>1 or q<0)
    {
        std::string zakres="Podaj wspolczynnik z zakresu 0-1";
        throw zakres;
    }
    else
    {
        r *=(1 - q);
        g*=(1 - q);
        b*=(1 - q);
    }
};


int kolor::suma_kolorow=0;

int kolor::suma_kolorow_f()
{
    return kolor::suma_kolorow;
}

kolortransparentny::kolortransparentny():kolor()
{
    alfa=0;
}

kolortransparentny::kolortransparentny(int x, int y, int z, int p):kolor(x,y,z)
{
    if(p<0 or p>255)
    {
        std::string zakres="Zakres przezroczystosci to: 0-255";
        throw zakres;
    }

    else
        alfa=p;
}



int kolortransparentny::get_alfa()
{
    return alfa;
}

void kolortransparentny::set_alfa(int p)
{
    if(p<0 or p>255)
    {
        std::string zakres="Zakres przezroczystosci to: 0-255";
        throw zakres;
    }

    else
        alfa=p;
}

kolornazwany::kolornazwany():kolortransparentny()
{
    nazwa="";
}

kolornazwany::kolornazwany(int x, int y, int z, int p, string n):kolor(x,y,z),kolortransparentny(x,y,z,p)
{
    for (int i = 0; i <n.length(); i++)
        if(!isalpha(n[i]))
        {
            std::string zakres="Nazwa koloru moze sie skladac tylko z liter";
            throw zakres;
        }


    nazwa=n;
}


punkt::punkt(double a, double b)
{
    x=a, y=b;
}


double punkt::odleglosc(punkt p2)
{
    return sqrt(pow((x-p2.x),2)+pow((y-p2.y),2));
}

bool punkt::wspolliniowe(punkt a,punkt b,punkt c)
{
    int f = a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
    if(f==0)
        return 1;
    else
        return 0;

}

punktnazwany::punktnazwany(double a, double b, string n): punkt(a,b)
{
    nazwa=n;

}

punktkolorowy::punktkolorowy(double xp, double yp, kolortransparentny kolorek):punkt(xp,yp),kolor(kolorek.get_r(),kolorek.get_g(), kolorek.get_b()),kolortransparentny(kolorek.get_r(),kolorek.get_g(), kolorek.get_b(),kolorek.get_alfa())
{;}

punktkolorowy::punktkolorowy(double xp, double yp, double rp, double gp, double bp, double ap):punkt(xp,yp),kolor(rp,gp,bp),kolortransparentny(rp,gp,bp,ap)
{}




punktkolorowynazwany::punktkolorowynazwany(double xp, double yp, string name, int rq, int gq, int bq,int aq):kolor(rq,gq,bq),punkt(xp,yp),kolortransparentny(rq,gq,bq,aq),punktkolorowy(xp,yp,rq,gq,bq,aq),punktnazwany(xp,yp,name)
{
}



wektor2d::wektor2d(double deltax, double deltay)
{
    dx=deltax;
    dy=deltay;
}

punkt2d::punkt2d(double a, double b):punkt(a,b)
{

}

void punkt2d::transpozycja(wektor2d w)
{
    x+=w.dx;
    y+=w.dy;
}

punkt3d::punkt3d(double a, double b,double c):punkt2d(a,b)
{
    z=c;
}

wektor3d::wektor3d(double a, double b,double c):wektor2d(a,b)
{
    dz=c;
}

void punkt3d::transpozycja(wektor3d w)
{
    x+=w.dx;
    y+=w.dy;
    z+=w.dz;
}


bool punkt3d::wspolliniowe(punkt3d a,punkt3d b,punkt3d c)
{
    punkt3d ab=punkt3d(b.x-a.x,b.y-a.y, b.z-a.z);
    punkt3d ac=punkt3d(c.x-a.z,c.y-a.y,c.z-a.z);
    if(ab.x/ac.x==ab.y/ac.y==ab.z/ac.z)
        return 1;
    else
        return 0;

}

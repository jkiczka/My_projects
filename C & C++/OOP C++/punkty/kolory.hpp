#include <exception>
#include <new>
#include<string>
#include <initializer_list>
#include <iostream>
#include<algorithm>
using namespace std;


class kolor
{
protected:
    int r,g,b;
    static int suma_kolorow;
public:
    kolor();
    kolor(int x, int y, int z);
public:
    int get_r () const;
    int get_g () const;
    int get_b () const;
    void set_r (int x);
    void set_g (int x);
    void set_b (int x);
    void jasniejszy(double q);
    void ciemniejszy(double q);
    static int suma_kolorow_f();

public:
    friend ostream& operator << (ostream &wy, const kolor &k);


};


class kolortransparentny:virtual public   kolor
{

protected:
    int alfa;
public:
    kolortransparentny();
    kolortransparentny(int x, int y, int z, int p);
    int get_alfa();
    void set_alfa(int p);
public:
    friend ostream& operator << (ostream &wy, const kolortransparentny &k);
};

class kolornazwany:public kolortransparentny
{

protected:
    string nazwa;
public:
    kolornazwany();
    kolornazwany(int x, int y, int z, int p, string n);
public:
    friend ostream& operator << (ostream &wy, const kolornazwany &k);

};

class punkt
{
protected:
    double x,y;
public:
    punkt()=default;
    punkt(double a, double b);
    double odleglosc(punkt p2);
    static bool wspolliniowe (punkt a, punkt b, punkt c);
public:
    friend ostream& operator << (ostream &wy, const punkt &p);


};

class punktnazwany:public virtual punkt
{
protected:
    string nazwa;
public:
    punktnazwany()=default;
    punktnazwany(double a, double b, string n);
public:
    friend ostream& operator << (ostream &wy, const punktnazwany &p);

};

class punktkolorowy:virtual public punkt,virtual public  kolortransparentny ///////????????????????????????????????????????????????????????
{
public:
    punktkolorowy();
    punktkolorowy(double xp, double yp, kolortransparentny kolorek );
    punktkolorowy(double xp, double yp, double rp, double gp, double bp, double ap);
public:
    friend ostream& operator << (ostream &wy, const punktkolorowy &p);
};

class punktkolorowynazwany: public punktkolorowy, public  punktnazwany
{
public:
    punktkolorowynazwany();
    punktkolorowynazwany(double xp, double yp, string name, int rq, int gq, int bq,int aq);

    friend ostream& operator << (ostream &wy, const punktkolorowynazwany &p);
};

class wektor2d
{
protected:
    double dx,dy;
public:
    wektor2d()=default;
    wektor2d(double deltax, double deltay);
    friend ostream& operator << (ostream &wy, const wektor2d &p);
    friend class punkt2d;
};

class punkt2d:public punkt
{
public:
    punkt2d()=default;
    punkt2d(double a, double b);
    void transpozycja(wektor2d w);
public:
    friend ostream& operator << (ostream &wy, const punkt2d &p);


};

class wektor3d:public  wektor2d
{
protected:
    double dz;
public:
    wektor3d()=default;
    wektor3d(double a, double b, double c);
    friend ostream& operator << (ostream &wy, const wektor3d &p);
    friend class punkt3d;
};

class punkt3d:public punkt2d
{
protected:
    double z;
public:
    punkt3d(double a,double b,double c);
    void transpozycja(wektor3d w);

    double odleglosc(punkt3d p);
    friend ostream& operator << (ostream &wy, const punkt3d &p);
    static bool wspolliniowe (punkt3d a, punkt3d b, punkt3d c);
};



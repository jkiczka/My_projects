#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
class wyrazenie
{
public:
    virtual double oblicz() = 0;
    virtual string zapis() = 0;

    virtual int priorytet()
    {
        return 9;
    }

    virtual bool lewostronny()
    {
        return true;
    }

    virtual ~wyrazenie() {};
};

class liczba : public wyrazenie
{
public:
    double value;

    liczba (double value);

    string zapis();
    double oblicz();
};

class zmienna : public wyrazenie
{
private:
    static vector<pair<string, double>> zmienne;

public:
    static void dodaj_zmienna(string var, double val);
    static void wypisz_zmienna();
    static void usun_zmienna(string var);

    string zmienna_name;

    zmienna(string var);

    string zapis();
    double oblicz();
};

class stala : public wyrazenie
{
protected:
    double value;
};
class e : public stala
{
public:
    string zapis();
    double oblicz();
    e();
};


class fi : public stala
{
public:
    string zapis();
    double oblicz();
    fi();
};


class pi : public stala
{
public:
    string zapis();
    double oblicz();
    pi();
};


class operator1arg : public wyrazenie
{
public:
    wyrazenie *arg1;
    operator1arg(wyrazenie *arg1);
};

class odwrot : public operator1arg
{
public:
    string zapis();
    double oblicz();
    odwrot(wyrazenie *arg1);
};


class przeciw : public operator1arg
{
public:
    string zapis();
    double oblicz();
    przeciw(wyrazenie *arg1);
};


class Abs : public operator1arg
{
public:
    string zapis();
    double oblicz();
    Abs(wyrazenie *arg1);
};

class Exp : public operator1arg
{
public:
    string zapis();
    double oblicz();
    Exp(wyrazenie *arg1);
};



class Sin : public operator1arg
{
public:
    string zapis();
    double oblicz();
    Sin(wyrazenie *arg1);
};

class Cos : public operator1arg
{
public:
    string zapis();
    double oblicz();
    Cos(wyrazenie *arg1);
};

class Ln : public operator1arg
{
public:
    string zapis();
    double oblicz();
    Ln(wyrazenie *arg1);
};

class operator2arg : public operator1arg
{
public:
    int priorytet() override
    {
        return 1;
    }

    wyrazenie *arg2;
    operator2arg(wyrazenie *arg1, wyrazenie *arg2);
    virtual bool lewostronny() = 0;
};

class dodaj : public operator2arg
{
public:
    string zapis();
    double oblicz();

    bool lewostronny() override
    {
        return false;
    }

    dodaj(wyrazenie *arg1, wyrazenie *arg2) : operator2arg(arg1, arg2) {}
};

class odejmij : public operator2arg
{
public:
    string zapis();
    double oblicz();

    bool lewostronny() override
    {
        return true;
    }

    odejmij(wyrazenie *arg1, wyrazenie *arg2) : operator2arg(arg1, arg2) {}
};

class mnoz : public operator2arg
{
public:
    string zapis();
    double oblicz();

    int priorytet() override
    {
        return 2;
    }

    bool lewostronny() override
    {
        return false;
    }

    mnoz(wyrazenie *arg1, wyrazenie *arg2) : operator2arg(arg1, arg2) {}
};

class dziel : public operator2arg
{
public:
    string zapis();
    double oblicz();

    int priorytet() override
    {
        return 2;
    }

    bool lewostronny() override
    {
        return true;
    }

    dziel(wyrazenie *arg1, wyrazenie *arg2) : operator2arg(arg1, arg2) {}
};

class potega : public operator2arg
{
public:
    string zapis();
    double oblicz();

    int priorytet() override
    {
        return 3;
    }

    bool lewostronny() override
    {
        return true;
    }

    potega(wyrazenie *arg1, wyrazenie *arg2) : operator2arg(arg1, arg2) {}
};

class modulo : public operator2arg
{
public:
    string zapis();
    double oblicz();

    int priorytet() override
    {
        return 3;
    }

    bool lewostronny() override
    {
        return true;
    }

    modulo(wyrazenie *arg1, wyrazenie *arg2) : operator2arg(arg1, arg2) {}
};

class logarytm : public operator2arg
{
public:
    string zapis();
    double oblicz();

    int priorytet() override
    {
        return 3;
    }

    bool lewostronny() override
    {
        return true;
    }

    logarytm(wyrazenie *arg1, wyrazenie *arg2) : operator2arg(arg1, arg2) {}
};



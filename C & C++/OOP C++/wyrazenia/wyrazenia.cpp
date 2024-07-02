#define _USE_MATH_DEFINES
#include "wyrazenia.hpp"
#include <cmath>
using namespace std;
vector<pair<string, double>> zmienna::zmienne;

liczba::liczba(double value)
{
    this->value = value;
}

string liczba::zapis()
{
    string str = to_string(value);
    int offset = 1;

    if (str.find_last_not_of('0') == str.find('.'))
        offset = 0;

    str.erase(str.find_last_not_of('0') + offset, string::npos);

    return str;
}

double liczba::oblicz()
{
    return value;
}

zmienna::zmienna(string var)
{
    this->zmienna_name = var;
}

string zmienna::zapis()
{
    return zmienna_name;
}

double zmienna::oblicz()
{

        int ile = zmienna::zmienne.size();
        for (int i = 0; i < ile; i++)
            if (zmienna::zmienne[i].first == zmienna_name)
                return zmienna::zmienne[i].second;

        string err = "Zmienna o tej nazwie nie wystepuje zbiorze";
        throw err;

}

void zmienna::dodaj_zmienna(string var, double val)
{
    int ile = zmienna::zmienne.size();
    for (int i = 0; i < ile; i++)
    {
        if (zmienna::zmienne[i].first == var)
        {
            cout << "Zmienna " << var << " juz istnieje, jej wartosc wynosi "
            << zmienna::zmienne[i].second << endl;
            return;
        }
    }

    zmienna::zmienne.push_back(make_pair(var, val));
}

void zmienna::usun_zmienna(string var)
{

        int ile = zmienna::zmienne.size();
        for (int i = 0; i < ile; i++)
        {
            if (zmienna::zmienne[i].first == var)
            {
                zmienna::zmienne.erase(zmienna::zmienne.begin() + i);
                return;
            }
        }
        string err = "Zmienna o tej nazwie nie wystepuje zbiorze";
        throw err;

}

void zmienna::wypisz_zmienna()
{
    int ile = zmienna::zmienne.size();

    cout << "Zmienne: " << endl;
    for (int i = 0; i < ile; i++)
        cout << zmienna::zmienne[i].first << "\t" << zmienna::zmienne[i].second << endl;
}

pi::pi()
{
    value = M_PI;//3.141592;
}

string pi::zapis()
{
    return "pi";
}

double pi::oblicz()
{
    return value;
}

e::e()
{
    value = M_E;//2.718281;
}

string e::zapis()
{
    return "e";
}

double e::oblicz()
{
    return value;
}

fi::fi()
{
    value = 1+sqrt(5)/2;//3.141592;
}

string fi::zapis()
{
    return "fi";
}

double fi::oblicz()
{
    return value;
}

operator1arg::operator1arg(wyrazenie *arg1)
{
    this->arg1 = arg1;
}

przeciw::przeciw(wyrazenie *arg1) : operator1arg(arg1) {}

string przeciw::zapis()
{
    return "-(" + arg1->zapis() + ")";
}

double przeciw::oblicz()
{
    return (-1) * arg1->oblicz();
}

odwrot::odwrot(wyrazenie *arg1) : operator1arg(arg1) {}

string odwrot::zapis()
{
    return "1 / " + arg1->zapis();
}

double odwrot::oblicz()
{
    return 1.0 / arg1->oblicz();
}

Abs::Abs(wyrazenie *arg1) : operator1arg(arg1) {}

string Abs::zapis()
{
    return "|" + arg1->zapis() + "|";
}

double Abs::oblicz()
{
    if (arg1->oblicz() < 0)
        return (-1) * arg1->oblicz();
    else
        return arg1->oblicz();
}

Exp::Exp(wyrazenie *arg1) : operator1arg(arg1) {}

string Exp::zapis()
{
    return "exp(" + arg1->zapis() + ")";
}

double Exp::oblicz()
{
    return exp(arg1->oblicz());
}

Ln::Ln(wyrazenie *arg1) : operator1arg(arg1) {}

string Ln::zapis()
{
    return "Ln(" + arg1->zapis() + ")";
}

double Ln::oblicz()
{
    return log(arg1->oblicz());
}

Sin::Sin(wyrazenie *arg1) : operator1arg(arg1) {}

string Sin::zapis()
{
    return "sin(" + arg1->zapis() + ")";
}

double Sin::oblicz()
{
    return sin(arg1->oblicz());
}

Cos::Cos(wyrazenie *arg1) : operator1arg(arg1) {}

string Cos::zapis()
{
    return "cos(" + arg1->zapis() + ")";
}

double Cos::oblicz()
{
    return cos(arg1->oblicz());
}

operator2arg::operator2arg(wyrazenie *arg1, wyrazenie *arg2) : operator1arg(arg1)
{
    this->arg2 = arg2;
}

string dodaj::zapis()
{
    string left, right;

    if (arg1->priorytet() > priorytet())
        left = arg1->zapis();
    else if (arg1->priorytet() < priorytet())
        left = "(" + arg1->zapis() + ")";
    else if (arg1->lewostronny() == true)
        left = arg1->zapis();
    else
        left = "(" + arg1->zapis() + ")";

    if (arg2->priorytet() > priorytet())
        right = arg2->zapis();
    else if (arg2->priorytet() < priorytet())
        right = "(" + arg2->zapis() + ")";
    else if (arg2->lewostronny() == true)
        right = arg2->zapis();
    else
        right = "(" + arg2->zapis() + ")";

    return left + " + " + right;
}

double dodaj::oblicz()
{
    return arg1->oblicz() + arg2->oblicz();
}

string odejmij::zapis()
{
    string left, right;

    if (arg1->priorytet() > priorytet())
        left = arg1->zapis();
    else if (arg1->priorytet() < priorytet())
        left = "(" + arg1->zapis() + ")";
    else if (arg1->lewostronny() == true)
        left = arg1->zapis();
    else
        left = "(" + arg1->zapis() + ")";

    if (arg2->priorytet() > priorytet())
        right = arg2->zapis();
    else if (arg2->priorytet() < priorytet())
        right = "(" + arg2->zapis() + ")";
    else if (arg2->lewostronny() == true)
        right = arg2->zapis();
    else
        right = "(" + arg2->zapis() + ")";

    return left + " - " + right;
}

double odejmij::oblicz()
{
    return arg1->oblicz() - arg2->oblicz();
}

string mnoz::zapis()
{
    string left, right;

    if (arg1->priorytet() > priorytet())
        left = arg1->zapis();
    else if (arg1->priorytet() < priorytet())
        left = "(" + arg1->zapis() + ")";
    else if (arg1->lewostronny() == true)
        left = arg1->zapis();
    else
        left = "(" + arg1->zapis() + ")";

    if (arg2->priorytet() > priorytet())
        right = arg2->zapis();
    else if (arg2->priorytet() < priorytet())
        right = "(" + arg2->zapis() + ")";
    else if (arg2->lewostronny() == true)
        right = arg2->zapis();
    else
        right = "(" + arg2->zapis() + ")";

    return left + " * " + right;
}

double mnoz::oblicz()
{
    return arg1->oblicz() * arg2->oblicz();
}

string dziel::zapis()
{
    string left, right;

    if (arg1->priorytet() > priorytet())
        left = arg1->zapis();
    else if (arg1->priorytet() < priorytet())
        left = "(" + arg1->zapis() + ")";
    else if (arg1->lewostronny() == true)
        left = arg1->zapis();
    else
        left = "(" + arg1->zapis() + ")";

    if (arg2->priorytet() > priorytet())
        right = arg2->zapis();
    else if (arg2->priorytet() < priorytet())
        right = "(" + arg2->zapis() + ")";
    else if (arg2->lewostronny() == true)
        right = arg2->zapis();
    else
        right = "(" + arg2->zapis() + ")";

    return left + " / " + right;
}

double dziel::oblicz()
{
    return arg1->oblicz() / arg2->oblicz();
}

string potega::zapis()
{
    string left, right;

    if (arg1->priorytet() > priorytet())
        left = arg1->zapis();
    else if (arg1->priorytet() < priorytet())
        left = "(" + arg1->zapis() + ")";
    else if (arg1->lewostronny() == true)
        left = arg1->zapis();
    else
        left = "(" + arg1->zapis() + ")";

    if (arg2->priorytet() > priorytet())
        right = arg2->zapis();
    else if (arg2->priorytet() < priorytet())
        right = "(" + arg2->zapis() + ")";
    else if (arg2->lewostronny() == true)
        right = arg2->zapis();
    else
        right = "(" + arg2->zapis() + ")";

    return left + " ^ " + right;
}

double potega::oblicz()
{
    return pow(arg1->oblicz(), arg2->oblicz());
}

string modulo::zapis()
{
    string left, right;

    if (arg1->priorytet() > priorytet())
        left = arg1->zapis();
    else if (arg1->priorytet() < priorytet())
        left = "(" + arg1->zapis() + ")";
    else if (arg1->lewostronny() == true)
        left = arg1->zapis();
    else
        left = "(" + arg1->zapis() + ")";

    if (arg2->priorytet() > priorytet())
        right = arg2->zapis();
    else if (arg2->priorytet() < priorytet())
        right = "(" + arg2->zapis() + ")";
    else if (arg2->lewostronny() == true)
        right = arg2->zapis();
    else
        right = "(" + arg2->zapis() + ")";

    return left + " % " + right;
}

double modulo::oblicz()
{
    return fmod(arg1->oblicz(), arg2->oblicz());
}

string logarytm::zapis()
{
    return "logarytm(" + arg1->zapis() + ", " + arg2->zapis() + ")";
}

double logarytm::oblicz()
{
    double e1, e2;
    if(arg2->oblicz()<=0 or arg1->oblicz()<=0)
        throw invalid_argument("");
    if(arg2->oblicz()==1 or arg1->oblicz()==1)
        throw invalid_argument("");

    e1 = log (arg1->oblicz());
    e2 = log (arg2->oblicz());

    return e2 / e1;
}

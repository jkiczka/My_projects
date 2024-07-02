
#include <stdexcept>
#include <string>

//class logic_error : public exception {
//public:
//  explicit logic_error (const string& what_arg);
//  explicit logic_error (const char* what_arg);
//};
//

class Index : public std::logic_error
{
protected:
    int max_zakres;
    int index;

public:

    explicit Index(int zak, int i, const char *msg): std::logic_error(msg),max_zakres(zak), index(i) {};

    virtual const char* what() const throw()
    {
        std::string error = "Podano niepoprawny indeks: " +
                            std::to_string(index) + ", " +
                            " zakres to [0, " + std::to_string(max_zakres-1) +"]";

        return error.c_str();

    }
};



class Niekwadratowa  :public std::logic_error
{
public:

    explicit Niekwadratowa(const char *msg):std::logic_error(msg) {};

    virtual const char* what() const throw()
    {
        return "Podana macierz nie jest kwadratowa";
    }
};

class Dodatni_rozmiar  :public std::logic_error
{
public:

    explicit Dodatni_rozmiar(const char *msg) : std::logic_error(msg) {};

    virtual const char* what() const throw()
    {
        return "Rozmiar pownien byc dodatni";
    }
};

class Niewlasciwe_wymiary  : public std::logic_error
{
    protected:
    int k1;
    int k2;


public:

    explicit Niewlasciwe_wymiary(int k1, int k2, const char *msg)
        : std::logic_error(msg),k1(k1), k2(k2) {};

    virtual const char* what() const throw()
    {

        std::string error = ("Wektory powinny miec ten sam rozmiar, natomiast ich rozmiary to: " +
                             std::to_string(k1) + " oraz " +
                             std::to_string(k2));

        return error.c_str();
    }
};

class Niewlasciwe_wymiary2  : public Niewlasciwe_wymiary
{

    //int k1;
    //int k2;
    int w1;
    int w2;


public:

    explicit Niewlasciwe_wymiary2(int k1, int k2, int w1, int w2, const char *msg)
        :  Niewlasciwe_wymiary(k1,k2, ""), w1(w1), w2(w2) {};

    virtual const char* what() const throw()
    {
        std::string error = "Macierze powinny miec odpowiednie wymiary, natomiast ich rozmiary to: " +
                            std::to_string(k1) + "x" + std::to_string(w1) + " oraz "+
                            std::to_string(k2) + "x" + std::to_string(w2);
        return error.c_str();
    }
};


class LI_dl :public std::logic_error
{
public:

    explicit LI_dl(const char *msg):  std::logic_error(msg) {};

    virtual const char* what() const throw()
    {
        return "Lista inicjalizacyjna powinna skaladac sie z wektorow tej samej dlugosci";
    }
};

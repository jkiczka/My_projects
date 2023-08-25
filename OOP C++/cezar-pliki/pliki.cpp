#include "pliki.hpp"
#include <stdexcept>
#include <string.h>
#include <iostream>


wejscie::wejscie(std::string file_name)
{
    is.exceptions(std::ios_base::badbit);
    is.open(file_name, std::ios::in);

}

std::string wejscie::nastlinia()
{
    std::string linia;
    if (std::getline(is,linia))
    {
        return linia;
    }
    return "";
}

bool wejscie::eof()
{
    return is.peek() == EOF;
}

wejscie::~wejscie()
{
    is.close();
}

wyjscie::wyjscie(std::string file_name)
{
    os.exceptions(std::ios_base::badbit | std::ios_base::failbit);
    os.open(file_name, std::ios::out);

}

void wyjscie::piszlinie(std::string s)
{
    os<<s<<'\n';
}

wyjscie::~wyjscie()
{
    os.close();
}

void cezar(std::string in, std::string out, int k)
{
    k=k%26;
    wejscie we(in);
    wyjscie wy(out);
    while(!we.eof())
    {
        std::string a=we.nastlinia();

        for (int i=0; i<a.length(); i++)
        {
                if(a[i]>='A' and a[i]<='Z')
                    a[i]=(a[i]-'A'+k)%26+'A';

        }

        wy.piszlinie(a);

    }
}


void odszyfr_cezar(std::string in, std::string out, int k)
{
    k=k%26;
    wejscie we(in);
    wyjscie wy(out);
    while(!we.eof())
    {
        std::string a=we.nastlinia();

        for (int i=0; i<a.length(); i++)
        {
            if(a[i]>='A' and a[i]<='Z')
                a[i]=(a[i]-k-'A'+26*2)%26+'A';

        }

        wy.piszlinie(a);

    }
}

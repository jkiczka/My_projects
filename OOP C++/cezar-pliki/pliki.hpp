#include <string>
#include <fstream>



class wejscie
{
    std::ifstream is;

public:
    bool eof();
    wejscie(std::string file_name);
    std::string nastlinia();
    ~wejscie();
};

class wyjscie
{
    std::ofstream os;


public:
    wyjscie(std::string file_name);
    void piszlinie(std::string s);
    ~wyjscie();
};



void cezar(std::string in, std::string out, int k);
void odszyfr_cezar(std::string in, std::string out, int k);

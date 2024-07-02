#include <iostream>
#include "wielomiany.hpp"
using namespace std;

int main()
{
    try
    {
        double x[3];

        for (int i=0; i<=3; i++)
            x[i]=i;
    wielomian w1=wielomian(3,x);


    cout<<w1[0];
    cout<<w1[3]<<endl;
    w1[3]=0;
    cout<<w1;
wielomian wiel1{3,5,6,1};
wielomian wiel2{8,8,0,1};
cout<<wiel1<<endl<<wiel2<<endl;
cout<<"MNOZENIE :"<<wiel1*wiel2<<endl;
cout<<"DODAWANIE :"<<wiel1+wiel2<<endl;
wielomian w10{3,5,6,1}; w10*=wiel2;
cout<<w10;
cout<<"ODEJMOWANIE :"<<wiel1-wiel2<<endl;
cout<<"MNOZENIE *5 :"<<wiel1*5<<endl;
wielomian w100{1,-1,0,0,3};
cout<<w100(2);


    }

    catch(string w)
    {
        cerr<<w<<endl;

        return -1;
    }
    catch(std::invalid_argument& e)
    {
        cerr<<e.what()<<endl;
        return -1;
    };

    return 0;

}

#include<iostream>
#include "tabbit.hpp"

int main()
{
    tab_bit t(46);
    tab_bit u(64);
    tab_bit v(t);

    tab_bit w(tab_bit {1,0,1,1,0,1,1,0,1,0,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,1,0,0,0,0,0,1,1,1,0});
    tab_bit z(tab_bit {0,0,1,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,1,1,1,1,0,0,0,0,0,1,1,1,0,1,0});
    v[1] = 1;
    v[1]=1;
    t[45] = 1;
    t[5] = 1;
    bool b = v[1];
    u[63] = 1;
    u[45] = u[46] = u[63];
    std::cout<<"T: "<<t<<std::endl;
    std::cout<<"U: "<<u<<std::endl;
    std::cout<<"V: "<<v<<std::endl;
    u[63] = 0;
    u[45] = false;
    std::cout<<"T: "<<t<<std::endl;
    std::cout<<"U: "<<u<<std::endl;
    std::cout<<"V: "<<v<<std::endl;
    std::cout<<"v[1]: "<<b<<std::endl;
    std::cout<<"W:"<<"\t"<<w<<std::endl;
    std::cout<<"Z:"<<"\t"<<z<<std::endl;
    std::cout<<"W^Z: "<<"\t"<<(w^z)<<std::endl;
    std::cout<<"W&Z: "<<"\t"<<(w&z)<<std::endl;
    std::cout<<"W|Z: "<<"\t"<<(w|z)<<std::endl;
    std::cout<<"!Z: "<<"\t"<<(!z)<<std::endl;

    w |= z;
    std::cout<<"W |= Z: "<<w<<std::endl;


    std::cout<<"W: "<<w<<std::endl;
    std::cout<<"Z: "<<z<<std::endl;

    w ^= z;
    std::cout<<"W ^= Z: "<<w<<std::endl;

    std::cout<<"W: "<<w<<std::endl;
    std::cout<<"Z: "<<z<<std::endl;
    w &= z;
    std::cout<<"W &= Z: "<<w<<std::endl;
    return 0;
}

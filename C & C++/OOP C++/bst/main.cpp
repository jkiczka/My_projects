#include <iostream>
#include "bst.hpp"

using namespace std;

int main()
{
    bst<int> drzewko{-33,24,4,-2,0,21,22,23,50};
    bst<char>drzewko2{'a', 'f', 'k', 'j', 'z', 'd', 'e'};

    drzewko.bst_insert(2);
    drzewko.bst_insert(1);

    drzewko.bst_insert(8);
    drzewko.bst_insert (17);
    drzewko.bst_insert(12);
    drzewko.bst_insert(30);
    drzewko.bst_insert (-1);
    drzewko.bst_insert (7);

    cout<<drzewko<<endl;
    cout<<(*drzewko.bst_delete(8))<<endl;

    cout<<drzewko2<<endl;

    int a;
    cout<<"ile liczb chcesz wpisac do drzewa?"<<endl;
    cin>>a;
    bst<int> d;
    for (int i=0; i<a; i++)
    {
        int x;
        cin>>x;
        d.bst_insert(x);
    }
    cout<<d<<endl;
    int b;
    cout<<"ile liczb chcesz usunac z drzewa?"<<endl;
    cin>>b;
    if (b<=a)
    {

        bst<int>* wsk;
        wsk=&d;

        for (int i=0; i<b; i++)
        {
            int x;
            cin>>x;
            wsk=(*wsk).bst_delete(x);
        }


        cout<<(*wsk);
    }




    return 0;

}

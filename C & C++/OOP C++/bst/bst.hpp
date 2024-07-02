#include <iostream>
#include <utility>
#include <functional>

template <class T, class C=std::less<T>> class bst
{
private:


    class node
    {
    public:
        const T value;
        node* left = nullptr ;
        node* right = nullptr ;
    public:
        node(T val);
        node();
        node(const node &t);
        node(node &&a);
        node* node_insert(T val, C cmp);
        node* node_search(T val, C cmp);
        node* minimum();
        node* node_delete(T val, C cmp);
        node &operator=(const node &n);
        node &operator=(node &&n);


        ~node();
        friend class bst;

        friend std::ostream &operator<<(std::ostream &wy, const node &a)
        {
            if (a.left!=nullptr)
                wy<<*(a.left);

            wy<<(a.value)<<", ";
            if (a.right!=nullptr)
                wy<<*(a.right);

            return wy;
        };

    };

    node* root = nullptr;
    C cmp;

public:
    bst();
    bst(const bst &t);               // konstruktor kopiuj¹cy
    bst(bst &&a);                    // konstruktor przenosz¹cy
    bst &operator=(const bst<T,C> &right); // przypisanie kopiuj¹ce
    bst &operator=(bst<T,C> &&right);         // przypisanie przenosz¹ce
    bst(std::initializer_list<T> list);

    ~bst();
    node* bst_insert ( T x);
    bst<T,C>* bst_delete ( T x);
    node* bst_search ( T x);

    friend std::ostream &operator<<(std::ostream &wy, const bst<T,C> &a)
    {
        wy<<"{ ";
        wy<<*(a.root);
        wy<<"\b \b"<<"\b \b";
        wy<<" }";

        return wy;
    };

};


template <class T, class C> bst<T,C>::node::node( T val)
    :value(val)
{
    left= nullptr;
    right = nullptr;
}
template <class T, class C> bst<T,C>::node::node()
{
    left= nullptr;
    right = nullptr;
    value = 0;
}
template <class T, class C> bst<T,C>::node::~node()
{
    left= nullptr;
    right = nullptr;
}

template < class T, class C>
typename bst<T,C>::node*  bst<T,C>::bst_insert ( T x)
{

    if (root==nullptr)
    {

        root=(new node(x));
        return root;
    }

    return root->node_insert(x,cmp);


}
template < class T, class C>
typename bst<T,C>::node* bst<T,C>::node::node_insert( T x, C cmp)
{

    if(value!=x)
    {
        if (cmp(value,x))
        {

            if(right==nullptr)
            {
                node* nowy=(new node(x));
                right=nowy;
                return this;
            }
            right=(right->node_insert(x, cmp));
        }
        else
        {

            if(left==nullptr)
            {
                node* nowy=(new node(x));
                left=nowy;
                return this;
            }

            left=(left->node_insert(x, cmp));

        }
    }

    return this;

}

template <class T, class C> bst<T,C>::bst(std::initializer_list<T> list)
{
    root=nullptr;
    for(auto x : list)
    {
        if(root==nullptr)
            this->bst_insert(x);
        else
            root->node_insert(x, cmp);
    }
}
template <class T, class C> bst<T,C>::bst()
{
    root=nullptr;
}

template <class T, class C> bst<T,C>::~bst()
{
    delete root;

}
template <class T, class C> bst<T,C>::bst::bst(const bst &t)
{
    root=t.root;
    cmp=t.cmp;
}

template <class T, class C> bst<T,C>::bst::bst( bst &&t)
{
    root=t.root;
    cmp=t.cmp;
    t.root=nullptr;
}




template <class T, class C>
bst<T,C>&bst<T,C>:: operator=( bst<T,C> &&right)

{
    if (this!=&right)
    {

        std::swap(root,right.root);
        std::swap(cmp,right.cmp);

    }
    return *this;

}

template <class T, class C>
bst<T,C>&bst<T,C>:: operator=(const bst<T,C> &right)

{
    if (this!=&right)
    {
        delete root;
        root=right.root;
        cmp=right.cmp;

    }
    return *this;

}

template <class T, class C> bst<T,C>::node::node(const node &t)
{
    right=t.right;
    right=t.right;
    left=t.left;
    value=t.value;
}

template <class T, class C> bst<T,C>::node::node( node &&t)
{
    right=t.right;
    left=t.left;
    value=t.value;
    t.right=nullptr;
    t.left=nullptr;
}




template <class T, class C>
typename bst<T,C>::node& bst<T,C>::node:: operator=( node &&r)

{
    if (this!=&r)
    {

        std::swap(right,r.right);
        std::swap(left,r.left);
        std::swap(value,r.value);

    }
    return *this;

}

template < class T, class C>
typename bst<T,C>::node*  bst<T,C>::bst_search ( T x)
{

    if (root!=nullptr)
    {

        return root->node_search(x, cmp);
    }


    return root;

}


template < class T, class C>
typename bst<T,C>::node* bst<T,C>::node::node_search(T x, C cmp)
{
    if(this==nullptr)
        return nullptr;
    if(value==x)
        return this;
    else
    {
        if (cmp(value,x))
        {
            return (right->node_search(x, cmp));

        }
        else
        {
            return (left->node_search(x, cmp));
        }
    }

    return this;
}

template < class T, class C>
bst<T,C>*  bst<T,C>::bst_delete ( T x)
{

    if (root!=nullptr)
    {

        root=root->node_delete(x, cmp);
        return this;
    }

    return nullptr;


}


template < class T, class C>
typename bst<T,C>::node* bst<T,C>::node::node_delete(T x, C cmp)
{

    node* parent = nullptr; //rodzic tego do usuniecia


    node* curr = this;


    //szukamy rodzica (parent) i tego do usuniecia(curr)
    while (curr != nullptr && curr->value != x)
    {

        parent = curr;


        if (cmp(x,curr->value))
        {
            curr = curr->left;
        }
        else
        {
            curr = curr->right;
        }
    }


    // nie znalezniono wartosci do usuniecia
    if (curr == nullptr)
    {
        return nullptr;
    }

    // gdy node do usuniecia nie ma dzieci
    if (curr->left == nullptr && curr->right == nullptr)
    {

        if (curr != this)
        {
            if (parent->left == curr)
            {
                parent->left = nullptr;
            }
            else
            {
                parent->right = nullptr;
            }
        }
        // gdy jest tylko jeden node, ktorego usuwamy
        else
        {

            return nullptr;
        }


        delete curr;
    }



    // gdy node do usuniecia ma 2 dzieci
    else if (curr->left!=nullptr && curr->right!=nullptr)
    {

        node* successor=curr->right;

        while (successor->left != nullptr)
        {
            successor = successor->left;
        }


        T val = successor->value;

        //usuwamy i podmieniamy z succesorem
        curr=curr->node_delete(val,cmp);

        node *nowy= (new node(val));
        nowy->right=curr->right;
        nowy->left=curr->left;
        if (parent->right==curr)
        {
            parent->right=nowy;
        }
        else
        {
            parent->left=nowy;
        }

        delete curr;
    }

    // node do usuniecia ma tylko jedno dziecko
    else
    {

        node* child = (curr->left)? curr->left: curr->right;
        if (curr != this)
        {
            if (curr == parent->left)
            {
                parent->left = child;
            }
            else
            {
                parent->right = child;
            }
        }

        //gdy ten do usuniecia to this
        else
        {
            return child;
        }

        delete curr;
    }

    return this;

}



template<class T>class Trait
{
public:
    using cmp = std::greater<>;
};




//------------------------------------------------------------------------------------------
//
//template <typename C=std::less<const char*>>
//class bst
//{
//private:
//public:
//
//    class node
//    {
//    public:
//        const char* value;
//        node* left = nullptr ;
//        node* right = nullptr ;
//    public:
//        node(const char* val);
//        node();
//        node(const node &t);
//        node(node &&a);
//        node* node_insert(const char* val, C cmp);
//        node* node_search(const char* val, C cmp);
//        node* minimum();
//        node* node_delete(T val, C cmp);
//        node &operator=(const node &n);
//        node &operator=(node &&n);
//
//
//        ~node();
//        friend class bst;
//
//        friend std::ostream &operator<<(std::ostream &wy, const node &a)
//        {
//            if (a.left!=nullptr)
//                wy<<*(a.left);
//
//            wy<<(a.value)<<", ";
//            if (a.right!=nullptr)
//                wy<<*(a.right);
//
//            return wy;
//        };
//
//    };
//
//    node* root = nullptr;
//    C cmp;
//
//public:
//    bst();
//    bst(const bst &t);               // konstruktor kopiuj¹cy
//    bst(bst &&a);                    // konstruktor przenosz¹cy
//    bst &operator=(const bst<const char*,C> &right); // przypisanie kopiuj¹ce
//    bst &operator=(bst<const char*,C> &&right);         // przypisanie przenosz¹ce
//    bst(std::initializer_list<const char*> list);
//
//    ~bst();
//    node* bst_insert ( cost char * x);
//    bst<const char*,C>* bst_delete ( T x);
//    node* bst_search ( const char* x);
//
//    friend std::ostream &operator<<(std::ostream &wy, const bst<const char*,C> &a)
//    {
//        wy<<"{ ";
//        wy<<*(a.root);
//        wy<<"\b \b"<<"\b \b";
//        wy<<" }";
//
//        return wy;
//    };
//
//};

//
// Created by william on 2021/9/16.
//

#include "templateTest.h"
#include "base.h"
//
//template<typename T>
//TemplateTest::TemplateTest():T()
//{
//    std::cout << "DerivedImp3" << std::endl;
//}



// b.hpp

class Base
{
    template<class...args>
    void templateFunc(args... parameters);
};


// b.cpp
template <class... args>
void Base::templateFunc(args... parameters)
{
}


class Temp;
template void Base::templateFunc(Temp*);


// a.cpp
class Temp
{

};

class Derive: public Base
{

};

#include <iostream>
#include<type_traits>
#include <PeripheralBase.hh>


enum class lablels{a,b,c,d, __length};
Container<lablels, bool, float, int, std::string> a;

int main(int argc, char const *argv[])
{
    std::cout << "hola x2" << std::endl;
    return 0;
}


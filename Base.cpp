#include "Base.hpp"

MyPrint print;

void MyPrint::show_print_count() const
{
    std::cout << print_count << std::endl;
}
//
// Created by Firmiana on 2024/7/24.
//

#ifndef BASE_HPP
#define BASE_HPP

#include <bits/stdc++.h>
#include <graphics.h>
#include <conio.h>
#include <thread>
#include <unordered_map>
#include <mutex>
#define kbhit _kbhit
#define getch _getch

#define MEMBER_DEFINE_GET_SET(type, var) \
private: \
    type var; \
public: \
    type& get_##var() { return var; } \
    void set_##var(const type& value) { var = value; }

template<class T>
std::ostream& operator << (std::ostream& out, const std::vector<T>& vec)
{
    for (const auto& i : vec)
        out << i << ' ';
    out << std::endl;
    return out;
}

template<class T>
std::ostream& operator << (std::ostream& out, const std::list<T>& list)
{
    for (const auto& i : list)
        out << i << ' ';
    out << std::endl;
    return out;
}

class MyPrint
{
    int print_count = 0;
public:
    template<class T>
    void operator() (const std::vector<T>& vec)
    {
        std::cout << vec;
        print_count++;
        std::cout << std::endl;
    }

    template<class T>
    void operator() (const std::list<T>& list)
    {
        std::cout << list;
        print_count++;
        std::cout << std::endl;
    }

    template<class T>
    void operator() (const T& i)
    {
        std::cout << i;
        print_count++;
        std::cout << std::endl;
    }

    void show_print_count() const;
};

extern MyPrint print;

template<class T>
class List : public std::list<T>
{
public:
    T& operator[](const int& index)
    {
        if(this->size() <= index) 
            throw std::runtime_error("List index out of range");
        else 
            return *std::next(this->begin(), index);
    }
};


#endif //BASE_HPP
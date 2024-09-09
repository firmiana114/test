#ifndef REALNUMBER_H
#define REALNUMBER_H

#include <iostream>
#include <list>
#include <stdexcept>

class realnum {
private:
    std::list<int> digits;  // 用于存放每一位的数字
    int decimal_pos;        // 表示小数点的位置在digits中的第几个元素后, 如果为0代表没有小数点
    int sign;               // 表示正负号，1为正，-1为负

    // 辅助函数：规范化数字，去除前导0
    void normalize();

    // 辅助函数：对齐小数点
    void alignDecimal(realnum& other);

    // 辅助函数：大数加法
    std::list<int> addDigits(const std::list<int>& a, const std::list<int>& b) const;

    // 辅助函数：大数减法，假设a >= b
    std::list<int> subtractDigits(const std::list<int>& a, const std::list<int>& b) const;

    // 辅助函数：比较两个数的绝对值
    bool absLess(const realnum& other) const;

public:
    // 构造函数，初始化为0
    realnum();

    // 构造函数，使用整数初始化
    realnum(int num);

    // 构造函数，使用字符串初始化
    realnum(const std::string& num);

    // 输出实数
    void print() const;

    // 加法
    realnum operator+(const realnum& other) const;

    // 减法
    realnum operator-(const realnum& other) const;

    // 乘法
    realnum operator*(const realnum& other) const;

    // 除法
    realnum operator/(const realnum& other) const;

    // 比较运算符
    bool operator<(const realnum& other) const;
    bool operator>(const realnum& other) const;
    bool operator>=(const realnum& other) const;
    bool operator<=(const realnum& other) const;
    bool operator==(const realnum& other) const;

    // 取反
    realnum operator-() const;


};

#endif
#ifndef REALNUMBER_H
#define REALNUMBER_H

#include <iostream>
#include <list>
#include <stdexcept>

class realnum {
private:
    std::list<int> digits;  // ���ڴ��ÿһλ������
    int decimal_pos;        // ��ʾС�����λ����digits�еĵڼ���Ԫ�غ�, ���Ϊ0����û��С����
    int sign;               // ��ʾ�����ţ�1Ϊ����-1Ϊ��

    // �����������淶�����֣�ȥ��ǰ��0
    void normalize();

    // ��������������С����
    void alignDecimal(realnum& other);

    // ���������������ӷ�
    std::list<int> addDigits(const std::list<int>& a, const std::list<int>& b) const;

    // ������������������������a >= b
    std::list<int> subtractDigits(const std::list<int>& a, const std::list<int>& b) const;

    // �����������Ƚ��������ľ���ֵ
    bool absLess(const realnum& other) const;

public:
    // ���캯������ʼ��Ϊ0
    realnum();

    // ���캯����ʹ��������ʼ��
    realnum(int num);

    // ���캯����ʹ���ַ�����ʼ��
    realnum(const std::string& num);

    // ���ʵ��
    void print() const;

    // �ӷ�
    realnum operator+(const realnum& other) const;

    // ����
    realnum operator-(const realnum& other) const;

    // �˷�
    realnum operator*(const realnum& other) const;

    // ����
    realnum operator/(const realnum& other) const;

    // �Ƚ������
    bool operator<(const realnum& other) const;
    bool operator>(const realnum& other) const;
    bool operator>=(const realnum& other) const;
    bool operator<=(const realnum& other) const;
    bool operator==(const realnum& other) const;

    // ȡ��
    realnum operator-() const;


};

#endif
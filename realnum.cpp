#include "realnum.h"

void realnum::normalize()
{
    while (!digits.empty() && digits.front() == 0 && decimal_pos > 1) {
        digits.pop_front();
        decimal_pos--;
    }
    while (!digits.empty() && digits.back() == 0 && decimal_pos <= digits.size()) {
        digits.pop_back();
    }
    if (digits.empty()) {
        digits.push_back(0);
        decimal_pos = 1;
        sign = 1;
    }
}

void realnum::alignDecimal(realnum& other) {
    int diff = decimal_pos - other.decimal_pos;

    if (diff > 0) {
        // 当前对象的小数点位置在其他对象的前面
        other.digits.insert(other.digits.end(), diff, 0);
        other.decimal_pos = decimal_pos;
    }
    else if (diff < 0) {
        // 当前对象的小数点位置在其他对象的后面
        digits.insert(digits.end(), -diff, 0);
        decimal_pos = other.decimal_pos;
    }
}


std::list<int> realnum::addDigits(const std::list<int>& a, const std::list<int>& b) const
{
    std::list<int> result;
    auto it1 = a.rbegin(), it2 = b.rbegin();
    int carry = 0;
    while (it1 != a.rend() || it2 != b.rend() || carry) {
        int sum = carry;
        if (it1 != a.rend()) sum += *it1++;
        if (it2 != b.rend()) sum += *it2++;
        result.push_front(sum % 10);
        carry = sum / 10;
    }
    return result;
}

std::list<int> realnum::subtractDigits(const std::list<int>& a, const std::list<int>& b) const
{
    std::list<int> result;
    auto it1 = a.rbegin(), it2 = b.rbegin();
    int borrow = 0;
    while (it1 != a.rend() || it2 != b.rend()) {
        int diff = borrow + (it1 != a.rend() ? *it1++ : 0) - (it2 != b.rend() ? *it2++ : 0);
        if (diff < 0) {
            diff += 10;
            borrow = -1;
        }
        else {
            borrow = 0;
        }
        result.push_front(diff);
    }
    return result;
}

bool realnum::absLess(const realnum& other) const
{
    if (decimal_pos != other.decimal_pos)
        return decimal_pos < other.decimal_pos;
    return digits < other.digits;
}

realnum::realnum() : digits(1, 0), decimal_pos(1), sign(1) 
{
}

realnum::realnum(int num)
{
    sign = (num < 0) ? -1 : 1;
    num = std::abs(num);
    while (num > 0) {
        digits.push_front(num % 10);
        num /= 10;
    }
    decimal_pos = digits.size();
    if (digits.empty()) {
        digits.push_back(0);
        decimal_pos = 1;
    }
}

realnum::realnum(const std::string& num)
{
    sign = 1;
    decimal_pos = num.size();
    bool decimal_found = false;
    for (char ch : num) {
        if (ch == '-') {
            sign = -1;
        }
        else if (ch == '.') {
            decimal_found = true;
            decimal_pos = digits.size();
        }
        else if (std::isdigit(ch)) {
            digits.push_back(ch - '0');
        }
    }
    if (!decimal_found) {
        decimal_pos = digits.size();
    }
    normalize();
}

void realnum::print() const
{
    if (sign == -1 && !(digits.size() == 1 && digits.front() == 0))
        std::cout << '-';
    auto it = digits.begin();
    for (int i = 0; it != digits.end(); ++i, ++it) {
        if (i == decimal_pos)
            std::cout << '.';
        std::cout << *it;
    }
    std::cout << std::endl;
}

realnum realnum::operator+(const realnum& other) const {
    if (sign == other.sign) {
        realnum result = *this;
        result.alignDecimal(const_cast<realnum&>(other));
        result.digits = addDigits(result.digits, other.digits);
        result.normalize();
        return result;
    }
    return *this - (-other);  // 如果符号不同，转换为减法
}

realnum realnum::operator-(const realnum& other) const {
    realnum a = *this, b = other;
    a.alignDecimal(b);

    if (sign == b.sign) {
        if (absLess(b)) {
            std::swap(a, b);
            a.sign = -a.sign;
        }
        a.digits = subtractDigits(a.digits, b.digits);
    }
    else {
        a.digits = addDigits(a.digits, b.digits);
    }
    a.normalize();
    return a;
}

// 实现乘法
realnum realnum::operator*(const realnum& other) const {
    realnum result;
    result.digits.clear();
    result.decimal_pos = decimal_pos + other.decimal_pos;  // 小数位对齐

    // 乘法需要存储中间结果
    std::list<int> temp(digits.size() + other.digits.size(), 0);
    auto it1 = digits.rbegin();
    for (int i = 0; it1 != digits.rend(); ++it1, ++i) {
        auto it2 = other.digits.rbegin();
        int carry = 0;
        auto temp_it = std::next(temp.rbegin(), i);
        for (; it2 != other.digits.rend(); ++it2, ++temp_it) {
            int mul = *it1 * *it2 + carry + *temp_it;
            carry = mul / 10;
            *temp_it = mul % 10;
        }
        if (carry) {
            *temp_it += carry;
        }
    }

    result.digits.assign(temp.rbegin(), temp.rend());
    result.sign = sign * other.sign;  // 符号处理
    result.normalize();
    return result;
}

// 实现除法 (此处采用长除法法则)
realnum realnum::operator/(const realnum& other) const {
    if (other.digits.size() == 1 && other.digits.front() == 0) {
        throw std::invalid_argument("除数不能为零");
    }

    realnum result;
    result.sign = sign * other.sign;  // 符号处理

    realnum dividend = *this;
    realnum divisor = other;

    // 对齐小数点
    int precision = 10;  // 我们使用10位小数精度
    int shift = dividend.decimal_pos - divisor.decimal_pos;
    dividend.digits.insert(dividend.digits.end(), precision, 0);
    dividend.decimal_pos += precision;

    divisor.decimal_pos = divisor.digits.size();  // 保证除数是整数

    // 长除法计算
    realnum current_dividend;
    auto it = dividend.digits.begin();
    while (it != dividend.digits.end() || result.digits.size() < precision) {
        if (it != dividend.digits.end()) {
            current_dividend.digits.push_back(*it);
            it++;
        }
        else {
            current_dividend.digits.push_back(0);  // 补足小数部分
        }

        current_dividend.normalize();
        int quotient_digit = 0;
        while (!(current_dividend < divisor)) {
            current_dividend = current_dividend - divisor;
            quotient_digit++;
        }

        result.digits.push_back(quotient_digit);
    }

    result.decimal_pos = result.digits.size() - precision;  // 设置小数点位置
    result.normalize();
    return result;
}

// 实现比较运算符
bool realnum::operator<(const realnum& other) const {
    if (sign != other.sign) return sign < other.sign;
    if (decimal_pos != other.decimal_pos) return (sign == 1) ? (decimal_pos < other.decimal_pos) : (decimal_pos > other.decimal_pos);
    return (sign == 1) ? (digits < other.digits) : (digits > other.digits);
}

// 取反
realnum realnum::operator-() const {
    realnum result = *this;
    result.sign = -sign;
    return result;
}

// 实现 >
bool realnum::operator>(const realnum& other) const {
    return !(*this < other) && !(*this == other);
}

// 实现 >=
bool realnum::operator>=(const realnum& other) const {
    return !(*this < other);
}

// 实现 <=
bool realnum::operator<=(const realnum& other) const {
    return (*this < other) || (*this == other);
}

// 实现 ==
bool realnum::operator==(const realnum& other) const {
    return (sign == other.sign) &&
        (decimal_pos == other.decimal_pos) &&
        (digits == other.digits);
}

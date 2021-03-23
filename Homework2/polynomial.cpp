#include <vector>
#include <cmath>
#include "polynomial.h"
#include <sstream>
#include <algorithm>

using namespace std;

Polynomial::Polynomial() : min_pow(0), max_pow(0) {
    n = 1;
    constants = new int[1]{0};
}

//fixed mas
Polynomial::Polynomial(int first, int second, const int *mass) : min_pow(first), max_pow(second) {
    n = second - first + 1;
    constants = new int[n];
    for (int i = 0; i < n; i++)
        constants[i] = mass[i];
}

Polynomial::Polynomial(const Polynomial &pol) {
    min_pow = pol.min_pow;
    max_pow = pol.max_pow;
    n = pol.n;
    constants = new int[n];
    for (int i = 0; i < n; i++)
        constants[i] = pol.constants[i];
}

Polynomial &Polynomial::operator=(const Polynomial &pol) {
    //fixed delete
    min_pow = pol.min_pow;
    max_pow = pol.max_pow;
    n = pol.n;
    delete[] constants;
    constants = new int[n];
    for (int i = 0; i < n; i++)
        constants[i] = pol.constants[i];
    return *this;
}

bool operator==(const Polynomial &left, const Polynomial &right) {
    stringstream s1, s2;
    s1 << left;
    s2 << right;
    return s1.str() == s2.str();
}

bool operator!=(const Polynomial &left, const Polynomial &right) {
    return !(left == right);
}

Polynomial operator-(const Polynomial &pol) {
    int *temp = new int[pol.n];
    for (int i = 0; i < pol.n; i++)
        temp[i] = -pol.constants[i];
    return Polynomial(pol.min_pow, pol.max_pow, temp);
}

Polynomial &Polynomial::operator+=(const Polynomial &pol) {
    int new_min = min(min_pow, pol.min_pow);
    int new_max = max(max_pow, pol.max_pow);
    int new_n = new_max - new_min + 1;
    int *new_mass = new int[new_n];

    for (int i = 0; i < new_n; i++)
        new_mass[i] = 0;

    for (int i = 0; i < n; i++)
        new_mass[min_pow + i - new_min] += constants[i];

    for (int i = 0; i < pol.n; i++)
        new_mass[pol.min_pow + i - new_min] += pol.constants[i];

    min_pow = new_min;
    max_pow = new_max;
    n = new_n;
    constants = new_mass;

    return *this;
}

Polynomial operator*(const Polynomial &left, const Polynomial &right) {
    auto temp = left;
    temp *= right;
    return temp;
}

Polynomial &Polynomial::operator*=(const Polynomial &pol) {
    int new_min = min_pow + pol.min_pow;
    int new_max = max_pow + pol.max_pow;
    int new_n = new_max - new_min + 1;
    int *new_mass = new int[new_n];

    for (int i = 0; i < new_n; i++)
        new_mass[i] = 0;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < pol.n; j++)
            new_mass[min_pow + i + pol.min_pow + j - new_min] += constants[i] * pol.constants[j];

    min_pow = new_min;
    max_pow = new_max;
    n = new_n;
    constants = new_mass;

    return *this;
}

Polynomial &Polynomial::operator*=(int num) {
    //fixed for_each
    for_each(constants, constants + n, [&](int& x) {x *= num;});
    return *this;
}

Polynomial &Polynomial::operator/=(int num) {
    for (int i = 0; i < n; i++)
        constants[i] /= num;
    return *this;
}

Polynomial operator*(const Polynomial &pol, int num) {
    auto temp = pol;
    temp *= num;
    return temp;
}

Polynomial operator*(int num, const Polynomial &pol) {
    return pol * num;
}

Polynomial operator/(const Polynomial &pol, int num) {
    auto temp = pol;
    temp /= num;
    return temp;
}

Polynomial &Polynomial::operator-=(const Polynomial &pol) {
    *this += (-pol);
    return *this;
}

int Polynomial::operator[](int num) const {
    if (num < min_pow || num > max_pow)
        return 0;
    return constants[num - min_pow];
}

int &Polynomial::operator[](int num) {
    if (num < min_pow || num > max_pow) {
        int new_min = min(min_pow, num);
        int new_max = max(max_pow, num);
        //fixed memory-leak
        int *mass = new int[new_max - new_min + 1];
        for (int i = 0; i < new_max - new_min + 1; i++) {
            mass[i] = 0;
        }
        Polynomial temp(new_min, new_max, mass);
        delete[] mass;

        *this += temp;
    }
    return constants[num - min_pow];
}

Polynomial operator+(const Polynomial &left, const Polynomial &right) {
    auto temp = left;
    temp += right;
    return temp;
}

Polynomial operator-(const Polynomial &left, const Polynomial &right) {
    auto temp = left;
    temp -= right;
    return temp;
}

ostream &operator<<(ostream &out, const Polynomial &pol) {
    bool check = true;
    for (int i = 0; i < pol.n; i++)
        if (pol.constants[i] != 0)
            check = false;

    if (check) {
        out << pol.constants[0];
        return out;
    }

    bool first = true;
    for (int i = pol.max_pow - pol.min_pow; i >= 0; i--)
        if (pol.constants[i] != 0) {
            if (pol.constants[i] > 0 && !first)
                out << "+";
            if (pol.constants[i] == -1)
                out << "-";

            if (abs(pol.constants[i]) != 1)
                out << pol.constants[i];

            if (pol.min_pow + i != 0) {
                out << "x";
                if (pol.min_pow + i != 1)
                    out << "^" << pol.min_pow + i;
            } else if (abs(pol.constants[i]) == 1)
                out << 1;

            first = false;
        }
    return out;
}

//fixed get O(n)
double Polynomial::get(int num) {
    double temp = constants[0] * pow(num, min_pow);
    double res = temp;

    for (int i = 1; i < n; i++) {
        temp *= num;
        res += constants[i] * temp;
    }

    return res;
}

Polynomial::~Polynomial() {
    delete[] constants;
}
#include <iostream>
#include <vector>
#include <cmath>
#include "polynomial.h"
#include <sstream>

using namespace std;

Polynomial::Polynomial() : min_pow(0), max_pow(0) {
    n = 1;
    constants = new int[1]{0};
}

Polynomial::Polynomial(const int first, const int second, const int *mas) : min_pow(first), max_pow(second) {
    n = second - first + 1;
    constants = new int[n];
    for (int i = 0; i < n; i++)
        constants[i] = mas[i];
}

Polynomial::Polynomial(const Polynomial &pol) : min_pow(pol.min_pow), max_pow(pol.max_pow), n(pol.n),
                                                constants(pol.constants) {}

Polynomial &Polynomial::operator=(const Polynomial &pol) {
    min_pow = pol.min_pow;
    max_pow = pol.max_pow;
    n = pol.n;
    constants = pol.constants;
    return *this;
}

bool operator==(const Polynomial &left, const Polynomial &right) {
    return (left.min_pow == right.min_pow && left.max_pow == right.max_pow && left.constants == right.constants);
}

bool operator!=(const Polynomial &left, const Polynomial &right) {
    return !(left == right);
}

Polynomial operator-(const Polynomial &pol) {
    auto temp = pol;
    for (int i = 0; i < temp.n; i++)
        pol.constants[i] = -pol.constants[i];
    return temp;
}

Polynomial &Polynomial::operator+=(const Polynomial &pol) {
    int new_min = min(min_pow, pol.min_pow);
    int new_max = max(max_pow, pol.max_pow);
    unsigned new_n = new_max - new_min + 1;
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

Polynomial &Polynomial::operator-=(const Polynomial &pol) {
    *this += (-pol);
    return *this;
}

Polynomial operator+(const Polynomial &left, const Polynomial &right) {
    auto d = left;
    d += right;
    return d;
}

Polynomial operator-(const Polynomial &left, const Polynomial &right) {
    auto d = left;
    d -= right;
    return d;
}

ostream &operator<<(ostream &out, const Polynomial &pol) {

    if (pol.max_pow - pol.min_pow == 0) {
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

Polynomial::~Polynomial() {
    delete constants;
}
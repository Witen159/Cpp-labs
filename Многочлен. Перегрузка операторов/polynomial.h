#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>

using namespace std;

class Polynomial {
public:
    Polynomial();

    Polynomial(int first, int second, const int* mas);

    Polynomial(const Polynomial& pol);

    Polynomial& operator=(const Polynomial& pol);

    friend ostream& operator<<(ostream& out, const Polynomial& pol);

    friend bool operator==(const Polynomial& left, const Polynomial& right);

    friend bool operator!=(const Polynomial& left, const Polynomial& right);

    friend Polynomial operator+(const Polynomial& left, const Polynomial& right);

    friend Polynomial operator-(const Polynomial& left, const Polynomial& right);

    friend Polynomial operator-(const Polynomial& pol);

    Polynomial& operator+=(const Polynomial& pol);

    Polynomial& operator-=(const Polynomial& pol);

    Polynomial& operator*=(const Polynomial& pol);

    Polynomial& operator*=(int num);

    Polynomial& operator/=(int num);

    friend Polynomial operator*(const Polynomial& left, const Polynomial& right);

    friend Polynomial operator*(const Polynomial& pol, int num);

    friend Polynomial operator*(int num, const Polynomial& pol);

    friend Polynomial operator/(const Polynomial& pol, int num);

    int operator[](int num) const;

    int& operator[](int num);

    double get(int num);

    ~Polynomial();

private:
    int min_pow;
    int max_pow;
    int* constants;
    int n;
};
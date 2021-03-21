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

    Polynomial(const int first, const int second, const int* mas);

    Polynomial(const Polynomial& pol);

    Polynomial& operator=(const Polynomial& pol);

    friend ostream& operator<< (ostream& out, const Polynomial& pol);

    friend bool operator==(const Polynomial& left, const Polynomial& right);

    friend bool operator!=(const Polynomial& left, const Polynomial& right);

    friend Polynomial operator+(const Polynomial& left, const Polynomial& right);

    friend Polynomial operator-(const Polynomial& left, const Polynomial& right);

    friend Polynomial operator-(const Polynomial& pol);

    Polynomial& operator+= (const Polynomial& pol);

    Polynomial& operator-= (const Polynomial& pol);



    ~Polynomial();

private:
    int min_pow;
    int max_pow;
    int* constants;
    unsigned n;
};
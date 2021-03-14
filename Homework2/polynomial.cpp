#include <iostream>
#include <vector>
#include <cmath>
#include "polynomial.h"

using namespace std;

Polynomial::Polynomial() : min_pow(0), max_pow(0) {}

Polynomial::Polynomial(const int first, const int second, int* mas) : min_pow(first), max_pow(second) {
	for (int i = 0; i < second - first + 1; i++)
		constants->push_back(mas[i]);
}

Polynomial::Polynomial(const Polynomial& pol) : min_pow(pol.min_pow), max_pow(pol.max_pow), constants(pol.constants) {}

Polynomial& Polynomial::operator=(const Polynomial& pol){
	min_pow = pol.min_pow;
	max_pow = pol.max_pow;
	constants = pol.constants;
	return *this;
}

/*int Polynomial::get_min() const {
	return min_pow;
}

int Polynomial::get_max() const {
	return max_pow;
}*/

bool Polynomial::operator==(const Polynomial& pol) {
	return (min_pow == pol.min_pow && max_pow == pol.max_pow && constants == pol.constants);
}

bool Polynomial::operator!=(const Polynomial& pol) {
	return !(*this == pol);
}

Polynomial& Polynomial::operator+ (const Polynomial& pol) {
	int new_min = min(min_pow, pol.min_pow);
	int new_max = max(max_pow, pol.max_pow);
	int* new_mass = new int[new_max - new_min + 1];

	for (int i = 0; i < new_max - new_min + 1; i++) {
		new_mass[i] = 0;
		int temp = new_max + i;
		if (temp <= max_pow && temp >= min_pow)
			new_mass[i] = new_mass[i] + constants[temp - min_pow];
	}
}

Polynomial::~Polynomial() {
	delete constants;
}
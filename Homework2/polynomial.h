#pragma once
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class Polynomial {
public:
	Polynomial();

	Polynomial(const int first, const int second, int* mas);

	Polynomial(const Polynomial& pol);

	Polynomial& operator=(const Polynomial& pol);

	bool operator==(const Polynomial& pol);

	bool operator!=(const Polynomial& pol);

	Polynomial& operator+ (const Polynomial& pol);

	/*int get_min() const;

	int get_max() const;*/

	~Polynomial();

private:
	int min_pow;
	int max_pow;
	vector<int>* constants;
};
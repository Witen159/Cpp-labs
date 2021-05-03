#pragma once
#include <iostream>
#include <vector>

template <class Iter, class prd_t>
bool allOf(Iter first, Iter last, prd_t prd) {
	bool check = true;

	while (first != last) {
		if (!prd(*first)) {
			check = false;
			break;
		}
		++first;
	}

	return check;
}

template <class Iter, class prd_t>
bool anyOf(Iter first, Iter last, prd_t prd) {
	bool check = false;

	while (first != last) {
		if (prd(*first)) {
			check = true;
			break;
		}
		++first;
	}

	return check;
}

template <class Iter, class prd_t>
bool noneOf(Iter first, Iter last, prd_t prd) {
	bool check = true;

	while (first != last) {
		if (prd(*first)) {
			check = false;
			break;
		}
		++first;
	}

	return check;
}

template <class Iter, class prd_t>
bool oneOf(Iter first, Iter last, prd_t prd) {
	bool check = false;
	int counter = 0;
	while (first != last) {
		if (prd(*first)) {
			check = true;
			++counter;
		}
		if (counter > 1) {
			check = false;
			break;
		}
		++first;
	}

	return check;
}


template <class Iter, class prd_t = std::less<>>
bool isSorted(Iter first, Iter last, prd_t prd = prd_t()) {
	bool check = true;
	auto temp = *first;
	first++;
	while (first != last) {
		if (!prd(temp, *first)) {
			check = false;
			break;
		}
		temp = *first;
		++first;
	}

	return check;
}



template <class Iter, class prd_t>
bool isPartitioned(Iter first, Iter last, prd_t prd) {
	bool check = true;
	bool temp = false;
	while (first != last) {
		if (!prd(*first)) {
			temp = true;
		}
		if (prd(*first) && temp) {
			check = false;
			break;
		}
		++first;
	}

	return check;
}


template <typename Iter, typename T>
Iter findNot(Iter first, Iter last, const T& value) {
	while (first != last && *first == value)
		++first;
	return first;
}

template <typename Iter, typename T>
Iter findBackward(Iter first, Iter last, const T& value) {
	Iter temp = last;
	while (first != last) {
		if (*first == value)
			temp = first;
		++first;
	}

	return temp;
}

template <class Iter, class prd_t>
bool isPalindrome(Iter first, Iter last, prd_t prd) {
	bool check = true;
	std::vector<int> arr;
	while (first != last) {
		arr.push_back(*first);
		++first;
	}
	size_t size = arr.size();
	for (size_t i = 0; i < size / 2; ++i) {
		if (!prd(arr[i], arr[size - i - 1])) {
			check = false;
			break;
		}
	}

	return check;
}
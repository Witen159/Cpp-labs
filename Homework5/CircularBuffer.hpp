#pragma once

#include <iostream>
#include <exception>

template <class T>
class My_iterator {
public:
	using iterator_category = std::random_access_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = value_type*;
	using reference = value_type&;

	My_iterator(T* start) : data_(start) {}

	My_iterator(const My_iterator& iter) : data_(iter.data_) {}

	~My_iterator() = default;

	T& operator*() const{
		return *(data_);
	}

	T* operator->() {
		return data_;
	}

	My_iterator operator++(){
		++data_;
		return *this;
	}

	My_iterator operator--(){
		--data_;
		return *this;
	}

	My_iterator operator+(T value){
		data_ += value;
		return *this;
	}

	My_iterator operator-(T value){
		data_ -= value;
		return *this;
	}

	T operator-(const My_iterator& obj) const {
		return data_ - obj.data_;
	}

	bool operator==(const My_iterator& iter) const{
		return data_ == iter.data_;
	}

	bool operator!=(const My_iterator& iter) const{
		return data_ != iter.data_;
	}

	bool operator<(const My_iterator& iter) const{
		return data_ < iter.data_;
	}

	bool operator<=(const My_iterator& iter) const{
		return data_ <= iter.data_;
	}

	bool operator>(const My_iterator& iter) const{
		return data_ > iter.data_;
	}

	bool operator>=(const My_iterator& iter) const{
		return data_ >= iter.data_;
	}

private:
	T* data_ = nullptr;
};

template <class T>
class CircularBuffer {
public:
	CircularBuffer(int capacity) : capacity_(capacity + 1) {
		data_ = new T[capacity_];
	}

	CircularBuffer(const CircularBuffer& buffer): size_(buffer.size_), capacity_(buffer.capacity_), data_(buffer.data_) {}

	~CircularBuffer() = default;

	My_iterator<T> begin() const{
		return My_iterator<T>(data_);
	}

	My_iterator<T> end() const{
		return My_iterator<T>(data_ + size_);
	}

	T first() const {
		return data_[0];
	}

	T last() const {
		return data_[size_ - 1];
	}

	void addFirst(T value) {
		if (size_ < capacity_ - 1)
		{
			for (int i = size_; i >= 1; i--)
				data_[i] = data_[i - 1];
			data_[0] = value;
			size_++;
		}
		else {
			for (int i = capacity_ - 2; i >= 1; i--)
				data_[i] = data_[i - 1];
			data_[0] = value;
		}
	}

	void addLast(T value) {
		if (size_ < capacity_ - 1) {
			data_[size_] = value;
			size_++;
		}
		else {
			for (int i = 1; i < capacity_ - 1; i++)
				data_[i - 1] = data_[i];
			data_[capacity_ - 2] = value;
		}
	}

	void delFirst() {
		if (size_ > 0) {
			for (int i = 0; i < size_ - 1; i++)
				data_[i] = data_[i + 1];
			size_--;
		}
	}

	void delLast() {
		if (size_ > 0)
			size_--;
	}

	void changeCapacity(int new_capacity) {
		if (new_capacity < size_) {
			throw std::logic_error("Wrong capacity");
		}
		else {
			T* new_data = new T[new_capacity + 1];
			for (int i = 0; i < size_; i++)
				new_data[i] = data_[i];
			delete data_;
			data_ = new_data;
			capacity_ = new_capacity + 1;
		}
	}

	T& operator[](int index) const{
		if (index > size_ - 1 || index < 0) {
			throw std::range_error("Out of range");
		}
		else
			return data_[index];
	}

	int size() {
		return size_;
	}


private:
	int size_ = 0;
	int capacity_ = 0;
	T* data_ = nullptr;
};
#pragma once

#include <iostream>
#include <exception>
#include <string>

//fixed O(1)
//fixeds size_t not int
template <class T>
class My_iterator {
public:
	using iterator_category = std::random_access_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = value_type*;
	using reference = value_type&;

	My_iterator(T* data, size_t capacity, size_t index, size_t head) : data_(data), capacity_(capacity), index_(index), head_(head) {}

	My_iterator(const My_iterator& iter) : data_(iter.data_), capacity_(iter.capacity_), index_(iter.index_), head_(iter.head_) {}

	~My_iterator() = default;

	T& operator*() const {
		return data_[(head_ + index_) % capacity_];
	}

	T* operator->() {
		return data_ + (head_ + index_) % capacity_;
	}

	My_iterator& operator++() {
		index_++;
		return *this;
	}

	My_iterator& operator--() {
		index_--;
		return *this;
	}

	My_iterator operator+(T value) {
		My_iterator temp = *this;
		temp.index_ += value;
		return temp;
	}

	My_iterator operator-(T value) {
		My_iterator temp = *this;
		temp.index_ -= value;
		return temp;
	}

	T operator-(const My_iterator& obj) const {
		return index_ - obj.index_;
	}

	T operator+(const My_iterator& obj) const {
		return index_ + obj.index_;
	}

	bool operator==(const My_iterator& iter) const {
		return index_ == iter.index_;
	}

	bool operator!=(const My_iterator& iter) const {
		return index_ != iter.index_;
	}

	bool operator<(const My_iterator& iter) const {
		return index_ < iter.index_;
	}

	bool operator<=(const My_iterator& iter) const {
		return index_ <= iter.index_;
	}

	bool operator>(const My_iterator& iter) const {
		return index_ > iter.index_;
	}

	bool operator>=(const My_iterator& iter) const {
		return index_ >= iter.index_;
	}

private:
	T* data_ = nullptr;
	size_t capacity_;
	size_t index_;
	size_t head_;
};

template <class T>
class CircularBuffer {
public:
	CircularBuffer(size_t capacity) : capacity_(capacity) {
		data_ = new T[capacity_];
	}

	CircularBuffer(const CircularBuffer& buffer) : size_(buffer.size_), capacity_(buffer.capacity_), data_(buffer.data_) {}

	~CircularBuffer() {
		delete[] data_;
	};

	My_iterator<T> begin() const {
		return My_iterator<T>(data_, capacity_, 0, head_);
	}

	My_iterator<T> end() const {
		return My_iterator<T>(data_, capacity_, size_, head_);
	}

	T first() const {
		return data_[head_];
	}

	T last() const {
		return data_[tail_];
	}

	void addFirst(T value) {
		if (size_ == 0) {
			size_++;
			data_[head_] = value;
		}
		else {
			go_back(head_);
			data_[head_] = value;

			if (size_ == capacity_) {
				go_back(tail_);
			}
			else
				size_++;
		}
	}

	void addLast(T value) {
		if (size_ == 0) {
			size_++;
			data_[tail_] = value;
		}
		else {
			go_forward(tail_);
			data_[tail_] = value;

			if (size_ == capacity_) {
				go_forward(head_);
			}
			else
				size_++;
		}
	}

	void delFirst() {
		size_--;
		if (size_ != 0)
			go_forward(head_);
	}

	void delLast() {
		size_--;
		if (size_ != 0)
			go_back(tail_);
	}

	void changeCapacity(size_t new_capacity) {
		if (new_capacity < size_) {
			throw std::logic_error("Wrong capacity");
		}

		if (size_ == 0) {
			delete[] data_;
			data_ = new T[new_capacity];
			capacity_ = new_capacity;
			head_ = 0;
			tail_ = 0;
		}
		else {
			T* new_data = new T[new_capacity];

			if (head_ <= tail_) {
				for (size_t i = 0; i <= tail_ - head_; i++)
					new_data[0] = data_[i + head_];
				head_ = 0;
				tail_ -= head_;
			}
			else {
				int curr_index = 0;
				for (size_t i = head_; i <= capacity_ - 1; i++) {
					new_data[curr_index] = data_[i];
					curr_index++;
				}
				for (size_t i = 0; i <= tail_; i++) {
					new_data[curr_index] = data_[i];
					curr_index++;
				}
				head_ = 0;
				tail_ += capacity_ - head_;
			}
			//fixed delete[]
			delete[] data_;
			data_ = new_data;
			capacity_ = new_capacity;
		}
	}

	//fixed more information in exceptions
	T& operator[](int index) const {
		if (size_ == 0)
			throw std::logic_error("Empty buffer. You can't take buffer elements.\n");
		if (index > size_ - 1 || index < 0)
			throw std::range_error("Wrong index.\nAvailable indexes: [0; " + std::to_string(size_ - 1) + "]\nYour index: " + std::to_string(index) + '\n');
		if (head_ <= tail_)
			return data_[head_ + index];
		else
			return data_[index - (capacity_ - head_)];
	}

	size_t size() {
		return size_;
	}

	T testing_data(size_t index) {
		return data_[index];
	}


private:
	size_t size_ = 0;
	size_t capacity_ = 0;
	size_t head_ = 0;
	size_t tail_ = 0;
	T* data_ = nullptr;

	void go_back(size_t& index) {
		if (index == 0)
			index = capacity_ - 1;
		else
			index--;
	}

	void go_forward(size_t& index) {
		if (index == capacity_ - 1)
			index = 0;
		else
			index++;
	}
};
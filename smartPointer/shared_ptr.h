#pragma once
#include <iostream>

namespace test {

template<typename T>
class shared_ptr {
public:
	explicit shared_ptr(T* p = nullptr);
	shared_ptr(shared_ptr&& p);
	shared_ptr(const shared_ptr& p);
	shared_ptr& operator=(shared_ptr&& p);
	shared_ptr& operator=(const shared_ptr& p);
	~shared_ptr();

	shared_ptr& operator=(T* p) = delete;
	explicit operator bool() const noexcept;
	T& operator*() const;
	T* operator->() const;
// private: // for debug & test
	T* ptr;
	int *count;
	void minus_count();
};

template<typename T>
inline void shared_ptr<T>::minus_count() {
	if (count) {
		--(*count);
		if (*count == 0) {
			delete count;
			if (ptr) {
#ifdef DEBUG
				std::cout << "delete ptr" << std::endl;
#endif
				delete ptr;
			}
		}
	}
}

template<typename T>
shared_ptr<T>::shared_ptr(T* p) : ptr(p) {
	if (ptr) {
		count = new int(1);
	}
}

template<typename T>
shared_ptr<T>::shared_ptr(shared_ptr&& p) {
#ifdef DEBUG
	std::cout << "move constructor" << std::endl;
#endif
	ptr = p.ptr;
	count = p.count;
	p.ptr = nullptr;
	p.count = nullptr;
}

template<typename T>
shared_ptr<T>::shared_ptr(const shared_ptr& p) {
#ifdef DEBUG
	std::cout << "copy constructor" << std::endl;
#endif
	ptr = p.ptr;
	count = p.count;
	if (count) {
		++(*count);
	}
}

template<typename T>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr&& p) {
#ifdef DEBUG
	std::cout << "move assignment" << std::endl;
#endif
	if (p.ptr == ptr) {
		return *this;
	}
	minus_count();
	ptr = p.ptr;
	count = p.count;
	p.ptr = nullptr;
	p.count = nullptr;
	return *this;
}

template<typename T>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr& p) {
#ifdef DEBUG
	std::cout << "copy assignment" << std::endl;
#endif
	if (p.ptr == ptr) {
		return *this;
	}
	minus_count();
	ptr = p.ptr;
	count = p.count;
	++(*count);
	return *this;
}

template<typename T>
shared_ptr<T>::~shared_ptr() {
	minus_count();
}

template<typename T>
shared_ptr<T>::operator bool() const noexcept {
	if (ptr) {
		return true;
	}
	return false;
}

template<typename T>
T& shared_ptr<T>::operator*() const {
	if (ptr) {
		return *ptr;
	}
	throw "empty shared_ptr!";
}

template<typename T>
T* shared_ptr<T>::operator->() const {
	if (ptr) {
		return ptr;
	}
	throw "empty shared_ptr!";
}

}
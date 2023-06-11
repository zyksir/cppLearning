#pragma once
#include <iostream>

namespace test {

template<typename T>
class unique_ptr {
public:
	explicit unique_ptr();
	explicit unique_ptr(T* p);
	unique_ptr(unique_ptr&& p);
	unique_ptr(const unique_ptr& p) = delete;
	unique_ptr& operator=(unique_ptr&& p);
	unique_ptr& operator=(const unique_ptr& p) = delete;
	unique_ptr& operator=(T* p) = delete;
	~unique_ptr();

	T* release();
	void reset(T* p = nullptr) noexcept;
	void swap(unique_ptr& rhs) noexcept;
	T* get() noexcept;
	explicit operator bool() const noexcept;
	T& operator*() const noexcept;
	T* operator->() const noexcept;
private:
	T* ptr;
};

template<typename T>
unique_ptr<T>::unique_ptr() : ptr(nullptr) {}

template<typename T>
unique_ptr<T>::unique_ptr(T* p) : ptr(p) {}

template<typename T>
unique_ptr<T>::unique_ptr(unique_ptr&& p) {
	ptr = std::forward<T*>(p.ptr);
	p.ptr = nullptr;
}

template<typename T>
unique_ptr<T>& unique_ptr<T>::operator=(unique_ptr&& p) {
	ptr = std::forward<T*>(p.ptr);
	p.ptr = nullptr;
	return *this;
}

template<typename T>
unique_ptr<T>::~unique_ptr() {
	if(ptr) {
		delete ptr;
	}
}

template<typename T>
T* unique_ptr<T>::release() {
	T* p = ptr;
	ptr = nullptr;
	return p;
}

template<typename T>
void unique_ptr<T>::reset(T* p) noexcept {
	if(ptr) {
		delete ptr;
	}
	ptr = p;
}

template<typename T>
void unique_ptr<T>::swap(unique_ptr& rhs) noexcept {
	std::swap(ptr, rhs.ptr);
}

template<typename T>
T* unique_ptr<T>::get() noexcept {
	return ptr;
}

template<typename T>
unique_ptr<T>::operator bool() const noexcept {
	if(ptr) {
		return true;
	}
	return false;
}

template<typename T>
T& unique_ptr<T>::operator*() const noexcept {
	return *ptr;
}

template<typename T>
T* unique_ptr<T>::operator->() const noexcept {
	return ptr;
}

}
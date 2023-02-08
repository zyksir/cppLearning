#include "realvector.h"
#include <stdexcept>

//constructors
template<class valueType>
RealVector<valueType>::RealVector() : logical_size(0), array_size(kInitialSize) {
    elems = new valueType[kInitialSize];
}

template<class valueType>
RealVector<valueType>::RealVector(size_t n, const valueType& val) {
    elems = new valueType[n];
    for(size_t i = 0; i < n; i++){
        elems[i] = val;
    }
    logical_size = n;
    array_size = n;
}


//destructor
template<class valueType>
RealVector<valueType>::~RealVector() {
    delete [] elems;
}

template<class valueType>
size_t RealVector<valueType>::size() const{
    return logical_size;
}

//interface
template<class valueType>
bool RealVector<valueType>::empty() const{
    return size() == 0;
}

template<class valueType>
typename RealVector<valueType>::iterator RealVector<valueType>::begin() {
    return elems;
}

template<class valueType>
typename RealVector<valueType>::const_iterator RealVector<valueType>::begin() const{
    return elems;
}


template<class valueType>
typename RealVector<valueType>::iterator RealVector<valueType>::end() {
    return begin() + size();
} 

template<class valueType>
typename RealVector<valueType>::const_iterator RealVector<valueType>::end() const {
    return begin() + size();
} 

template<class valueType>
valueType& RealVector<valueType>::operator[](size_t index) {
    return *(begin() + index);
}

template<class valueType>
valueType& RealVector<valueType>::at(size_t index) {
    if (index >= logical_size) {
        throw std::out_of_range("Out of range.");
    }

    return operator[](index); 
}

template<class valueType>
void RealVector<valueType>::grow(){
    valueType *newElems = new valueType[array_size*2];
    for(size_t i = 0; i < logical_size; i++){
        newElems[i] = elems[i];
    }
    delete [] elems;
    array_size*=2;
    elems = newElems;
}

template<class valueType>
void RealVector<valueType>::insert(size_t pos, const valueType &elem) {
    if (pos > logical_size) {
        throw std::out_of_range("Out of range.");
    }
    if(logical_size == array_size){
        grow();
    }
    for(size_t i = logical_size; i != pos; --i){
        elems[i] = elems[i-1];
    }
    elems[pos] = elem;
    logical_size++;
}

template<class valueType>
void RealVector<valueType>::push_back(const valueType &elem) {
    insert(size(), elem);
}

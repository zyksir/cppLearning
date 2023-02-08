#include "DynamicArray.h"
#include <iostream>

DynamicArray::DynamicArray(int size) {
    this->size = size;
    array = new int[size];
}

DynamicArray::~DynamicArray() {
    delete[] array;
}

void DynamicArray::setSquaredIndices() {
    for (int i = 0; i < size; i++) {
        array[i] = i * i;
    }
}

void DynamicArray::displayElements() {
    std::cout << "Array elements: ";
    for (int i = 0; i < size; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

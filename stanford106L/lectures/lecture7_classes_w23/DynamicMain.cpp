#include "DynamicArray.h"
#include <iostream>

int main() {
    int size;
    std::cout << "Enter the size of the array: ";
    std::cin >> size;

    DynamicArray arr(size);
    arr.setSquaredIndices();
    arr.displayElements();

    return 0;
}

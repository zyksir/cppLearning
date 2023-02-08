// Header guard symbol to prevent multiple inclusion of this header file
// Define the symbol if it has not been defined
// Note: not explicitly necessary but a best practice
#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

class DynamicArray {
    private:
        int *array;
        int size;

    public:
        // Constructor to allocate memory for the array
        DynamicArray(int size);

        // Destructor to free the memory allocated for the array
        ~DynamicArray();

        // Method to set the elements of the array to the square of its index
        void setSquaredIndices();

        // Method to display the elements of the array
        void displayElements();
};

// Undefine the header guard symbol at the end of the header file
#endif

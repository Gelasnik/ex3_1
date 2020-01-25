#include <iostream>

int test(const int& number, int* array){

    array[5] = number;
    std::cout << number << std::endl;
    return number;
}

int main() {
    int testArray[10];

    int poop = 10;


    test(poop, testArray);
    std::cout << testArray[5] << std::endl;
    return 0;
}



#include <cassert>
#include <functional>
#include <iostream>
#include <cmath>
#include "UniqueArray.h"
using namespace std;


int main(){
    unsigned int size =5;
    UniqueArray<int> testArray2(size-2);
    testArray2.insert(1);
    testArray2.insert(2);
    testArray2.insert(3);

   // testArray.printArray();
    //testArray2.printArray();
    const unsigned int a=1;
    const int* b = testArray2[a];
    cout<<*b<<endl;




    return 0;

}

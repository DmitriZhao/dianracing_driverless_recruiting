#include "Lib/OpencvTest.h"
#include <iostream>

int main()
{
    OpencvTest* opencvtest = new OpencvTest();
    opencvtest->runTest();
    delete opencvtest;
    return 0;
}
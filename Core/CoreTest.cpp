#include <iostream>
#include "CoreTest.hpp"

CoreTest::CoreTest()
{
    std::cout << "Hello" << std::endl;
}

CoreTest::~CoreTest()
{
    std::cout << "GoodBye" << std::endl;
}

void CoreTest::Ready(void)
{
    std::cout << "Ready" << std::endl;
}
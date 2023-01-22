#include <Nevermore/Unit.h>

#include <cstdlib>
#include <iostream>


int main()
{
    using namespace SF;
    
    auto speed = 10_m * 2;

    if (speed == 20_m)
        std::cout << "succ" << std::endl;

    if (speed != 30_m)
        std::cout << "succ" << std::endl;

    return EXIT_SUCCESS;
}
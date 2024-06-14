#include <cstdlib>
#include <iostream>

#include "nevermore/Memory.h"

int main(int argc, char* argv[])
{
    auto ptr = sf::mmap_unique<int>(42);
    std::cout << *ptr << '\n';

    auto ptr1 = sf::mmap_unique_server<int>(100);
    auto ptr2 = sf::mmap_unique_client<int>();
    std::cout << *ptr1 << '\n';
    std::cout << *ptr2 << '\n';

    return EXIT_SUCCESS;
}
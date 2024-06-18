#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <bitset>

int main(int argc, char* argv[])
{
    std::uint64_t value = 10000000;
    {
        std::ofstream of{ "test.file" };
        of << std::bitset<sizeof(value) * 8>{ value } << '\n';
    }

    std::ifstream fs{ "test.file" };
    std::string input_value;
    fs >> input_value;

    std::cout << input_value
              << ": " << std::stoul(input_value, nullptr, 2) << '\n';

    return EXIT_SUCCESS;
}
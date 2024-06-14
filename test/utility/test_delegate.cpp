#include <cstdlib>
#include <array>
#include <iostream>

#include "nevermore/Delegate.h"

class SpiderMan {
public:
    bool attack(int x, [[maybe_unused]] int )
    {
        return x == 0;
    }
};

class CaptainAmerica {
public:
    bool attack([[maybe_unused]] int, int y) const
    {
        return y == 0;
    }
};

bool attack(int x, int y)
{
    return x == 42 && y == 42;
}

int main(int argc, char* argv[])
{
    SpiderMan spider_man{};
    CaptainAmerica captain_america{};
    std::array<sf::Delegate<bool(int, int)>, 3> heros {
        sf::Delegate{ attack },
        sf::Delegate{ &spider_man, &SpiderMan::attack },
        sf::Delegate{ &captain_america, &CaptainAmerica::attack }
    };

    for (auto& h: heros)
        std::cout << std::boolalpha << h(0, 42) << '\n';

    return EXIT_SUCCESS;
}

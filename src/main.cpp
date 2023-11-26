#include <flecs.h>

#include <iostream>

int main()
{
    flecs::world world;

    auto player = world.entity();
    std::cout << player.is_alive() << std::endl; // true!

    player.destruct();
    std::cout << player.is_alive() << std::endl; // false!

    return 0;
}
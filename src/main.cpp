#include "components.h"
#include "tags.h"

#include <flecs.h>

#include <iostream>

int main()
{
    flecs::world world;

    auto player = world.entity();
    player.add<Player>();

    return 0;
}
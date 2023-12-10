#include "modules/levelSystem.h"

#include <flecs.h>

#include <iostream>

modules::LevelSystem::LevelSystem(flecs::world& world)
{
    world.module<LevelSystem>();

    // Register components
    world.component<Level>();

    levelInit(world);
}

void modules::LevelSystem::levelInit(flecs::world& world)
{
    // Set singletons
    world.set<Level>({ 1, INITIAL_METEORS });
}

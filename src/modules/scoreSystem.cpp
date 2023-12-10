#include "modules/scoreSystem.h"

#include <flecs.h>

#include <iostream>

modules::ScoreSystem::ScoreSystem(flecs::world& world)
{
    world.module<ScoreSystem>();

    // Register components
    world.component<Level>();

    scoreInit(world);
}

void modules::ScoreSystem::scoreInit(flecs::world& world)
{
    // Set singletons
    world.set<Level>({ 1, INITIAL_METEORS });
}

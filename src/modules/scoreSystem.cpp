#include "modules/scoreSystem.h"

#include <flecs.h>

#include <iostream>

modules::ScoreSystem::ScoreSystem(flecs::world& world)
{
    world.module<ScoreSystem>();

    // Register components
    world.component<Score>();

    scoreInit(world);
}

void modules::ScoreSystem::scoreInit(flecs::world& world)
{
    // Set singleton
    world.set<Score>({ 0 });
}

#include "modules/scoreSystem.h"

#include <flecs.h>

#include <iostream>

modules::ScoreSystem::ScoreSystem(flecs::world& world)
{
    world.module<ScoreSystem>();

    // Register components
    world.component<Score>();
    world.component<Text>();

    scoreInit(world);
}

void modules::ScoreSystem::scoreInit(flecs::world& world)
{
    // Set singletons
    world.set<Score>({ 0 });
    world.set<Text>({ "Score: 0", FONT_SIZE, TEXT_COLOR, TEXT_POSITION });
}

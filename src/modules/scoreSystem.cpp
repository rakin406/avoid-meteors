#include "modules/scoreSystem.h"

#include <flecs.h>

#include <format>
#include <iostream>

modules::ScoreSystem::ScoreSystem(flecs::world& world)
{
    world.module<ScoreSystem>();

    // Register components
    world.component<Score>();
    world.component<Text>();

    scoreInit(world);

    // Observer to update text upon score change
    world.observer<const Score>("Score Change")
        .event(flecs::OnSet)
        .term_at(1)
        .singleton()
        .each(
            [](flecs::iter& it, const Score& score)
            {
                Text* text { it.world().get_mut<Text>() };
                text->content = std::format("Score: {}", score);
            });
}

void modules::ScoreSystem::scoreInit(flecs::world& world)
{
    // Set singletons
    world.set<Score>({ 0 });
    world.set<Text>({ "Score: 0", FONT_SIZE, TEXT_COLOR, TEXT_POSITION });
}

#include "modules/scoreSystem.h"
#include "components.h"
#include "graphics/renderWindow.h"

#include <flecs.h>

#include <format>
#include <iostream>

// TODO: Update the score.

modules::ScoreSystem::ScoreSystem(flecs::world& world) {
    world.module<ScoreSystem>();

    // Register components
    world.component<Score>();
    world.component<Text>();

    scoreInit(world);

    // Observer to update text texture upon score change
    world.observer<const Score>("Score Change")
        .event(flecs::OnSet)
        .term_at(1)
        .singleton()
        .iter([](flecs::iter& it, const Score* score) {
            // Get singletons
            Text* text{it.world().get_mut<Text>()};
            RenderWindow* window{it.world().get_mut<RenderWindow>()};

            // Update texture
            text->content = std::format("Score: {}", score->value);
            text->texture = window->loadTexture(text->content, text->size, text->color);
        });

    // System that loads text texture on startup
    world.system<RenderWindow, Text>("LoadTextTexture")
        .kind(flecs::OnStart)
        .term_at(1)
        .singleton()
        .term_at(2)
        .singleton()
        .each([](RenderWindow& window, Text& text) {
            text.texture = window.loadTexture(text.content, text.size, text.color);
        });
}

void modules::ScoreSystem::scoreInit(flecs::world& world) {
    // Set singletons
    world.set<Score>({0});
    world.set<Text>({"Score: 0", FONT_SIZE, nullptr, TEXT_COLOR, TEXT_POSITION});
}

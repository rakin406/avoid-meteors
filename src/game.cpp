#include "game.h"
#include "modules/player.h"
#include "modules/renderSystem.h"

#include <flecs.h>

void Game::run()
{
    init();
    while (world.progress())
    {
        // Run systems
    }
}

void Game::init()
{
    using namespace modules;

    world.import <Player>();
    world.import <RenderSystem>();
}

#include "modules/player.h"
#include "modules/renderSystem.h"

#include <flecs.h>

int main(int argv, char** args)
{
    using namespace modules;

    flecs::world world {};

    world.import <Player>();
    world.import <RenderSystem>();

    return world.app().run();
}

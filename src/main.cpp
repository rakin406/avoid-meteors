#include "modules/playerSystem.h"
#include "modules/renderSystem.h"

#include <flecs.h>

int main(int argc, char* argv[])
{
    using namespace modules;

    flecs::world world { argc, argv };

    world.import <flecs::alerts>();
    world.import <flecs::units>(); // For better visualization
    world.import <PlayerSystem>();
    world.import <RenderSystem>();

    // Run application with REST interface
    return world.app().enable_rest().run();
}

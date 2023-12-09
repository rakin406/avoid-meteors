#include "game.h"
#include "collisionLayer.h"
#include "components.h"
#include "modules/player.h"
#include "modules/renderSystem.h"
#include "tags.h"

#include <flecs.h>

#include <iostream>

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

    // System that checks for collisions between entities
    world.system<Collider>("Collision")
        .kind(flecs::PostUpdate)
        .each(
            [](flecs::entity entity, Collider)
            {
                if (entity.has<Player::PlayerTag>())
                {
                    const Transform* transform { entity.get<Transform>() };
                    // NOTE: I have no idea how the hell this is working...
                    if (transform->position.x <= -Player::FRAME_SIZE ||
                        transform->position.x >=
                            (RenderSystem::WINDOW_WIDTH -
                             (Player::FRAME_SIZE * (Player::FRAME_SCALE - 1))))
                    {
                        entity.add<CollidesWith, CollisionLayer::Wall>();
                    }
                }
            });
}

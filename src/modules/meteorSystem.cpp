#include "modules/meteorSystem.h"
#include "collisionLayer.h"
#include "components.h"
#include "constants.h"
#include "renderWindow.h"
#include "tags.h"
#include "tools.h"

#include "SDL.h"
#include <flecs.h>

#include <iostream>
#include <ranges>

modules::MeteorSystem::MeteorSystem(flecs::world& world)
{
    using namespace tools;

    world.module<MeteorSystem>();

    // Register components
    world.component<CollisionLayer>();
    world.component<CollisionMask>();
    world.component<Level>();
    world.component<SpriteRenderer>();
    world.component<Sprite>();
    world.component<Transform>();
    world.component<Velocity>();

    // Setup meteors
    meteorsInit(world);

    // System that loads meteor components on startup
    world.system<RenderWindow, Sprite, Transform, Meteor>("LoadMeteors")
        .kind(flecs::OnStart)
        .term_at(1)
        .singleton()
        .each(
            [](RenderWindow& window, Sprite& sprite, Transform& transform,
               Meteor)
            {
                // Load meteor sprite
                sprite.texture = window.loadTexture(METEOR_SPRITE);

                // Get random x-axis position
                SDL_FPoint textureSize { getSize(sprite.texture) };
                float randomPosX { getRandomValue<float>(
                    0.0f, static_cast<float>(WINDOW_WIDTH) - textureSize.x) };

                // Set meteor position in the sky
                transform.position = { randomPosX, -textureSize.y };
            });

    // System that moves meteor entities
    world.system<Transform, const Velocity, Meteor>("Move")
        .kind(flecs::OnUpdate)
        .each(
            [](flecs::iter& it, size_t index, Transform& transform,
               const Velocity& vel, Meteor)
            {
                auto meteor { it.entity(index) };

                // TODO: Remove this.
                if (!meteor.has<CollisionMask::LeftWall>())
                {
                    transform.position.x -= vel.x * it.delta_time();
                }
                else if (!meteor.has<CollisionMask::RightWall>())
                {
                    transform.position.x += vel.x * it.delta_time();
                }
            });
}

void modules::MeteorSystem::meteorsInit(flecs::world& world)
{
    const Level* level { world.get<Level>() };

    for (int i : std::views::iota(0, level->numMeteors))
    {
        float randomScale { tools::getRandomValue<float>(MIN_SCALE,
                                                         MAX_SCALE) };

        auto meteor { world.entity() };
        meteor.add<Meteor>()
            .add<CollisionLayer::Meteor>()
            .add<CollisionMask::Ground>()
            .add<CollisionMask::Player>()
            .add<SpriteRenderer>()
            .set<Sprite>({ nullptr, nullptr })
            .set<Transform>({ {}, 0.0f, { randomScale, randomScale } })
            .set<Velocity>({ INITIAL_SPEED, INITIAL_SPEED });
    }
}

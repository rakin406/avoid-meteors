#include "modules/meteorSystem.h"
#include "collisionLayer.h"
#include "components.h"
#include "constants.h"
#include "renderWindow.h"
#include "tags.h"
#include "tools.h"

#include "SDL.h"
#include <flecs.h>

#include <cmath>
#include <iostream>
#include <ranges>

modules::MeteorSystem::MeteorSystem(flecs::world& world)
{
    using namespace tools;

    world.module<MeteorSystem>();

    // Register components
    world.component<Angle>();
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
    world.system<RenderWindow, Angle, Sprite, Transform, Meteor>("LoadMeteors")
        .kind(flecs::OnStart)
        .term_at(1)
        .singleton()
        .each(
            [](RenderWindow& window, Angle& angle, Sprite& sprite,
               Transform& transform, Meteor)
            {
                // Load meteor sprite
                sprite.texture = window.loadTexture(METEOR_SPRITE);

                // NOTE: This is probably not accurate.
                // Get random x-axis position
                SDL_FPoint textureSize { getSize(sprite.texture) };
                float randomPosX { getRandomValue<float>(
                    0.0f, static_cast<float>(WINDOW_WIDTH) - textureSize.x) };

                // Set meteor position in the sky
                transform.position = { randomPosX, -textureSize.y };

                {
                    // NOTE: Probably not accurate too.
                    float middlePosX { static_cast<float>(WINDOW_WIDTH / 2) -
                                       ((textureSize.x * transform.scale.x) /
                                        2) };

                    float opposite {};
                    float adjacent { static_cast<float>(WINDOW_HEIGHT) };

                    // Find opposite side length
                    if (transform.position.x <= middlePosX)
                    {
                        opposite = static_cast<float>(WINDOW_WIDTH) -
                                   transform.position.x;
                    }
                    else if (transform.position.x >= middlePosX)
                    {
                        opposite = transform.position.x;
                    }

                    // NOTE: Not accurate as well I guess.
                    // Set random angle
                    angle = tools::getRandomValue<float>(
                        0.0f, std::tan(opposite / adjacent));
                }
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
            .add<Angle>()
            .set<Sprite>({ nullptr, nullptr })
            .set<Transform>({ {}, 0.0f, { randomScale, randomScale } })
            .set<Velocity>({ INITIAL_SPEED, INITIAL_SPEED });
    }
}

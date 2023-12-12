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

// TODO: Create separate collision systems that match specific collision
// masks. That would make more sense rather than adding and removing the
// collision masks.

namespace
{
    /**
     * @brief Handles meteor and ground collision.
     * @param meteor Meteor entity.
     * @param transform Transform component.
     */
    void handleGroundCollision(flecs::entity& meteor,
                               const Transform& transform)
    {
        if (transform.position.y == GROUND_POS_Y)
        {
            meteor.remove<CollisionMask::Ground>();
        }
    }
} // namespace

modules::MeteorSystem::MeteorSystem(flecs::world& world)
{
    using namespace tools;

    world.module<MeteorSystem>();

    // Register components
    world.component<Angle>();
    world.component<CollisionLayer>();
    world.component<CollisionMask>();
    world.component<Direction>();
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
            [](flecs::entity meteor, RenderWindow& window, Angle& angle,
               Sprite& sprite, Transform& transform, Meteor)
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
                        meteor.add(Direction::Right);
                    }
                    else if (transform.position.x >= middlePosX)
                    {
                        opposite = transform.position.x;
                        meteor.add(Direction::Left);
                    }

                    // NOTE: Not accurate as well I guess.
                    // Set random angle
                    angle = tools::getRandomValue<float>(
                        0.0f, std::tan(opposite / adjacent));
                }
            });

    // System that moves meteor entities
    world.system<Transform, const Angle, const Velocity, Meteor>("Move")
        .kind(flecs::OnUpdate)
        .each(
            [](flecs::iter& it, size_t index, Transform& transform,
               const Angle& angle, const Velocity& velocity, Meteor)
            {
                auto meteor { it.entity(index) };

                // Move meteor based on direction
                if (meteor.has(Direction::Left))
                {
                    transform.position.x -=
                        (velocity.x * std::cos(angle)) * it.delta_time();
                }
                else if (meteor.has(Direction::Right))
                {
                    transform.position.x +=
                        (velocity.x * std::cos(angle)) * it.delta_time();
                }

                transform.position.y +=
                    (velocity.y * std::sin(angle)) * it.delta_time();
            });

    // System that rotates meteor entities
    world.system<Transform, Meteor>("Rotate")
        .kind(flecs::OnUpdate)
        .with<Direction>(flecs::Wildcard)
        .iter(
            [](flecs::iter& it, Transform* transform, Meteor*)
            {
                auto direction { it.pair(3).second().to_constant<Direction>() };

                switch (direction)
                {
                case Direction::Left:
                    transform->rotation -= ROTATION_SPEED * it.delta_time();
                    break;
                case Direction::Right:
                    transform->rotation += ROTATION_SPEED * it.delta_time();
                    break;
                default:
                    break;
                }
            });

    // System that checks for collisions between meteor and other entities
    world.system<const Transform, CollisionLayer::Meteor>("MeteorCollision")
        .kind(flecs::PostUpdate)
        .each([](flecs::entity meteor, const Transform& transform,
                 CollisionLayer::Meteor)
              { handleGroundCollision(meteor, transform); });
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

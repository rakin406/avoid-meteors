#include "modules/meteorSystem.h"
#include "collisionLayer.h"
#include "components.h"
#include "constants.h"
#include "graphics/renderWindow.h"
#include "tags.h"
#include "tools.h"

#include <SDL2/SDL.h>
#include <flecs.h>
#include <glm/glm.hpp>

#include <cmath>
#include <iostream>
#include <ranges>

// TODO: Create separate collision systems that match specific collision
// masks. That would make more sense rather than adding and removing the
// collision masks.

namespace {
/**
 * @brief Creates meteor entity with components.
 * @param world
 */
void createMeteor(flecs::world& world) {
    using namespace modules;

    const float randomScale{
        tools::getRandomValue<float>(MeteorSystem::MIN_SCALE, MeteorSystem::MAX_SCALE)};

    auto meteor{world.entity()};

    meteor.add<Meteor>()
        .add<CollisionLayer::Meteor>()
        .add<CollisionMask::Ground>()
        .add<CollisionMask::Player>()
        .add<SpriteRenderer>()
        .add<MeteorSystem::Angle>()
        .add<Sprite>()
        .set<Transform>({{}, 0.0f, {randomScale, randomScale}})
        .set<Velocity>({MeteorSystem::INITIAL_SPEED, MeteorSystem::INITIAL_SPEED});
}

/**
 * @brief Handles meteor and ground collision.
 * @param meteor Meteor entity.
 * @param transform Transform component.
 */
void handleGroundCollision(flecs::entity& meteor, const Transform& transform) {
    if (transform.position.y == GROUND_POS_Y) {
        meteor.remove<CollisionMask::Ground>();
    }
}
}  // namespace

modules::MeteorSystem::MeteorSystem(flecs::world& world) {
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

    // Observer to destroy meteor upon ground collision
    world.observer<CollisionMask::Ground>("Ground Collision")
        .event(flecs::OnRemove)
        .each([&](flecs::entity meteor, CollisionMask::Ground) {
            Sprite* sprite{meteor.get_mut<Sprite>()};
            SDL_DestroyTexture(sprite->texture);
            meteor.destruct();
            createMeteor(world);  // Replace destroyed meteor
        });

    // System that loads meteor components on startup
    world.system<RenderWindow, Angle, Sprite, Transform, Meteor>("LoadMeteors")
        .kind(flecs::OnStart)
        .term_at(1)
        .singleton()
        .each([](flecs::entity meteor, RenderWindow& window, Angle& angle,
                 Sprite& sprite, Transform& transform, Meteor) {
            // Load meteor sprite
            sprite.texture = window.loadTexture(METEOR_SPRITE);

            // NOTE: This is probably not accurate.
            // Get random x-axis position
            glm::vec2 textureSize{getSize(sprite.texture)};
            float randomPosX{getRandomValue<float>(
                0.0f, static_cast<float>(WINDOW_WIDTH) - textureSize.x)};

            // Set meteor position in the sky
            transform.position = {randomPosX, -textureSize.y};

            {
                // NOTE: Probably not accurate too.
                float middlePosX{static_cast<float>(WINDOW_WIDTH / 2) -
                                 ((textureSize.x * transform.scale.x) / 2)};

                float opposite{};
                float adjacent{static_cast<float>(WINDOW_HEIGHT)};

                // Find opposite side length
                if (transform.position.x <= middlePosX) {
                    opposite = static_cast<float>(WINDOW_WIDTH) - transform.position.x;
                    meteor.add(Direction::Right);
                } else if (transform.position.x >= middlePosX) {
                    opposite = transform.position.x;
                    meteor.add(Direction::Left);
                }

                // NOTE: Not accurate as well I guess.
                // Set random angle
                angle.value =
                    tools::getRandomValue<float>(0.0f, std::atan(opposite / adjacent));
            }
        });

    // System that moves meteor entities
    world.system<Transform, const Angle, const Velocity, Meteor>("Move")
        .kind(flecs::OnUpdate)
        .each([](flecs::iter& it, size_t index, Transform& transform,
                 const Angle& angle, const Velocity& velocity, Meteor) {
            auto meteor{it.entity(index)};

            // Move meteor based on direction
            if (meteor.has(Direction::Left)) {
                transform.position.x -=
                    (velocity.x * std::cos(angle.value)) * it.delta_time();
            } else if (meteor.has(Direction::Right)) {
                transform.position.x +=
                    (velocity.x * std::cos(angle.value)) * it.delta_time();
            }

            transform.position.y +=
                (velocity.y * std::sin(angle.value)) * it.delta_time();
        });

    // System that checks for collisions between meteor and other entities
    world.system<const Transform, CollisionLayer::Meteor>("MeteorCollision")
        .kind(flecs::PostUpdate)
        .each([](flecs::entity meteor, const Transform& transform,
                 CollisionLayer::Meteor) { handleGroundCollision(meteor, transform); });
}

void modules::MeteorSystem::meteorsInit(flecs::world& world) {
    const Level* level{world.get<Level>()};

    for (int i : std::views::iota(0, level->numMeteors)) {
        createMeteor(world);
    }
}

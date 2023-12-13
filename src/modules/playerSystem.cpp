#include "modules/playerSystem.h"
#include "collisionLayer.h"
#include "components.h"
#include "constants.h"
#include "renderWindow.h"
#include "tags.h"
#include "tools.h"

#include <flecs.h>
#include <SDL2/SDL.h>

#include <array>
#include <iostream>

namespace
{
    /**
     * @brief Handles player and wall collision.
     * @param player Player entity.
     * @param transform Transform component.
     */
    void handleWallCollision(flecs::entity& player, const Transform& transform)
    {
        using namespace modules;

        // NOTE: I have no idea how the hell this is working...
        if (transform.position.x <= -PlayerSystem::FRAME_SIZE)
        {
            player.add<CollisionMask::LeftWall>();
        }
        else if (transform.position.x >=
                 (WINDOW_WIDTH -
                  (PlayerSystem::FRAME_SIZE * (PlayerSystem::FRAME_SCALE - 1))))
        {
            player.add<CollisionMask::RightWall>();
        }
        else
        {
            // NOTE: I feel like this is inefficient because the system
            // will try to remove the tags even if they don't exist.
            // Maybe find an alternative way? Observers?? Idk...
            player.remove<CollisionMask::LeftWall>();
            player.remove<CollisionMask::RightWall>();
        }
    }
} // namespace

modules::PlayerSystem::PlayerSystem(flecs::world& world)
{
    world.module<PlayerSystem>();

    // Register components
    world.component<Animation>();
    world.component<Player>();
    world.component<Movement>();
    world.component<Direction>();
    world.component<CollisionLayer>();
    world.component<CollisionMask>();
    world.component<SpriteRenderer>();
    world.component<Sprite>();
    world.component<Transform>();
    world.component<Velocity>();

    // Setup player entity
    playerInit(world);

    // System that loads player sprite sheet on startup
    world.system<RenderWindow, Sprite, Player>("LoadPlayerSprite")
        .kind(flecs::OnStart)
        .term_at(1)
        .singleton()
        .each([](RenderWindow& window, Sprite& sprite, Player)
              { sprite.texture = window.loadTexture(SPRITE_SHEET); });

    // System that processes player input
    world.system<Direction, Player>("Input")
        .kind(flecs::PreUpdate)
        .with<Movement>(flecs::Wildcard)
        .each(
            [](flecs::entity player, Direction& direction, Player)
            {
                SDL_PumpEvents();
                const Uint8* keyState { SDL_GetKeyboardState(nullptr) };

                // Continuous-response keys
                if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A])
                {
                    player.add(Movement::Running);
                    direction = LEFT;
                }
                else if (keyState[SDL_SCANCODE_RIGHT] ||
                         keyState[SDL_SCANCODE_D])
                {
                    player.add(Movement::Running);
                    direction = RIGHT;
                }
                else
                {
                    player.add(Movement::Idle);
                }
            });

    // System that moves player entity
    world.system<Transform, const Direction, const Velocity, Player>("Move")
        .kind(flecs::OnUpdate)
        .with(Movement::Running)
        .each(
            [](flecs::iter& it, size_t index, Transform& transform,
               const Direction& direction, const Velocity& velocity, Player)
            {
                auto player { it.entity(index) };

                // Move player unless there's a collision
                // if (!player.has<CollisionMask::LeftWall>() &&
                //    !player.has<CollisionMask::RightWall>())
                //{
                //    transform.position.x +=
                //        direction.x * velocity.x * it.delta_time();
                //}

                // FIX: Player only moving right.
                transform.position.x +=
                    direction.x * velocity.x * it.delta_time();
                std::cout << transform.position.x << std::endl;
            });

    // System that animates player entity
    world.system<const Direction, Animation>("Animate")
        .kind(flecs::OnUpdate)
        .with<Movement>(flecs::Wildcard)
        .iter(
            [](flecs::iter& it, const Direction* direction,
               Animation* animation)
            {
                // Get the current value of the states
                auto movement { it.pair(3).second().to_constant<Movement>() };

                // Calculate the current frame based on time
                Uint32 currentTime { SDL_GetTicks() };
                // NOTE: This assumes you have 6 frames in each row.
                int currentFrame { (currentTime / animation->frameDuration) %
                                   6 };

                // Update the x-coordinate of the source rectangle
                animation->frameRec.x = animation->frameRec.w * currentFrame;

                switch (movement)
                {
                case Movement::Idle:
                    animation->frameRec.y = 0;
                    break;
                case Movement::Running:
                    animation->frameRec.y = animation->frameRec.h;
                    break;
                default:
                    break;
                }

                // Add a flip based on direction
                if (*direction == LEFT)
                {
                    animation->flip = SDL_FLIP_HORIZONTAL;
                }
                else if (*direction == RIGHT)
                {
                    animation->flip = SDL_FLIP_NONE;
                }
            });

    // System that checks for collisions between player and other entities
    world.system<const Transform, CollisionLayer::Player>("PlayerCollision")
        .kind(flecs::PostUpdate)
        .each([](flecs::entity player, const Transform& transform,
                 CollisionLayer::Player)
              { handleWallCollision(player, transform); });
}

void modules::PlayerSystem::playerInit(flecs::world& world)
{
    // Used for random starting direction
    static constexpr std::array<Direction, 2> ALL_DIRECTIONS { LEFT, RIGHT };

    Direction randomDirection { ALL_DIRECTIONS[tools::getRandomValue<int>(
        0, static_cast<int>(ALL_DIRECTIONS.size()) - 1)] };

    auto player { world.entity("Player") };

    // Set player components
    player.add<Player>()
        .add<CollisionLayer::Player>()
        .add<CollisionMask::Meteor>()
        .add<CollisionMask::LeftWall>()
        .add<CollisionMask::RightWall>()
        .add<SpriteRenderer>()
        .add(Movement::Idle)
        .set<Animation>({ { 0, 0, static_cast<int>(FRAME_SIZE),
                            static_cast<int>(FRAME_SIZE) },
                          SDL_FLIP_NONE,
                          FRAME_DURATION })
        .set<Direction>(randomDirection)
        .set<Sprite>({ nullptr, nullptr })
        .set<Transform>(
            { INITIAL_POSITION, 0.0f, { FRAME_SCALE, FRAME_SCALE } })
        .set<Velocity>({ SPEED, 0.0f });
}

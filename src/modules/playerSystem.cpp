#include "modules/playerSystem.h"
#include "collisionLayer.h"
#include "components.h"
#include "renderWindow.h"
#include "tags.h"
#include "tools.h"

#include "SDL.h"
#include <flecs.h>

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
                 (RenderSystem::WINDOW_WIDTH -
                  (PlayerSystem::FRAME_SIZE * (Player::FRAME_SCALE - 1))))
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
    world.component<Sprite>();
    world.component<Transform>();
    world.component<Velocity>();

    // Setup player entity
    playerInit(world);

    // System that loads player sprite sheet on startup
    world.system<RenderWindow, Sprite, Player>("LoadSpriteSheet")
        .kind(flecs::OnStart)
        .term_at(1)
        .singleton()
        .each(
            [](RenderWindow& window, Sprite& sprite, Player)
            {
                // FIX: Sprite sheet not loading.
                std::cout << "Loading sprite sheet...\n";
                sprite.texture = window.loadTexture(SPRITE_SHEET);
            });

    // System that processes player input
    world.system<Player>("Input")
        .kind(flecs::PreUpdate)
        .with<Movement>(flecs::Wildcard)
        .with<Direction>(flecs::Wildcard)
        .each(
            [](flecs::entity player, Player)
            {
                SDL_PumpEvents();
                const Uint8* keyState { SDL_GetKeyboardState(nullptr) };

                // Continuous-response keys
                if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A])
                {
                    player.add(Movement::Running);
                    player.add(Direction::Left);
                }
                else if (keyState[SDL_SCANCODE_RIGHT] ||
                         keyState[SDL_SCANCODE_D])
                {
                    player.add(Movement::Running);
                    player.add(Direction::Right);
                }
                else
                {
                    player.add(Movement::Idle);
                }
            });

    // System that moves player entity
    world.system<Transform, const Velocity, Player>("Move")
        .kind(flecs::OnUpdate)
        .with(Movement::Running)
        .with<Direction>(flecs::Wildcard)
        .each(
            [](flecs::iter& it, size_t index, Transform& transform,
               const Velocity& vel, Player)
            {
                auto player { it.entity(index) };
                auto direction { it.pair(5).second().to_constant<Direction>() };

                // Move player unless there's a collision
                if (direction == Direction::Left &&
                    !player.has<CollisionMask::LeftWall>())
                {
                    transform.position.x -= vel.x * it.delta_time();
                }
                else if (direction == Direction::Right &&
                         !player.has<CollisionMask::RightWall>())
                {
                    transform.position.x += vel.x * it.delta_time();
                }
            });

    // System that animates player entity
    world.system<Animation>("Animate")
        .kind(flecs::OnUpdate)
        .with<Movement>(flecs::Wildcard)
        .with<Direction>(flecs::Wildcard)
        .iter(
            [](flecs::iter& it, Animation* animation)
            {
                // Get the current value of the states
                auto movement { it.pair(2).second().to_constant<Movement>() };
                auto direction { it.pair(3).second().to_constant<Direction>() };

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
                switch (direction)
                {
                case Direction::Left:
                    animation->flip = SDL_FLIP_HORIZONTAL;
                    break;
                case Direction::Right:
                    animation->flip = SDL_FLIP_NONE;
                    break;
                default:
                    break;
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
    static constexpr std::array<Direction, 2> ALL_DIRECTIONS {
        Direction::Left, Direction::Right
    };

    Direction randomDirection { ALL_DIRECTIONS[tools::getRandomValue(
        0, static_cast<int>(ALL_DIRECTIONS.size() - 1))] };

    auto player { world.entity("Player") };

    // Set player components
    player.add<Player>()
        .add<CollisionLayer::Player>()
        .add<CollisionMask::LeftWall>()
        .add<CollisionMask::RightWall>()
        .add<RenderSystem::SpriteRenderer>()
        .add(Movement::Idle)
        .add(randomDirection)
        .add<Sprite>()
        .set<Animation>({ { 0, 0, static_cast<int>(FRAME_SIZE),
                            static_cast<int>(FRAME_SIZE) },
                          SDL_FLIP_NONE,
                          FRAME_DURATION })
        .set<Transform>(
            { STARTING_POSITION, 0.0f, { FRAME_SCALE, FRAME_SCALE } })
        .set<Velocity>({ SPEED, 0.0f });
}

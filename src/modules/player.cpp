#include "modules/player.h"
#include "components.h"
#include "states.h"
#include "tags.h"

#include "SDL.h"
#include <flecs.h>

#include <iostream>

modules::Player::Player(flecs::world& world)
{
    world.module<Player>();
    world.component<tags::Player>();
    world.component<tags::SpriteRenderer>();
    world.component<Movement>();
    world.component<Direction>();
    world.component<Transform>();
    world.component<Sprite>();
    world.component<Velocity>();
    world.component<Animation>();

    // auto lastDirection { &randomDirection };

    // FIX: Output only once.
    // Create an observer for direction change
    // world.observer<>()
    //    .event(flecs::OnAdd)
    //    .term<Direction>()
    //    .each(
    //        [](flecs::entity entity)
    //        {
    //            // Add a flip based on direction
    //            std::cout << "Direction changed\n";
    //            Animation* animation { entity.get_mut<Animation>() };
    //            animation->flip = SDL_FLIP_HORIZONTAL;
    //        });

    world.system<Transform, const Velocity, tags::Player>("MovementSystem")
        .iter(
            [](flecs::iter& it, Transform* transform, const Velocity* vel,
               tags::Player*)
            {
                SDL_PumpEvents();
                const Uint8* keyState { SDL_GetKeyboardState(nullptr) };

                std::cout << "Moving\n";

                // Continuous-response keys
                if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A])
                {
                    it.world().set(Movement::Running);
                    it.world().set(Direction::Left);
                    transform->position.x -= vel->x * it.delta_time();
                }
                else if (keyState[SDL_SCANCODE_RIGHT] ||
                         keyState[SDL_SCANCODE_D])
                {
                    it.world().set(Movement::Running);
                    it.world().set(Direction::Right);
                    transform->position.x += vel->x * it.delta_time();
                }
                else
                {
                    it.world().set(Movement::Idle);
                }
            });

    world.system<Animation>("AnimationSystem")
        .with<Movement>(flecs::Wildcard)
        .singleton()
        .with<Direction>(flecs::Wildcard)
        .singleton()
        .iter(
            [](flecs::iter& it, Animation* animation)
            {
                std::cout << "Animating\n";

                // Get the current value of the states
                auto movement { it.pair(1).second().to_constant<Movement>() };
                auto direction { it.pair(1).second().to_constant<Direction>() };

                // if (movement == Movement::Idle)
                //{
                //     std::cout << "Idle\n";
                // }

                // Calculate the current frame based on time
                Uint32 currentTime { SDL_GetTicks() };
                // NOTE: This assumes you have 6 frames in each row.
                int currentFrame { (currentTime / animation->frameDuration) %
                                   6 };

                // Update the x-coordinate of the source rectangle
                animation->frameRec.x = animation->frameRec.w * currentFrame;

                // TODO: Turn into a separate function.
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
                // if (direction == lastDirection)
                //{
                //    std::cout << "same direction\n";
                //    animation.flip = SDL_FLIP_NONE;
                //}
                // else
                //{
                //    std::cout << "different direction\n";
                //    animation.flip = SDL_FLIP_HORIZONTAL;
                //    lastDirection = direction;
                //}
            });
}

#include "game.h"
#include "colors.h"
#include "components.h"
#include "constants.h"
#include "modules/player.h"
#include "renderWindow.h"
#include "states.h"
#include "tags.h"
#include "tools.h"

#include "SDL.h"
#include "SDL_image.h"
#include <flecs.h>

#include <array>
#include <iostream>

namespace
{
    constexpr std::array<Direction, 2> ALL_DIRECTIONS { Direction::Left,
                                                        Direction::Right };

    /**
     * @brief Returns true if user requests quit. For use in main loop.
     * @param event SDL_Event&
     * @return bool
     */
    bool isQuitRequested(const SDL_Event& event)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return true;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                return true;
            break;
        default:
            break;
        }

        return false;
    }

} // namespace

Game::Game()
    : running { true },
      window { constants::window::WIDTH, constants::window::HEIGHT,
               constants::window::TITLE },
      background { window.loadTexture(constants::assets::BACKGROUND) }
{
}

void Game::run()
{
    init();
    while (running)
        update();
    stop();
}

void Game::init()
{
    using namespace constants;

    world.import <modules::Player>();

    // Set singleton
    world.add<tags::Player>();

    Direction randomDirection { ALL_DIRECTIONS[tools::getRandomValue(
        0, static_cast<int>(ALL_DIRECTIONS.size() - 1))] };

    world.component<Movement>().add(Movement::Idle);
    world.component<Direction>().add(randomDirection);

    // TODO: Set sdl_rect nullptr.
    world.set<Animation>({ { 0, 0, static_cast<int>(player::FRAME_SIZE),
                             static_cast<int>(player::FRAME_SIZE) },
                           { player::FRAME_SIZE, player::FRAME_SIZE },
                           nullptr,
                           player::FRAME_DURATION });

    world
        .system<const Transform, const Sprite, const tags::SpriteRenderer>(
            "SpriteRendererSystem")
        .each(
            [this](flecs::entity entity, const Transform& transform,
                   const Sprite& sprite, tags::SpriteRenderer)
            {
                // Render player
                if (world.has<tags::Player>() && world.has<Animation>())
                {
                    Animation* animation { world.get_mut<Animation>() };
                    SDL_FRect dest { transform.position.x, transform.position.y,
                                     animation->frameSize.x * transform.scale.x,
                                     animation->frameSize.y *
                                         transform.scale.y };
                    window.render(sprite.texture, &animation->frameRec, &dest,
                                  transform.rotation, nullptr, sprite.color,
                                  animation->flip);
                }
            });

    auto player { world.entity("Player") };
    SDL_Texture* playerSprite { window.loadTexture(assets::PLAYER_SHEET) };

    // Set player components
    player.add<tags::SpriteRenderer>()
        .set<Transform>({ player::STARTING_POSITION,
                          0.0f,
                          { player::FRAME_SCALE, player::FRAME_SCALE } })
        .set<Sprite>({ playerSprite, nullptr })
        .set<Velocity>({ player::SPEED, 0.0f });
}

void Game::update()
{
    // Get our controls and events
    while (SDL_PollEvent(&event))
    {
        if (isQuitRequested(event))
            running = false;
    }

    window.clear(WHITE);

    // Draw the background
    window.render(background, nullptr, nullptr, 0, nullptr);

    world.progress();

    window.display();
}

void Game::stop() { window.close(); }

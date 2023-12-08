/**
 * @file
 * @brief Provides player module.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "constants.h"

#include "SDL.h"
#include <flecs.h>

#include <string_view>

namespace modules
{
    /**
     * @brief Player module.
     */
    class Player
    {
    public:
        // -- Constants

        static constexpr std::string_view SPRITE_SHEET { PROJECT_ROOT
                                                         "res/gfx/player.png" };
        static constexpr float SPEED { 300.0f };
        static constexpr float FRAME_SCALE { 5.0f };
        static constexpr float FRAME_SIZE { 32.0f };

        // NOTE: I don't know what I'm doing here lol
        static constexpr int FRAME_DURATION { static_cast<int>(SPEED) / 2 };
        static constexpr SDL_FPoint STARTING_POSITION {
            (constants::window::WIDTH / 2) - (FRAME_SIZE * (FRAME_SCALE / 2)),
            constants::window::HEIGHT - 195 // Ground y-axis
        };

        // -- Components

        /**
         * @brief Manages animations.
         */
        struct Animation
        {
            SDL_Rect frameRec {};
            SDL_RendererFlip flip {};
            int frameDuration {};
        };

        /**
         * @brief Stores input-related data.
        */
        struct Input
        {
            bool left {};
            bool right {};
        };

        struct PlayerTag
        {
        };

        /**
         * @brief Movement state.
         */
        enum class Movement
        {
            Idle,
            Running
        };

        /**
         * @brief Direction state.
         */
        enum class Direction
        {
            Left,
            Right
        };

        /**
         * @brief Module import function.
         * @param world
         */
        Player(flecs::world& world);

    private:
        /**
         * @brief Initializes player entity and components.
         * @param world
         */
        static void playerInit(flecs::world& world);
    };
} // namespace modules

#endif

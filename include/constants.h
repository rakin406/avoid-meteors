#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "SDL.h"

#include <string_view>

namespace constants::assets
{
    inline constexpr std::string_view BACKGROUND { PROJECT_ROOT
                                                   "res/gfx/background.jpg" };
    inline constexpr std::string_view PLAYER_SHEET { PROJECT_ROOT
                                                     "res/gfx/player.png" };
} // namespace constants::assets

namespace constants::window
{
    inline constexpr int WIDTH { 1280 };
    inline constexpr int HEIGHT { 720 };
    inline constexpr std::string_view TITLE { "Avoid Meteors" };
} // namespace constants::window

namespace constants::player
{
    inline constexpr int SPEED { 10 };
    inline constexpr SDL_Point FRAME_SCALE { 5, 5 };
    inline constexpr SDL_Point FRAME_SIZE { 32, 32 };
    inline constexpr SDL_Point STARTING_POSITION {
        (window::WIDTH / 2) - (FRAME_SIZE.x * (FRAME_SCALE.x / 2)),
        window::HEIGHT - 195 // Ground y-axis
    };
} // namespace constants::player

#endif

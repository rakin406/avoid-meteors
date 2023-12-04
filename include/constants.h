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

namespace constants::player
{
    inline constexpr int SPEED { 10 };
    inline constexpr SDL_Point SCALE { 4, 4 };
    inline constexpr SDL_Point FRAME_SIZE { 32, 32 };
} // namespace constants::player

namespace constants::window
{
    inline constexpr int WIDTH { 1280 };
    inline constexpr int HEIGHT { 720 };
    inline constexpr std::string_view TITLE { "Avoid Meteors" };
} // namespace constants::window

#endif

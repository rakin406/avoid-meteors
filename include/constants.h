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
    inline constexpr float SPEED { 300.0f };
    inline constexpr float FRAME_SCALE { 5.0f };
    inline constexpr float FRAME_SIZE { 32.0f };
    // NOTE: I don't know what I'm doing here lol
    inline constexpr int FRAME_DURATION { static_cast<int>(SPEED) / 2 };
    inline constexpr SDL_FPoint STARTING_POSITION {
        (window::WIDTH / 2) - (FRAME_SIZE * (FRAME_SCALE / 2)),
        window::HEIGHT - 195 // Ground y-axis
    };
} // namespace constants::player

#endif

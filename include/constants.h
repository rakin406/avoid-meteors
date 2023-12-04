#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "states.h"

#include "SDL.h"

#include <array>
#include <string_view>

namespace constants
{
    inline constexpr int WINDOW_WIDTH { 1280 };
    inline constexpr int WINDOW_HEIGHT { 720 };
    inline constexpr int GROUND_POS_Y { WINDOW_HEIGHT - 10 };
    inline constexpr int PLAYER_SPEED { 10 };
    inline constexpr SDL_Point PLAYER_SCALE { 4, 4 };
    inline constexpr SDL_Point FRAME_SIZE { 32, 32 };

    inline constexpr std::string_view BG_IMG_PATH { PROJECT_ROOT
                                                    "res/gfx/background.jpg" };
    inline constexpr std::string_view PLAYER_SHEET_PATH {
        PROJECT_ROOT "res/gfx/player.png"
    };

    inline constexpr std::array<Direction, 2> ALL_DIRECTIONS {
        Direction::Left, Direction::Right
    };
} // namespace constants

#endif
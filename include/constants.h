/**
 * @file
 * @brief Provides game related constants.
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string_view>

namespace constants::assets
{
    inline constexpr std::string_view BACKGROUND { PROJECT_ROOT
                                                   "res/gfx/background.jpg" };
} // namespace constants::assets

namespace constants::window
{
    inline constexpr int WIDTH { 1280 };
    inline constexpr int HEIGHT { 720 };
    inline constexpr std::string_view TITLE { "Avoid Meteors" };
} // namespace constants::window

#endif

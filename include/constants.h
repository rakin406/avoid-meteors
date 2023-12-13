/**
 * @file
 * @brief Contains global constants.
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <glm/glm.hpp>

#include <string_view>

inline constexpr int WINDOW_WIDTH{1280};
inline constexpr int WINDOW_HEIGHT{720};
inline constexpr std::string_view WINDOW_TITLE{"Avoid Meteors"};

inline constexpr int GROUND_POS_Y{WINDOW_HEIGHT - 195};

// Direction constants
inline constexpr glm::vec2 LEFT{-1, 0};  // Left unit vector
inline constexpr glm::vec2 RIGHT{1, 0};  // Right unit vector

#endif

/**
 * @file
 * @brief Provides bitmasks.
 */

#ifndef ENUM_FLAGS_H
#define ENUM_FLAGS_H

#include <magic_enum.hpp>

enum class CollisionFlags
{
    Barrier = 1 << 0,
    Ground = 1 << 1,
    Meteor = 1 << 2,
    Player = 1 << 3
};

// Add specialization `is_flags` to define that enum are flags.
template <>
struct magic_enum::customize::enum_range<CollisionFlags>
{
    static constexpr bool is_flags = true;
};

#endif

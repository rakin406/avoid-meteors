/**
 * @file
 * @brief Provides collision layer for the entities.
 */

#ifndef COLLISION_LAYER_H
#define COLLISION_LAYER_H

struct CollisionLayer
{
    struct Ground
    {
    };

    struct Meteor
    {
    };

    struct Player
    {
    };

    enum class Wall
    {
        Left,
        Right
    };
};

#endif

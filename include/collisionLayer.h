/**
 * @file
 * @brief Provides collision layer/mask for the entities.
 */

#ifndef COLLISION_LAYER_H
#define COLLISION_LAYER_H

/**
 * @brief This describes the layers that the object appears in.
 */
struct CollisionLayer {
    struct Ground {};

    struct Meteor {};

    struct Player {};

    struct LeftWall {};

    struct RightWall {};
};

/**
 * @brief This describes what layers the body will scan for collisions.
 */
struct CollisionMask : CollisionLayer {};

#endif

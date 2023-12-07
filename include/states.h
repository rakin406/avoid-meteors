/**
 * @file
 * @brief Contains states for the game.
 */

#ifndef STATES_H
#define STATES_H

enum CollisionFlags
{
    Barrier = 1 << 0,
    Ground = 1 << 1,
    Meteor = 1 << 2,
    Player = 1 << 3
};

enum class Movement
{
    Idle,
    Running
};

enum class Direction
{
    Left,
    Right
};

#endif

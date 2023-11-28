#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "../deps/flecs_components_transform.h"
#include "../deps/flecs_components_physics.h"

using namespace flecs::components;

// Shortcuts to imported components
using Position = transform::Position2;
using Velocity = physics::Velocity2;

#endif
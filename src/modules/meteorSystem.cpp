#include "modules/meteorSystem.h"
#include "collisionLayer.h"
#include "components.h"
#include "tags.h"

#include <flecs.h>

#include <iostream>
#include <ranges>

modules::MeteorSystem::MeteorSystem(flecs::world& world)
{
    world.module<MeteorSystem>();

    // Register components
    world.component<CollisionLayer>();
    world.component<CollisionMask>();
    world.component<Level>();
    world.component<SpriteRenderer>();
    world.component<Sprite>();
    world.component<Transform>();
    world.component<Velocity>();

    // Setup meteors
    meteorsInit(world);
}

void modules::MeteorSystem::meteorsInit(flecs::world& world)
{
    const Level* level { world.get<Level>() };

    for (int i : std::views::iota(0, level->numMeteors))
    {
        auto meteor { world.entity() };
        meteor.add<Meteor>()
            .add<CollisionLayer::Meteor>()
            .add<CollisionMask::Ground>()
            .add<CollisionMask::Player>()
            .add<SpriteRenderer>()
            .add<Sprite>()
            // TODO: Set component values.
            .set<Transform>({})
            .set<Velocity>({});
    }
}

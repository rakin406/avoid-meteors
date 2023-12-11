#include "modules/meteorSystem.h"
#include "collisionLayer.h"
#include "components.h"
#include "renderWindow.h"
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

    // System that loads meteor sprite on startup
    world.system<RenderWindow, Sprite, Meteor>("LoadMeteorSprite")
        .kind(flecs::OnStart)
        .term_at(1)
        .singleton()
        .each([](RenderWindow& window, Sprite& sprite, Meteor)
              { sprite.texture = window.loadTexture(METEOR_SPRITE); });
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

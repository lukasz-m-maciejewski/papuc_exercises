#include "Collision.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <cassert>
#include <cmath>

#include "Entity.hpp"
#include "Manifold.hpp"

namespace physics
{
Collision::Collision(std::size_t triggers_count,
                     std::size_t static_colliders_count,
                     std::size_t dynamic_colliders_count)
{
    _triggers.reserve(triggers_count);
    _static_colliders.reserve(static_colliders_count);
    _dynamic_colliders.reserve(dynamic_colliders_count);
}

void Collision::update()
{
    collidersVsTriggers();
    collidersVsColliders();
}

void Collision::collidersVsTriggers()
{
    boost::for_each(_dynamic_colliders, [this](auto& collider) {
        boost::for_each(_triggers, [this, &collider](auto& trigger) {
            if (this->AABBvsAABB(collider.get_aabb(), trigger.get_aabb()))
            {
                trigger.addCollision(collider);
            }
        });
    });
}
Manifold getCollisionAlignment(float x_overlap, float y_overlap,
                               sf::Vector2f from_static_to_dynamic)
{
    Manifold m;
    if (x_overlap < y_overlap)
    {
        m.penetration = x_overlap;
        if (from_static_to_dynamic.x > 0)
        {
            m.normal = sf::Vector2f{1.0f, 0.0f};
        }
        else
        {
            m.normal = sf::Vector2f{-1.0f, 0.0f};
        }
    }
    else
    {
        m.penetration = y_overlap;
        if (from_static_to_dynamic.y > 0)
        {
            m.normal = sf::Vector2f{0.0f, 1.0f};
        }
        else
        {
            m.normal = sf::Vector2f{0.0f, -1.0f};
        }
    }
    return m;
}

void Collision::collidersVsColliders()
{
    // TODO(abergard):
    boost::for_each(_dynamic_colliders, [this](auto& dynamic_collider) {
        boost::for_each(_static_colliders, [this, &dynamic_collider](
                                               auto& static_collider) {

            AABB& dynamic_AABB = dynamic_collider.get_aabb();
            AABB& static_AABB = static_collider.get_aabb();

            auto half_dynamic_width = dynamic_AABB.width / 2;
            auto half_dynamic_height = dynamic_AABB.height / 2;
            auto dynamic_center =
                sf::Vector2f(dynamic_AABB.x + half_dynamic_width,
                             dynamic_AABB.y + half_dynamic_height);

            auto half_static_width = static_AABB.width / 2;
            auto half_static_height = static_AABB.height / 2;
            auto static_center =
                sf::Vector2f(static_AABB.x + half_static_width,
                             static_AABB.y + half_static_height);

            auto from_static_to_dynamic = dynamic_center - static_center;

            auto x_overlap = half_dynamic_width + half_static_width -
                             std::fabs(from_static_to_dynamic.x);
            if (x_overlap <= 0)
            {
                return;
            }

            auto y_overlap = half_dynamic_height + half_static_height -
                             std::fabs(from_static_to_dynamic.y);
            if (y_overlap <= 0)
            {
                return;
            }

            auto manifold = getCollisionAlignment(x_overlap, y_overlap,
                                                  from_static_to_dynamic);

            dynamic_collider.get_entity().move(manifold.normal *
                                               manifold.penetration);

            static_collider.addCollision(dynamic_collider);
        });
    });

    boost::for_each(_dynamic_colliders, [this](auto& collider) {
        boost::for_each(_dynamic_colliders, [this, &collider](auto& trigger) {
            if (AABBvsAABB(collider.get_aabb(), trigger.get_aabb()))
            {
                //
            }
        });
    });
}

Collider& Collision::addTrigger(sf::Vector2f position, float width,
                                float height)
{
    _triggers.push_back(createCollider(position, width, height));
    return _triggers.back();
}

Collider& Collision::addStaticCollider(sf::Vector2f position, float width,
                                       float height)
{
    _static_colliders.push_back(createCollider(position, width, height));
    return _static_colliders.back();
}

Collider& Collision::addDynamicCollider(sf::Vector2f position, float width,
                                        float height)
{
    _dynamic_colliders.push_back(createCollider(position, width, height));
    return _dynamic_colliders.back();
}

Collider Collision::createCollider(sf::Vector2f position, float width,
                                   float height)
{
    AABB aabb{};
    aabb.x = position.x;
    aabb.y = position.y;
    aabb.width = width;
    aabb.height = height;

    return Collider{aabb};
}

bool Collision::AABBvsAABB(const AABB& a, const AABB& b) const
{
    if (a.x > (b.x + b.width) || (a.x + a.width) < b.x)
    {
        return false;
    }

    if (a.y > (b.y + b.height) || (a.y + a.height) < b.y)
    {
        return false;
    }

    return true;
}
}
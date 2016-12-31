#include "Entity.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <boost/range/algorithm/for_each.hpp>

#include "Collider.hpp"

#include <iostream>

namespace bomberman
{
Entity::Entity(sf::RectangleShape& shape, physics::Collider& collider)
    : _shape{shape}, _collider{collider}
{
}

sf::RectangleShape& Entity::get_shape()
{
    return _shape;
}

physics::Collider& Entity::get_collider()
{
    return _collider;
}

void Entity::update()
{
    _collider.runScript();
}

void Entity::move(sf::Vector2f translation)
{
    _shape.move(translation);
    _collider.setPosition(_shape.getPosition());
}

void Entity::setPosition(sf::Vector2f position)
{
    _shape.setPosition(position);
    _collider.setPosition(position);
}

sf::Vector2f Entity::getPosition() const
{
    return _shape.getPosition();
}
}
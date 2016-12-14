#include "PhysicalEntity.hpp"
#include "PhysicalConstants.hpp"
#include "sfml_help.hpp"

using namespace sfml_help;

namespace lmg03
{
PhysicalEntity::PhysicalEntity(common::Log& log_init,
                               const sf::Vector2f& position_init,
                               const sf::Vector2f& velocity_init,
                               float mass_init,
                               const QuantumField& force_source_init)
    : log{log_init}, current_state{position_init, velocity_init, mass_init},
      force_source{force_source_init}
{
}

void PhysicalEntity::prepare_next_pos(float dt)
{
    next_state = compute_next_state(dt);
}

void PhysicalEntity::detect_colision(const sf::FloatRect& ent)
{
    sf::Vector2f size{20.f, 20.f};
    sf::FloatRect this_ent{next_state.position - 0.5f * size, size};
    if (not this_ent.intersects(ent))
    {
        current_state = next_state;
    }
    else
    {
    }
}

void PhysicalEntity::draw(sf::RenderTarget& target)
{
    sf::Vector2f size{20.f, 20.f};
    sf::RectangleShape box{size};
    box.setFillColor(sf::Color::Blue);
    box.setPosition(current_state.position - 0.5f * size);
    target.draw(box);
}

PointMass PhysicalEntity::compute_next_state(float dt)
{
    Derivative a, b, c, d;

    a = evaluate(0.0f, Derivative());
    b = evaluate(dt * 0.5f, a);
    c = evaluate(dt * 0.5f, b);
    d = evaluate(dt, c);

    sf::Vector2f dpos_dt =
        1.0f / 6.0f *
        (a.delta_pos + 2.0f * (b.delta_pos + c.delta_pos) + d.delta_pos);

    sf::Vector2f dv_dt =
        1.0f / 6.0f *
        (a.delta_vee + 2.0f * (b.delta_vee + c.delta_vee) + d.delta_vee);

    return {current_state.position + dpos_dt * dt,
            current_state.velocity + dv_dt * dt, current_state.mass};
}

Derivative PhysicalEntity::evaluate(float dt, const Derivative& d)
{
    return {current_state.velocity + d.delta_vee * dt,
            force_source.get_accel(current_state)};
}
}
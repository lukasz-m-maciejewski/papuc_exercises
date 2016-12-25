#include "Bomberman.hpp"

#include <algorithm>
#include <boost/range/algorithm/for_each.hpp>
#include <gsl/gsl_assert>
#include <random>

#include <cassert>
#include <iostream>

#include "GreenAllert.hpp"
#include "RedAllert.hpp"

namespace bomberman
{
Bomberman::Bomberman()
    : _window{sf::VideoMode(800, 600), "bomberman"}, _board{_window.getView()},
      _collision{_static_entity_count, _dynamic_entity_count}, _player{nullptr}
{
    _window.setVerticalSyncEnabled(true);

    _shapes.reserve(_static_entity_count + _dynamic_entity_count);
    generateRandomnlyArrangedStaticEntities(_static_entity_count);
    generateDynamicEntities(_dynamic_entity_count);
}

void Bomberman::run()
{
    sf::Clock frameClock;
    while (_window.isOpen())
    {
        sf::Time frameTick = frameClock.restart();

        updateEntities();
        updateInput();
        updateMovement(frameTick.asSeconds());
        updatePhysics();
        updateRender();
    }
}

void Bomberman::updateEntities()
{
    boost::for_each(_static_entities, [](auto& entity) { entity.update(); });
    boost::for_each(_dynamic_entities, [](auto& entity) { entity.update(); });
    // TODO(abergard): remove destroyed entities here
}

void Bomberman::updateInput()
{
    sf::Event event{};
    while (_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed ||
            (event.type == sf::Event::KeyPressed &&
             event.key.code == sf::Keyboard::Escape))
        {
            _window.close();
        }
    }
    updatePlayerInput();
}

void Bomberman::updatePlayerInput()
{
    _movementDirection = sf::Vector2f{0, 0};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        _movementDirection.y = -1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        _movementDirection.y = 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        _movementDirection.x = -1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        _movementDirection.x = 1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
        _bonusSpeed = 0.5f;
    }
    else
    {
        _bonusSpeed = 1.0f;
    }
}

void Bomberman::updateMovement(float deltaTime)
{
    movePlayer(_movementDirection * _baseSpeed * _bonusSpeed * deltaTime);
}

void Bomberman::movePlayer(const sf::Vector2f& transl)
{
    Expects(_player != nullptr);
    if (sf::FloatRect{{0.f, 0.f},
                      _window.getView().getSize() - sf::Vector2f{20.f, 20.f}}
            .contains(_player->getPosition() + transl))
    {
        _player->move(transl);
    }
}

void Bomberman::updateRender()
{
    clearDisplay();
    renderShapes();
    swapBuffer();
}

void Bomberman::updatePhysics()
{
    _collision.update();
}

void Bomberman::renderShapes()
{
    _board.drawOn(_window);
    boost::for_each(_shapes,
                    [this](auto const& entity) { _window.draw(entity); });
}

void Bomberman::clearDisplay()
{
    _window.clear(sf::Color::Black);
}

void Bomberman::swapBuffer()
{
    _window.display();
}

void Bomberman::generateDynamicEntities(const std::size_t& count)
{
    _dynamic_entities.reserve(count);

    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> x_distr(20.f,
                                                  _window.getSize().x - 20.f);
    std::uniform_real_distribution<float> y_distr(20.f,
                                                  _window.getSize().y - 20.f);
    std::uniform_real_distribution<float> radius_distr(10.f, 50.f);

    for (std::size_t i = 0; i < count - 1; ++i)
    {
        float w = radius_distr(gen);
        float h = radius_distr(gen);
        _shapes.emplace_back(sf::Vector2f{w, h});
        float x = x_distr(gen);
        float y = y_distr(gen);
        _shapes.back().setPosition(x, y);

        auto& collider = _collision.addStaticCollider(sf::Vector2f(x, y), w, h);

        _dynamic_entities.push_back(Entity{_shapes.back(), collider});

        collider.set_entity(_dynamic_entities.back());
        _shapes.back().setFillColor(sf::Color::Magenta);

        collider.attachScript(
            std::make_unique<GreenAllert>(_dynamic_entities.back()));
    }
    {
        float x = 0.0f;
        float y = 0.0f;
        float w = 20.0f;
        float h = 20.0f;
        _shapes.emplace_back(sf::Vector2f{w, h});
        auto& player = _shapes.back();
        player.setFillColor(sf::Color::Black);
        player.setPosition(x, y);

        auto& collider =
            _collision.addDynamicCollider(sf::Vector2f(x, y), w, h);
        _dynamic_entities.emplace_back(player, collider);
        collider.set_entity(_dynamic_entities.back());
        _player = &_dynamic_entities.back();
    }
}

void Bomberman::generateRandomnlyArrangedStaticEntities(
    const std::size_t& count)
{
    _static_entities.reserve(count);

    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> x_distr(
        20.f, static_cast<float>(_window.getSize().x) - 20.f);
    std::uniform_real_distribution<float> y_distr(
        20.f, static_cast<float>(_window.getSize().y) - 20.f);
    std::uniform_real_distribution<float> radius_distr(10.f, 50.f);

    for (std::size_t i = 0; i < count; ++i)
    {
        float w = radius_distr(gen);
        float h = radius_distr(gen);
        _shapes.emplace_back(sf::Vector2f{w, h});
        float x = x_distr(gen);
        float y = y_distr(gen);
        _shapes.back().setPosition(x, y);

        auto& trigger = _collision.addTrigger(sf::Vector2f(x, y), w, h);
        _static_entities.emplace_back(_shapes.back(), trigger);
        trigger.set_entity(_static_entities.back());
        trigger.attachScript(
            std::make_unique<RedAllert>(_static_entities.back()));
        _shapes.back().setFillColor(sf::Color::Blue);
    }
}
}

int main()
{
    bomberman::Bomberman bomberman;
    bomberman.run();
}
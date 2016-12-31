#pragma once

#include <SFML/Graphics.hpp>

#include "Board.hpp"
#include "Collision.hpp"
#include "Entity.hpp"

namespace bomberman
{
using Shapes = std::vector<sf::RectangleShape>;
using Entities = std::vector<Entity>;

class Bomberman
{
public:
    Bomberman();
    void run();

private:
    void updateEntities();
    void updateInput();
    void updatePlayerInput();
    void updatePlayerKeyboard();
    void updatePlayerXbox360Pad();
    void updateMovement(float deltaTime);
    void updateRender();
    void updatePhysics();

    void swapBuffer();
    void clearDisplay();

    void movePlayer(const sf::Vector2f& transl);
    void renderShapes();
    void generateRandomnlyArrangedStaticEntities(const std::size_t& count);
    void generateRandomnlyStaticTriggerEntities(const std::size_t& count);
    void generateRandomnlyStaticColliderEntities(const std::size_t& count);
    void generateDynamicEntities(const std::size_t& count);

    void generateMap(const std::size_t& count);
    void generateUnDestructibleWalls(int width, int height, int tile_size);

    sf::RenderWindow _window;
    lmg01::Board _board;
    const float _baseSpeed = 150.f;
    const std::size_t _static_entity_count = 200;
    const std::size_t _dynamic_entity_count = 1;
    float _bonusSpeed = 1.0f;
    sf::Vector2f _movementDirection{0, 0};
    std::vector<sf::RectangleShape> _shapes;
    Entities _static_entities;
    Entities _dynamic_entities;
    physics::Collision _collision;
    Entity* _player;
};
}

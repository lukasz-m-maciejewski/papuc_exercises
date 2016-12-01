#pragma once
#include <SFML/Graphics.hpp>

namespace lmg01
{
class Board
{
public:
    Board(const sf::View&);

    void drawOn(sf::RenderWindow&);
    void movePlayer(const sf::Vector2i& transl);

private:
    void drawEmptyBoard(sf::RenderWindow&);
    void drawPlayer(sf::RenderWindow& target);

    sf::View currentView;
    int N;
    int M;
    sf::Vector2i playerPos;
    bool needsRedraw{true};
};
}

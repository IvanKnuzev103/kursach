#ifndef HEXGAME_H
#define HEXGAME_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class hex {
public:
    float x, y;
    int q, r;
    std::string player;

    hex(int q, int r, float x_pos, float y_pos, const std::string& player_type);
    void draw_self(sf::RenderWindow& window);
    bool isClicked(sf::Vector2i mousePos, float radius);

private:
    sf::Color getColorForPlayer();
};

int distance(const hex& a, const hex& b);
std::vector<hex> generateHexGridHexShape(int radius, float startX, float startY);
void drawHexGrid(std::vector<hex>& hexGrid, sf::RenderWindow& window);
bool checkGameOver(const std::vector<hex>& hexGrid);
void printResults(const std::vector<hex>& hexGrid);
std::vector<std::pair<hex*, hex*>> getPossibleMoves(const std::vector<hex>& hexGrid, const std::string& aiPlayer);
int evaluateMove(const hex& source, const hex& target, const std::vector<hex>& hexGrid, const std::string& aiPlayer);
void convertNeighbors(std::vector<hex>& hexGrid, hex& target, const std::string& currentPlayer);
bool makeAIMove(std::vector<hex>& hexGrid, const std::string& aiPlayer);
void game_vs_player();
void game_vs_ii();

#endif
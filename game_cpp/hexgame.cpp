#include "hexgame.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <limits>

hex::hex(int q, int r, float x_pos, float y_pos, const std::string& player_type)
    : q(q), r(r), x(x_pos), y(y_pos), player(player_type) {}

void hex::draw_self(sf::RenderWindow& window) {
    const float radius = 60.f;
    sf::Vector2f center(x, y);

    sf::ConvexShape hexagon;
    hexagon.setPointCount(6);
    hexagon.setFillColor(getColorForPlayer());
    hexagon.setOutlineThickness(2.f);
    hexagon.setOutlineColor(sf::Color::Black);

    for (int i = 0; i < 6; ++i) {
        float angle = 2 * 3.14159265f * i / 6;
        hexagon.setPoint(i, sf::Vector2f(center.x + radius * cos(angle), center.y + radius * sin(angle)));
    }

    window.draw(hexagon);
}

bool hex::isClicked(sf::Vector2i mousePos, float radius) {
    float dx = mousePos.x - x;
    float dy = mousePos.y - y;
    return std::sqrt(dx * dx + dy * dy) <= radius;
}

sf::Color hex::getColorForPlayer() {
    if (player == "player1") {
        return sf::Color::Blue;
    } else if (player == "player2") {
        return sf::Color::Red;
    } else {
        return sf::Color::White;
    }
}

int distance(const hex& a, const hex& b) {
    return (abs(a.q - b.q) + abs(a.q + a.r - b.q - b.r) + abs(a.r - b.r)) / 2;
}

std::vector<hex> generateHexGridHexShape(int radius, float startX, float startY) {
    std::vector<hex> hexGrid;
    const float hexRadius = 60.f;
    const float hexWidth = 2 * hexRadius * cos(3.14159265f / 6);
    const float hexHeight = 2 * hexRadius;

    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();

    for (int q = -radius; q <= radius; ++q) {
        int r1 = std::max(-radius, -q - radius);
        int r2 = std::min(radius, -q + radius);
        for (int r = r1; r <= r2; ++r) {
            float y = startY + (r + radius + q / 2.0f) * hexHeight * 0.866f;
            if (y < minY) minY = y;
            if (y > maxY) maxY = y;
        }
    }
// x = q
// z = r
// y = -x - z = -q - r
    for (int q = -radius; q <= radius; ++q) {
        int r1 = std::max(-radius, -q - radius);
        int r2 = std::min(radius, -q + radius);
        for (int r = r1; r <= r2; ++r) {
            float x = startX + (q + radius) * hexWidth * 0.75f;
            float y = startY + (r + radius + q / 2.0f) * hexHeight * 0.866f;
            
            std::string player_type = "neutral";
            if (std::abs(y - minY) < 1.0f) {
                player_type = "player1";
            }
            else if (std::abs(y - maxY) < 1.0f) {
                player_type = "player2";
            }
            
            hexGrid.push_back(hex(q, r, x, y, player_type));
        }
    }

    return hexGrid;
}

void drawHexGrid(std::vector<hex>& hexGrid, sf::RenderWindow& window) {
    for (auto& hexagon : hexGrid) {
        hexagon.draw_self(window);
    }
}

bool checkGameOver(const std::vector<hex>& hexGrid) {
    int neutralCount = 0;
    int player1Count = 0;
    int player2Count = 0;
    
    for (const auto& h : hexGrid) {
        if (h.player == "neutral") neutralCount++;
        else if (h.player == "player1") player1Count++;
        else if (h.player == "player2") player2Count++;
    }
    
    return neutralCount == 0 || player1Count == 0 || player2Count == 0;
}

void printResults(const std::vector<hex>& hexGrid) {
    int player1Count = 0;
    int player2Count = 0;
    
    for (const auto& h : hexGrid) {
        if (h.player == "player1") player1Count++;
        else if (h.player == "player2") player2Count++;
    }
    
    std::cout << "Player1: " << player1Count << std::endl;
    std::cout << "Player2: " << player2Count << std::endl;
    
    if (player1Count > player2Count) {
        std::cout << "Player1 wins!" << std::endl;
    } else if (player2Count > player1Count) {
        std::cout << "Player2 wins!" << std::endl;
    } else {
        std::cout << "Draw!" << std::endl;
    }
}

std::vector<std::pair<hex*, hex*>> getPossibleMoves(const std::vector<hex>& hexGrid, const std::string& aiPlayer) {
    std::vector<std::pair<hex*, hex*>> moves;
    
    std::vector<hex*> aiHexes;
    for (auto& h : const_cast<std::vector<hex>&>(hexGrid)) {
        if (h.player == aiPlayer) {
            aiHexes.push_back(&h);
        }
    }
    
    for (auto* source : aiHexes) {
        for (auto& target : const_cast<std::vector<hex>&>(hexGrid)) {
            if (target.player == "neutral") {
                int dist = distance(*source, target);
                if (dist == 1 || dist == 2) {
                    moves.push_back({source, &target});
                }
            }
        }
    }
    
    return moves;
}

int evaluateMove(const hex& source, const hex& target, const std::vector<hex>& hexGrid, const std::string& aiPlayer) {
    int score = 0;
    
    score += 10;
    
    std::string opponent = (aiPlayer == "player1") ? "player2" : "player1";
    for (const auto& neighbor : hexGrid) {
        if (distance(target, neighbor) == 1 && neighbor.player == opponent) {
            score += 15;
        }
    }
    
    int distFromCenter = abs(target.q) + abs(target.r);
    score += (5 - distFromCenter);
    
    int sourceNeighbors = 0;
    for (const auto& neighbor : hexGrid) {
        if (distance(source, neighbor) == 1 && neighbor.player == aiPlayer) {
            sourceNeighbors++;
        }
    }
    if (sourceNeighbors <= 1) {
        score -= 5;
    }
    
    return score;
}

void convertNeighbors(std::vector<hex>& hexGrid, hex& target, const std::string& currentPlayer) {
    std::string opponent = (currentPlayer == "player1") ? "player2" : "player1";
    
    for (auto& neighbor : hexGrid) {
        if (distance(target, neighbor) == 1 && neighbor.player == opponent) {
            neighbor.player = currentPlayer;
        }
    }
}

bool makeAIMove(std::vector<hex>& hexGrid, const std::string& aiPlayer) {
    auto possibleMoves = getPossibleMoves(hexGrid, aiPlayer);
    
    if (possibleMoves.empty()) {
        return false;
    }
    
    int bestScore = -1000;
    std::pair<hex*, hex*> bestMove;
    
    for (const auto& move : possibleMoves) {
        int score = evaluateMove(*move.first, *move.second, hexGrid, aiPlayer);
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }
    
    hex* source = bestMove.first;
    hex* target = bestMove.second;
    int dist = distance(*source, *target);
    
    if (dist == 1) {
        target->player = aiPlayer;
        convertNeighbors(hexGrid, *target, aiPlayer);
    } else if (dist == 2) {
        for (auto& mid : hexGrid) {
            if (distance(*source, mid) == 1 && distance(mid, *target) == 1) {
                source->player = "neutral";
                target->player = aiPlayer;
                convertNeighbors(hexGrid, *target, aiPlayer);
                break;
            }
        }
    }
    
    return true;
}
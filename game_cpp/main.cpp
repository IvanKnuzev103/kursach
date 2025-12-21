#include "hexgame.h"
#include <iostream>

void game_vs_player() {
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktopMode, "Hexagon", sf::Style::Fullscreen);
    std::vector<hex> hexGrid = generateHexGridHexShape(5, 550.f, 100.f);
    int count = 0;
    const float radius = 60.f;
    hex* selectedHex = nullptr;

    while (window.isOpen()) {
        if (checkGameOver(hexGrid)) {
            printResults(hexGrid);
            window.close();
            return;
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.scancode == sf::Keyboard::Scan::Escape))
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                for (auto& h : hexGrid) {
                    if (h.isClicked(mousePos, radius)) {
                        std::string currentPlayer = (count % 2 == 0) ? "player1" : "player2";
                        
                        if (selectedHex == nullptr) {
                            if (h.player == currentPlayer) {
                                selectedHex = &h;
                            }
                        } else {
                            if (h.player == "neutral") {
                                int dist = distance(*selectedHex, h);
                                
                                if (dist == 1) {
                                    h.player = currentPlayer;
                                    convertNeighbors(hexGrid, h, currentPlayer);
                                    selectedHex = nullptr;
                                    count++;
                                }
                                else if (dist == 2) {
                                    for (auto& mid : hexGrid) {
                                        if (distance(*selectedHex, mid) == 1 && distance(mid, h) == 1) {
                                            selectedHex->player = "neutral";
                                            h.player = currentPlayer;
                                            convertNeighbors(hexGrid, h, currentPlayer);
                                            selectedHex = nullptr;
                                            count++;
                                            break;
                                        }
                                    }
                                } else {
                                    selectedHex = nullptr;
                                }
                            } else {
                                selectedHex = nullptr;
                            }
                        }
                        break;
                    }
                }
            }
        }

        window.clear(sf::Color::White);
        drawHexGrid(hexGrid, window);
        
        if (selectedHex != nullptr) {
            sf::CircleShape highlight(65.f);
            highlight.setFillColor(sf::Color::Transparent);
            highlight.setOutlineColor(sf::Color::Yellow);
            highlight.setOutlineThickness(5.f);
            highlight.setPosition(selectedHex->x - 65.f, selectedHex->y - 65.f);
            window.draw(highlight);
        }
        
        window.display();
    }
}

void game_vs_ii() {
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktopMode, "Hexagon", sf::Style::Fullscreen);
    std::vector<hex> hexGrid = generateHexGridHexShape(5, 550.f, 100.f);
    int count = 0;
    const float radius = 60.f;
    hex* selectedHex = nullptr;
    bool playerTurn = true;

    while (window.isOpen()) {
        if (checkGameOver(hexGrid)) {
            printResults(hexGrid);
            window.close();
            return;
        }

        if (!playerTurn && !checkGameOver(hexGrid)) {
            makeAIMove(hexGrid, "player2");
            playerTurn = true;
            count++;
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || 
                (event.type == sf::Event::KeyPressed && event.key.scancode == sf::Keyboard::Scan::Escape))
                window.close();

            if (playerTurn && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                for (auto& h : hexGrid) {
                    if (h.isClicked(mousePos, radius)) {
                        if (selectedHex == nullptr) {
                            if (h.player == "player1") {
                                selectedHex = &h;
                            }
                        } else {
                            if (h.player == "neutral") {
                                int dist = distance(*selectedHex, h);
                                
                                if (dist == 1) {
                                    h.player = "player1";
                                    convertNeighbors(hexGrid, h, "player1");
                                    selectedHex = nullptr;
                                    playerTurn = false;
                                    count++;
                                }
                                else if (dist == 2) {
                                    for (auto& mid : hexGrid) {
                                        if (distance(*selectedHex, mid) == 1 && distance(mid, h) == 1) {
                                            selectedHex->player = "neutral";
                                            h.player = "player1";
                                            convertNeighbors(hexGrid, h, "player1");
                                            selectedHex = nullptr;
                                            playerTurn = false;
                                            count++;
                                            break;
                                        }
                                    }
                                } else {
                                    selectedHex = nullptr;
                                }
                            } else {
                                selectedHex = nullptr;
                            }
                        }
                        break;
                    }
                }
            }
        }

        window.clear(sf::Color::White);
        drawHexGrid(hexGrid, window);
        
        if (selectedHex != nullptr) {
            sf::CircleShape highlight(65.f);
            highlight.setFillColor(sf::Color::Transparent);
            highlight.setOutlineColor(sf::Color::Yellow);
            highlight.setOutlineThickness(5.f);
            highlight.setPosition(selectedHex->x - 65.f, selectedHex->y - 65.f);
            window.draw(highlight);
        }
        
        window.display();
    }
}

int main() {
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktopMode, "SFML", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    sf::RectangleShape button1(sf::Vector2f(500, 200));
    button1.setFillColor(sf::Color(100, 149, 237));
    button1.setPosition(700, 150);

    sf::RectangleShape button2(sf::Vector2f(500, 200));
    button2.setFillColor(sf::Color(100, 149, 237));
    button2.setPosition(700, 450);

    sf::RectangleShape button3(sf::Vector2f(500, 200));
    button3.setFillColor(sf::Color(100, 149, 237));
    button3.setPosition(700, 750);

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        return 1;
    }

    sf::Text text1, text2, text3;
    text1.setFont(font);
    text1.setString("Game vs Player");
    text1.setCharacterSize(24);
    text1.setFillColor(sf::Color::White);

    text2.setFont(font);
    text2.setString("Game vs ii");
    text2.setCharacterSize(24);
    text2.setFillColor(sf::Color::White);

    text3.setFont(font);
    text3.setString("Exit");
    text3.setCharacterSize(24);
    text3.setFillColor(sf::Color::White);

    sf::FloatRect textRect1 = text1.getLocalBounds();
    text1.setOrigin(textRect1.left + textRect1.width / 2.0f, 
                    textRect1.top + textRect1.height / 2.0f);
    text1.setPosition(button1.getPosition().x + button1.getSize().x / 2.0f, button1.getPosition().y + button1.getSize().y / 2.0f);

    sf::FloatRect textRect2 = text2.getLocalBounds();
    text2.setOrigin(textRect2.left + textRect2.width / 2.0f, textRect2.top + textRect2.height / 2.0f);
    text2.setPosition( button2.getPosition().x + button2.getSize().x / 2.0f, button2.getPosition().y + button2.getSize().y / 2.0f);

    sf::FloatRect textRect3 = text3.getLocalBounds();
    text3.setOrigin(textRect3.left + textRect3.width / 2.0f, textRect3.top + textRect3.height / 2.0f);
    text3.setPosition(button3.getPosition().x + button3.getSize().x / 2.0f,button3.getPosition().y + button3.getSize().y / 2.0f);

    bool button1Clicked = false;
    bool button2Clicked = false;
    bool button3Clicked = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed && event.key.scancode == sf::Keyboard::Scan::Escape)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (button1.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y))
                {
                    button1Clicked = true;
                    button1.setFillColor(sf::Color::Green);
                    window.close();
                    game_vs_player();
                }

                if (button2.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y))
                {
                    button2Clicked = true;
                    button2.setFillColor(sf::Color::Green);
                    window.close();
                    game_vs_ii();
                }
                if (button3.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y))
                {
                    button3Clicked = true;
                    button3.setFillColor(sf::Color::Green);
                    window.close();
                }
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                button1Clicked = false;
                button2Clicked = false;
                button3Clicked = false;
            }
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (button1.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y))
        {
            if (!button1Clicked) button1.setFillColor(sf::Color(70, 130, 180));
        }
        else
        {
            if (!button1Clicked) button1.setFillColor(sf::Color(100, 149, 237));
        }

        if (button2.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y))
        {
            if (!button2Clicked) button2.setFillColor(sf::Color(70, 130, 180));
        }
        else
        {
            if (!button2Clicked) button2.setFillColor(sf::Color(100, 149, 237));
        }

        if (button3.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y))
        {
            if (!button3Clicked) button3.setFillColor(sf::Color(70, 130, 180));
        }
        else
        {
            if (!button3Clicked) button3.setFillColor(sf::Color(100, 149, 237));
        }

        window.clear(sf::Color::White);
        window.draw(button1);
        window.draw(text1);
        window.draw(button2);
        window.draw(text2);
        window.draw(button3);
        window.draw(text3);

        window.display();
    }

    return 0;
}   
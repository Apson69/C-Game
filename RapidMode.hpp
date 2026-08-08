#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class RapidMode {
public:
    enum class Phase { Countdown, Active, Finished };
    Phase phase = Phase::Countdown;

    // Delta-Time Countdown (4.0s -> 0.0s)
    float countdownTimer = 4.0f;
    int currentCount = 4;
    bool isGo = false;

    std::string resultText = "READY...";
    sf::Color textColor = sf::Color::Yellow;

    sf::Sprite whiteSheep;
    sf::Sprite blackSheep;

    float whitePosX = 180.f;
    float blackPosX = 620.f;
    int winner = 0; // 0 = none, 1 = White, 2 = Black

    RapidMode(const sf::Texture& whiteTex, const sf::Texture& blackTex)
        : whiteSheep(whiteTex), blackSheep(blackTex)
    {
        reset();
    }

    void reset() {
        phase = Phase::Countdown;
        countdownTimer = 4.0f;
        currentCount = 4;
        isGo = false;
        resultText = "READY...";
        textColor = sf::Color::Yellow;
        winner = 0;

        // Reset Sheep Positions & Scales
        whiteSheep.setScale({ 0.12f, 0.12f });
        blackSheep.setScale({ -0.12f, 0.12f }); // Flip facing left

        whitePosX = 180.f;
        blackPosX = 620.f;

        sf::FloatRect wBounds = whiteSheep.getLocalBounds();
        sf::FloatRect bBounds = blackSheep.getLocalBounds();
        whiteSheep.setOrigin({ wBounds.position.x + wBounds.size.x / 2.f, wBounds.position.y + wBounds.size.y / 2.f });
        blackSheep.setOrigin({ bBounds.position.x + bBounds.size.x / 2.f, bBounds.position.y + bBounds.size.y / 2.f });

        whiteSheep.setPosition({ whitePosX, 280.f });
        blackSheep.setPosition({ blackPosX, 280.f });
    }

    void handleKeyPress(sf::Keyboard::Key key) {
        if (phase == Phase::Finished) {
            if (key == sf::Keyboard::Key::R) {
                reset();
            }
            return;
        }

        // Player 1 (White) = A, Player 2 (Black) = L
        if (key == sf::Keyboard::Key::A) {
            if (!isGo && phase == Phase::Countdown) {
                // Player 1 Pressed Early -> FOUL -> Player 2 Wins!
                winner = 2;
                resultText = "Player 1 FOUL!\nPlayer 2 (Black) Wins!";
                textColor = sf::Color::Red;
                phase = Phase::Finished;
            } else if (isGo && phase == Phase::Active) {
                // Valid Press -> Player 1 Wins!
                winner = 1;
                resultText = "PLAYER 1 (WHITE) WINS!";
                textColor = sf::Color::Green;
                phase = Phase::Finished;
            }
        }
        else if (key == sf::Keyboard::Key::L) {
            if (!isGo && phase == Phase::Countdown) {
                // Player 2 Pressed Early -> FOUL -> Player 1 Wins!
                winner = 1;
                resultText = "Player 2 FOUL!\nPlayer 1 (White) Wins!";
                textColor = sf::Color::Red;
                phase = Phase::Finished;
            } else if (isGo && phase == Phase::Active) {
                // Valid Press -> Player 2 Wins!
                winner = 2;
                resultText = "PLAYER 2 (BLACK) WINS!";
                textColor = sf::Color::Green;
                phase = Phase::Finished;
            }
        }
    }

    void update(float dt) {
        // 1. Countdown Phase using dt
        if (phase == Phase::Countdown) {
            countdownTimer -= dt;

            if (countdownTimer > 3.0f) {
                currentCount = 4;
            } else if (countdownTimer > 2.0f) {
                currentCount = 3;
            } else if (countdownTimer > 1.0f) {
                currentCount = 2;
            } else if (countdownTimer > 0.0f) {
                currentCount = 1;
            } else {
                // Countdown reached 0 -> GO!
                isGo = true;
                phase = Phase::Active;
                resultText = "GO!";
                textColor = sf::Color::Magenta;
            }
        }

        // 2. Bumping animation when someone wins
        if (phase == Phase::Finished) {
            if (winner == 1) { // White pushes Black to the right
                if (whitePosX < blackPosX - 60.f) {
                    whitePosX += 450.f * dt;
                } else {
                    whitePosX += 600.f * dt;
                    blackPosX += 600.f * dt;
                }
            } else if (winner == 2) { // Black pushes White to the left
                if (blackPosX > whitePosX + 60.f) {
                    blackPosX -= 450.f * dt;
                } else {
                    whitePosX -= 600.f * dt;
                    blackPosX -= 600.f * dt;
                }
            }
            whiteSheep.setPosition({ whitePosX, 280.f });
            blackSheep.setPosition({ blackPosX, 280.f });
        }
    }

    void draw(sf::RenderWindow& window, const sf::Font& font) {
        // Draw both sheep
        window.draw(whiteSheep);
        window.draw(blackSheep);

        // Display Text
        sf::Text statusText(font, "", 45);
        statusText.setFillColor(textColor);

        if (phase == Phase::Countdown) {
            statusText.setString(std::to_string(currentCount));
            statusText.setCharacterSize(75);
        } else {
            statusText.setString(resultText);
        }

        // Center Text accurately in SFML 3
        sf::FloatRect bounds = statusText.getLocalBounds();
        statusText.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });
        statusText.setPosition({ 400.f, 100.f });
        window.draw(statusText);

        // Subtext Controls Info
        sf::Text subText(font, "Player 1: [ A ]   |   Player 2: [ L ]", 22);
        subText.setFillColor(sf::Color::White);
        sf::FloatRect subBounds = subText.getLocalBounds();
        subText.setOrigin({ subBounds.position.x + subBounds.size.x / 2.f, subBounds.position.y + subBounds.size.y / 2.f });
        subText.setPosition({ 400.f, 480.f });
        window.draw(subText);

        if (phase == Phase::Finished) {
            sf::Text restartText(font, "Press R to Replay  |  Backspace for Menu", 20);
            restartText.setFillColor(sf::Color::Yellow);
            sf::FloatRect rBounds = restartText.getLocalBounds();
            restartText.setOrigin({ rBounds.position.x + rBounds.size.x / 2.f, rBounds.position.y + rBounds.size.y / 2.f });
            restartText.setPosition({ 400.f, 440.f });
            window.draw(restartText);
        }
    }
};
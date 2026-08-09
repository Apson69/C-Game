#pragma once
#include <vector>
#include <deque>
#include <random>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "Sheep.hpp"

class LaneManager {
public:
    std::vector<Sheep> lanes[5];
    float laneYPositions[5] = { 110.f, 190.f, 270.f, 350.f, 430.f };

    // Player 1 & Player 2 Lanes
    int p1SelectedLane = 2; // Middle lane default
    int p2SelectedLane = 2;

    // Upcoming Sheep Queues (3 sheep each)
    std::deque<SheepSize> p1Queue;
    std::deque<SheepSize> p2Queue;

    // Cooldown Timers (2.3 Seconds)
    const float maxCooldown = 2.3f;
    float p1Cooldown = 0.0f;
    float p2Cooldown = 0.0f;

    std::mt19937 rng;

    LaneManager() : rng(std::random_device{}()) {
        resetQueues();
    }

    SheepSize getRandomSize() {
        std::uniform_int_distribution<int> dist(0, 3);
        int r = dist(rng);
        if (r == 0) return SheepSize::Small;
        if (r == 1) return SheepSize::Medium;
        if (r == 2) return SheepSize::Large;
        return SheepSize::Giant;
    }

    void resetQueues() {
        p1Queue.clear();
        p2Queue.clear();
        p1Cooldown = 0.0f;
        p2Cooldown = 0.0f;

        for (int i = 0; i < 3; ++i) {
            p1Queue.push_back(getRandomSize());
            p2Queue.push_back(getRandomSize());
        }
    }

    // Player 1 (White) Launch
    bool tryLaunchP1(const sf::Texture& whiteTex) {
        if (p1Cooldown > 0.0f) return false; // Still on cooldown

        SheepSize nextSize = p1Queue.front();
        p1Queue.pop_front();
        p1Queue.push_back(getRandomSize());

        float startX = -40.f;
        lanes[p1SelectedLane].emplace_back(Team::White, nextSize, p1SelectedLane, startX, laneYPositions[p1SelectedLane], whiteTex);

        p1Cooldown = maxCooldown; // Start 2.3s cooldown
        return true;
    }

    // Player 2 (Black) Launch
    bool tryLaunchP2(const sf::Texture& blackTex) {
        if (p2Cooldown > 0.0f) return false; // Still on cooldown

        SheepSize nextSize = p2Queue.front();
        p2Queue.pop_front();
        p2Queue.push_back(getRandomSize());

        float startX = 840.f;
        lanes[p2SelectedLane].emplace_back(Team::Black, nextSize, p2SelectedLane, startX, laneYPositions[p2SelectedLane], blackTex);

        p2Cooldown = maxCooldown; // Start 2.3s cooldown
        return true;
    }

    void update(float dt) {
        // Update Cooldown Timers
        if (p1Cooldown > 0.0f) p1Cooldown = std::max(0.0f, p1Cooldown - dt);
        if (p2Cooldown > 0.0f) p2Cooldown = std::max(0.0f, p2Cooldown - dt);

        // Update Physics & Collisions
        for (int i = 0; i < 5; ++i) {
            auto& sheepList = lanes[i];

            int whiteWeight = 0;
            int blackWeight = 0;
            bool collisionActive = false;

            for (const auto& s : sheepList) {
                if (s.team == Team::White) whiteWeight += s.weight;
                else blackWeight += s.weight;
            }

            for (size_t a = 0; a < sheepList.size(); ++a) {
                for (size_t b = a + 1; b < sheepList.size(); ++b) {
                    if (sheepList[a].team != sheepList[b].team) {
                        sf::FloatRect boundsA = sheepList[a].sprite.getGlobalBounds();
                        sf::FloatRect boundsB = sheepList[b].sprite.getGlobalBounds();
                        if (boundsA.findIntersection(boundsB).has_value()) {
                            collisionActive = true;
                            break;
                        }
                    }
                }
            }

            int netWeight = whiteWeight - blackWeight;
            float groupPushSpeed = 0.f;

            if (netWeight > 0)      groupPushSpeed = 40.f + (netWeight * 15.f);
            else if (netWeight < 0) groupPushSpeed = -40.f + (netWeight * 15.f);
            else                    groupPushSpeed = 0.f;

            for (auto& s : sheepList) {
                s.isColliding = collisionActive;
                s.update(dt, groupPushSpeed);
            }

            sheepList.erase(
                std::remove_if(sheepList.begin(), sheepList.end(), [](const Sheep& s) {
                    return (s.team == Team::White && s.position.x > 840.f) ||
                           (s.team == Team::Black && s.position.x < -60.f);
                }),
                sheepList.end()
            );
        }
    }

    void drawHUD(sf::RenderWindow& window, const sf::Texture& whiteTex, const sf::Texture& blackTex) {
        // --- 1. Draw Player 1 (White) Queue (Top Left) ---
        drawPlayerQueue(window, whiteTex, p1Queue, p1Cooldown, 35.f, 30.f, Team::White);

        // --- 2. Draw Player 2 (Black) Queue (Top Right) ---
        drawPlayerQueue(window, blackTex, p2Queue, p2Cooldown, 680.f, 30.f, Team::Black);

        // --- 3. Draw Lane Selector Indicators ---
        // Player 1 Lane Pointer
        sf::Texture textureGo;
        textureGo.loadFromFile(GoButton);
        sf::Sprite spriteGo(textureGo);

        spriteGo.setScale({0.15,0.15});
        spriteGo.setPosition({ 15.f, laneYPositions[p1SelectedLane] - 8.f });
        window.draw(spriteGo);

        // Player 2 Lane Pointer
        spriteGo.setScale({0.15,0.15});
        spriteGo.setPosition({ 765.f, laneYPositions[p2SelectedLane] - 8.f });
        window.draw(spriteGo);
    }

private:
    void drawPlayerQueue(sf::RenderWindow& window, const sf::Texture& tex, const std::deque<SheepSize>& queue, float cooldown, float startX, float startY, Team team) {
        for (int i = 0; i < 3; ++i) {
            float posX = startX + (i * 62.f);
            float posY = startY;

            sf::Sprite previewSprite(tex);
            float scale = 0.04f + (static_cast<int>(queue[i]) * 0.008f);

            if (team == Team::Black) {

                previewSprite.setScale({ -scale, scale });
            } else {

                previewSprite.setScale({ scale, scale });
            }

            sf::FloatRect bounds = previewSprite.getLocalBounds();
            previewSprite.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });
            previewSprite.setPosition({ posX, posY+10 });

            // Draw Cooldown Ring around Current (First) Sheep
            if (i == 0) {
                float ringRadius = 28.f;
                sf::CircleShape bgRing(ringRadius);
                bgRing.setOrigin({ ringRadius, ringRadius });
                bgRing.setPosition({ posX, posY+10 });
                bgRing.setFillColor(sf::Color(0, 0, 0, 150));

                if (cooldown > 0.0f) {
                    // Charging Cooldown (Red/Orange Outline)
                    bgRing.setOutlineThickness(4.f);
                    bgRing.setOutlineColor(sf::Color(255, 100, 0));
                } else {
                    // Ready to Launch (Green Outline)
                    bgRing.setOutlineThickness(4.f);
                    bgRing.setOutlineColor(sf::Color::Green);
                }
                window.draw(bgRing);
            }

            window.draw(previewSprite);
        }
    }

public:
    void draw(sf::RenderWindow& window) {
        for (int i = 0; i < 5; ++i) {
            for (auto& s : lanes[i]) {
                s.draw(window);
            }
        }
    }
};
#pragma once
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "Sheep.hpp"

class LaneManager {
public:
    std::vector<Sheep> lanes[5]; // 5 horizontal lanes
    // Y position offset for each lane matching grass rows
    float laneYPositions[5] = { 110.f, 190.f, 270.f, 350.f, 430.f };

    void spawnSheep(Team team, SheepSize size, int lane, const sf::Texture& texture) {
        if (lane < 0 || lane >= 5) return;
        float startX = (team == Team::White) ? -40.f : 840.f;
        lanes[lane].emplace_back(team, size, lane, startX, laneYPositions[lane], texture);
    }

    void update(float dt) {
        for (int i = 0; i < 5; ++i) {
            auto& sheepList = lanes[i];

            int whiteWeight = 0;
            int blackWeight = 0;
            bool collisionActive = false;

            for (const auto& s : sheepList) {
                if (s.team == Team::White) whiteWeight += s.weight;
                else blackWeight += s.weight;
            }

            // SFML 3 Collision Detection
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

            // Calculate Group Push Speed based on Net Weight
            int netWeight = whiteWeight - blackWeight;
            float groupPushSpeed = 0.f;

            if (netWeight > 0)      groupPushSpeed = 40.f + (netWeight * 15.f);  // White pushes Right
            else if (netWeight < 0) groupPushSpeed = -40.f + (netWeight * 15.f); // Black pushes Left
            else                    groupPushSpeed = 0.f;                         // Equal weight: Stalemate

            // Update all sheep in this lane
            for (auto& s : sheepList) {
                s.isColliding = collisionActive;
                s.update(dt, groupPushSpeed);
            }

            // Erase sheep that reach the end of the field
            sheepList.erase(
                std::remove_if(sheepList.begin(), sheepList.end(), [](const Sheep& s) {
                    return (s.team == Team::White && s.position.x > 840.f) ||
                           (s.team == Team::Black && s.position.x < -60.f);
                }),
                sheepList.end()
            );
        }
    }

    void draw(sf::RenderWindow& window) {
        for (int i = 0; i < 5; ++i) {
            for (auto& s : lanes[i]) {
                s.draw(window);
            }
        }
    }
};
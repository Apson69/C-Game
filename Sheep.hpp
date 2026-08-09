#pragma once
#include <SFML/Graphics.hpp>

enum class Team { White, Black };
enum class SheepSize { Small = 1, Medium = 2, Large = 4, Giant = 8 };

class Sheep {
public:
    Team team;
    SheepSize size;
    int weight;
    float speed;
    sf::Vector2f position;
    int laneIndex;
    bool isColliding = false;

    sf::Sprite sprite;

    Sheep(Team t, SheepSize s, int lane, float startX, float startY, const sf::Texture& texture)
        : team(t), size(s), weight(static_cast<int>(s)), laneIndex(lane), sprite(texture)
    {
        // 1. Base speed by size
        float baseSpeed = 100.f;
        switch (s) {
            case SheepSize::Small:  baseSpeed = 120.f; break;
            case SheepSize::Medium: baseSpeed = 115.f; break;
            case SheepSize::Large:  baseSpeed = 110.f; break;
            case SheepSize::Giant:  baseSpeed = 105.f; break;
        }
        speed = (team == Team::White) ? baseSpeed : -baseSpeed;

        // 2. Visual scale by size
        float scaleFactor = 0.1f;
        switch (s) {
            case SheepSize::Small:  scaleFactor = 0.07f; break;
            case SheepSize::Medium: scaleFactor = 0.09f; break;
            case SheepSize::Large:  scaleFactor = 0.10f; break;
            case SheepSize::Giant:  scaleFactor = 0.12f; break;
        }

        // Apply scale (flip horizontally for Black team)
        if (team == Team::Black) {
            sprite.setScale({ -scaleFactor, scaleFactor });
        } else {
            sprite.setScale({ scaleFactor, scaleFactor });
        }

        // Center origin so sprite flips and scales smoothly
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });

        position = { startX, startY };
        sprite.setPosition(position);
    }

    void update(float dt, float pushSpeed = 0.f) {
        float moveX = isColliding ? pushSpeed : speed;
        position.x += moveX * dt;
        sprite.setPosition(position);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
};
#include <iostream>
#include <SFML/Graphics.hpp>
#include "DEFINITIONS.hpp"
#include "LaneManager.hpp"
#include "Sheep.hpp"
#include "RapidMode.hpp"

// Tile Pattern Creation
std::vector<sf::Sprite> patternmaker(const sf::Texture& light, const sf::Texture& dark) {
    std::vector<sf::Sprite> tiles;
    float posX = -105.f, posY = 80.f;
    int pattern = 0;

    for (int i = 0; i < 5; i++) {
        posX = -105.f;
        for (int j = 0; j < 10; j++) {
            sf::Sprite sprite(pattern % 2 == 0 ? light : dark);
            sprite.setScale({0.43f, 0.43f});
            sprite.setPosition({posX, posY});
            tiles.push_back(sprite);
            pattern++;
            posX += 105.f;
        }
        posY += 80.f;
        pattern++;
    }
    return tiles;
}

int main()
{
    // const int FPS=60;
    // const int WindowLength=800;
    // const int WindowBreadth=550;
    // const char FontPath[]="C:/Users/apson/Documents/CPP/font/Virgil.ttf";

    int ModeNumber = 0;

    // Import Font
    sf::Font font;
    if (!font.openFromFile(FontPath))
    {
        std::cout << "Failed to load font: " << FontPath << std::endl;
        return -1;
    }

    // Window
    sf::RenderWindow window(
        sf::VideoMode({WindowLength, WindowBreadth}), "Bheda"
    );

    window.setFramerateLimit(FPS);

    // Title Text (Bheda)
    sf::Text TitleText(font, TITLE, 50);
    TitleText.setFillColor(sf::Color::White);
    TitleText.setPosition({320.f, 35.f});

    // Bottom Mode Text
    sf::Text TitleTextMode(font, "", 25);
    TitleTextMode.setFillColor(sf::Color::White);
    TitleTextMode.setPosition({190.f, 500.f});

    // Mode Menu Selector (>)
    sf::Text TitleModeMenuSelector(font, "", 35);
    TitleModeMenuSelector.setFillColor(sf::Color::White);

    // Mode Menu List
    sf::Text TitleModeMenu1(font, "", 35);
    TitleModeMenu1.setFillColor(sf::Color::White);
    TitleModeMenu1.setString("Classic Mode");
    TitleModeMenu1.setPosition({230.f, 150.f});

    sf::Text TitleModeMenu2(font, "", 35);
    TitleModeMenu2.setFillColor(sf::Color::White);
    TitleModeMenu2.setString("Rapid Mode");
    TitleModeMenu2.setPosition({230.f, 250.f});

    sf::Text TitleModeMenu3(font, "", 35);
    TitleModeMenu3.setFillColor(sf::Color::White);
    TitleModeMenu3.setString("Exit");
    TitleModeMenu3.setPosition({230.f, 350.f});

    // Classic Mode Sprites
    sf::Texture textureSideBush;
    textureSideBush.loadFromFile(SideBush);

    sf::Sprite bushleft(textureSideBush);
    bushleft.setPosition({-58.f, 70.f});
    bushleft.setScale({0.7f, 0.55f});

    sf::Sprite bushright(textureSideBush);
    bushright.setScale({-0.7f, 0.55f});
    bushright.setPosition({915.f, 70.f});

    sf::Texture textureLightGrass, textureDarkGrass;
    textureLightGrass.loadFromFile(LightGrass);
    textureDarkGrass.loadFromFile(DarkGrass);
    auto grassTiles = patternmaker(textureLightGrass, textureDarkGrass);

    // Rapid mode sprites
    sf::Texture textureRapidBg;
    textureRapidBg.loadFromFile(RapidBg);
    sf::Sprite rapidBg(textureRapidBg);
    rapidBg.setScale({0.42f, 0.36f});

    // Game Logic Setup
    sf::Clock deltaClock;
    LaneManager laneManager;
    int selectedLane = 2; // Default to middle lane (0 to 4)

    sf::Texture whiteSheepTexture, blackSheepTexture;
    whiteSheepTexture.loadFromFile(WhiteSheep);
    blackSheepTexture.loadFromFile(BlackSheep);


    // Initialize Rapid Mode Manager
    RapidMode rapidMode(whiteSheepTexture, blackSheepTexture);

    // States of Game
    enum class State
    {
        Title,
        Classic,
        Rapid,
        Pause,
        Resume
    };

    // Default State
    State currentState = State::Title;

    // Game Loop
    while (window.isOpen())
    {
        // 1. Calculate Frame Delta Time
        float dt = deltaClock.restart().asSeconds();

        // 2. Event handling
        while (const std::optional event = window.pollEvent())
        {
            // Closing Event
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            // Key Pressing Recorder
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                // Returning to Title Screen on Backspace
                if (keyPressed->code == sf::Keyboard::Key::Backspace)
                {
                    currentState = State::Title;
                }

                // Exiting when ESCAPE is pressed on Title Screen
                if (keyPressed->code == sf::Keyboard::Key::Escape && currentState == State::Title)
                {
                    window.close();
                }

                // Title Screen Controls
                if (currentState == State::Title)
                {
                    if (keyPressed->code == sf::Keyboard::Key::Down)
                    {
                        ModeNumber = (ModeNumber + 1) % 3;
                    }

                    if (keyPressed->code == sf::Keyboard::Key::Up)
                    {
                        ModeNumber = (ModeNumber + 2) % 3;
                    }

                    if (keyPressed->code == sf::Keyboard::Key::Enter)
                    {
                        switch (ModeNumber)
                        {
                        case 0:
                            currentState = State::Classic;
                            break;
                        case 1:
                            currentState = State::Rapid;
                                rapidMode.reset();
                            break;
                        case 2:
                            window.close();
                            break;
                        }
                    }
                }

                // Classic Mode Gameplay Controls
                else if (currentState == State::Classic)
                {
                    // Select Lane (W/S or Up/Down)
                    if (keyPressed->code == sf::Keyboard::Key::W || keyPressed->code == sf::Keyboard::Key::Up)
                    {
                        selectedLane = (selectedLane + 4) % 5;
                    }
                    if (keyPressed->code == sf::Keyboard::Key::S || keyPressed->code == sf::Keyboard::Key::Down)
                    {
                        selectedLane = (selectedLane + 1) % 5;
                    }

                    // Spawn White Sheep (Keys 1-4)
                    if (keyPressed->code == sf::Keyboard::Key::Num1) {
                        laneManager.spawnSheep(Team::White, SheepSize::Small, selectedLane, whiteSheepTexture);
                    }
                    if (keyPressed->code == sf::Keyboard::Key::Num2) {
                        laneManager.spawnSheep(Team::White, SheepSize::Medium, selectedLane, whiteSheepTexture);
                    }
                    if (keyPressed->code == sf::Keyboard::Key::Num3) {
                        laneManager.spawnSheep(Team::White, SheepSize::Large, selectedLane, whiteSheepTexture);
                    }
                    if (keyPressed->code == sf::Keyboard::Key::Num4) {
                        laneManager.spawnSheep(Team::White, SheepSize::Giant, selectedLane, whiteSheepTexture);
                    }

                    // Spawn Black Sheep (Keys I, O, P, L for testing collision)
                    if (keyPressed->code == sf::Keyboard::Key::I) {
                        laneManager.spawnSheep(Team::Black, SheepSize::Small, selectedLane, blackSheepTexture);
                    }
                    if (keyPressed->code == sf::Keyboard::Key::O) {
                        laneManager.spawnSheep(Team::Black, SheepSize::Medium, selectedLane, blackSheepTexture);
                    }
                    if (keyPressed->code == sf::Keyboard::Key::P) {
                        laneManager.spawnSheep(Team::Black, SheepSize::Large, selectedLane, blackSheepTexture);
                    }
                    if (keyPressed->code == sf::Keyboard::Key::L) {
                        laneManager.spawnSheep(Team::Black, SheepSize::Giant, selectedLane, blackSheepTexture);
                    }
                }
                else if (currentState == State::Rapid)
                {
                    // Pass keypresses (A / L / R) to Rapid Mode
                    rapidMode.handleKeyPress(keyPressed->code);
                }
            }
        }

        // 3. Update Menu Text
        switch (ModeNumber)
        {
        case 0:
            TitleTextMode.setString("Press Enter to Play Classic Mode");
            TitleModeMenuSelector.setString(">");
            TitleModeMenuSelector.setPosition({205.f, 150.f});
            break;

        case 1:
            TitleTextMode.setString("Press Enter to Play Rapid Mode");
            TitleModeMenuSelector.setString(">");
            TitleModeMenuSelector.setPosition({205.f, 250.f});
            break;

        case 2:
            TitleTextMode.setString("Press Enter to Exit");
            TitleModeMenuSelector.setString(">");
            TitleModeMenuSelector.setPosition({205.f, 350.f});
            break;
        }

        // 4. Update Game Logic
        if (currentState == State::Classic)
        {
            laneManager.update(dt);

        }
        else if (currentState == State::Rapid)
        {
            rapidMode.update(dt); //  TICK-DOWNS THE COUNTDOWN TIMER!
        }

        // 5. Draw Frame
        window.clear();

        switch (currentState)
        {
        case State::Title:
            window.draw(TitleText);                 // Bheda TITLE
            window.draw(TitleModeMenu1);            // Classic Mode
            window.draw(TitleModeMenu2);            // Rapid Mode
            window.draw(TitleModeMenu3);            // Exit
            window.draw(TitleTextMode);             // Bottom Text
            window.draw(TitleModeMenuSelector);     // ">" Selector
            break;

        case State::Classic:
            for (auto& tile : grassTiles) window.draw(tile); // Grass Tiles
                laneManager.draw(window);                        // Draw Sheep
            window.draw(bushleft);                           // Left Bush
            window.draw(bushright);                          // Right Bush

            break;

        case State::Rapid:
            window.draw(rapidBg);
                rapidMode.draw(window, font);      // Draws countdown, sheep, and winner announcement
            break;

        case State::Pause:
            break;

        case State::Resume:
            break;
        }

        // 6. Display Everything Drawn
        window.display();
    }

    return 0;
}
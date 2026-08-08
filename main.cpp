#include <iostream>
#include <SFML/Graphics.hpp>
#include "DEFINITIONS.hpp"


    //Tile Pattern Creattion
    std::vector<sf::Sprite> patternmaker(const sf::Texture& light, const sf::Texture& dark) {
        std::vector<sf::Sprite> tiles;
        float posX = -105, posY = 80;
        int pattern = 0;

        for (int i = 0; i < 5; i++) {
            posX = -105;
            for (int j = 0; j < 10; j++) {
                sf::Sprite sprite(pattern % 2 == 0 ? light : dark);
                sprite.setScale({0.43f, 0.43f});
                sprite.setPosition({posX, posY});
                tiles.push_back(sprite);
                pattern++;
                posX += 105;
            }
            posY += 80;
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
        std::cout << "Failed to load font\n";
        return -1;
    }

    // Window
    sf::RenderWindow window(
        sf::VideoMode({WindowLength, WindowBreadth}),"Bheda"
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

    //Mode Menu Selector (>)
    sf::Text TitleModeMenuSelector(font, "", 35);
    TitleModeMenuSelector.setFillColor(sf::Color::White);


    //MOde Menu List
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

    //Classic Text
    // sf::Text ClassicText(font,"You Entered Classic Mode!!!!",30);
    // ClassicText.setFillColor(sf::Color::White);
    // ClassicText.setPosition({320.f, 350.f});

    //Rapid Text
    // sf::Text RapidText(font,"You Entered Rapid Mode!!!!",30);
    // RapidText.setFillColor(sf::Color::White);
    // RapidText.setPosition({320.f, 350.f});



        //Classic Mode Sprites

        sf::Texture textureSideBush;
        textureSideBush.loadFromFile(SideBush);

        sf::Sprite bushleft(textureSideBush);
        bushleft.setPosition({-58, 70});
        bushleft.setScale({0.7,0.55});

        sf::Sprite bushright(textureSideBush);
        bushright.setScale({-0.7,0.55});
        bushright.setPosition({915, 70});




    sf::Texture textureLightGrass, textureDarkGrass;
    textureLightGrass.loadFromFile(LightGrass);
    textureDarkGrass.loadFromFile(DarkGrass);
    auto grassTiles = patternmaker(textureLightGrass, textureDarkGrass);



    //Rapid mode sprites
        sf::Texture textureRapidBg;
        textureRapidBg.loadFromFile(RapidBg);
        sf::Sprite rapidBg(textureRapidBg);
        rapidBg.setScale({0.42,0.36});



    //
    //
    // sf::Texture textureStoneGrass;
    // textureStoneGrass.loadFromFile(StoneGrass);
    // sf::Sprite spriteStoneGrass(textureStoneGrass);
    //
    //
    //
    // textureSideBush.loadFromFile(SideBush);
    // sf::Sprite spriteSideBush(textureSideBush);

    //Classic Background
    // sf::Texture textureBackground;
    // textureBackground.loadFromFile(Background);
    // sf::Sprite spriteBackground(textureBackground);
    //
    // sf::Vector2u textureSize = textureBackground.getSize();
    // spriteBackground.setScale({
    //     800.f / textureSize.x,
    //     550.f / textureSize.y
    // });





    //States of Game
    enum class State
    {
        Title,
        Classic,
        Rapid,
        Pause,
        Resume
    };

    //Default State
    State currentState = State::Title;


    //Game Loop
    while (window.isOpen())
    {
        //Event handling
        while (const std::optional event = window.pollEvent())
        {

            //Closing Event
            if (event->is<sf::Event::Closed>() )
            {
                window.close();
            }

            //Key Pressing Recorder
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                //Returning to Title Screen on Backsapce
                if (keyPressed->code == sf::Keyboard::Key::Backspace)
                {
                    currentState=State::Title;
                }

                //Exiting when ESCAPE is pressed on Title Screen
                if (keyPressed->code==sf::Keyboard::Key::Escape && currentState==State::Title)
                {
                    window.close();
                }


                if (currentState == State::Title)
                {
                    // Move through menu
                    if (keyPressed->code == sf::Keyboard::Key::Down)
                    {
                        ModeNumber = (ModeNumber + 1) % 3;
                    }

                    if (keyPressed->code == sf::Keyboard::Key::Up)
                    {
                        ModeNumber = (ModeNumber + 2) % 3;
                    }

                    // Select menu option
                    if (keyPressed->code == sf::Keyboard::Key::Enter)
                    {
                        switch (ModeNumber)
                        {
                        case 0:
                            currentState = State::Classic;
                            break;

                        case 1:
                            currentState = State::Rapid;
                            break;

                        case 2:
                            window.close();
                            break;
                        }
                    }
                }
            }
        }

        // Update menu text
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

        // Draw
        window.clear();

        switch (currentState)
        {
        case State::Title:
            window.draw(TitleText);                 //Bheda TITLE
                window.draw(TitleModeMenu1);        //Classic Mode
                window.draw(TitleModeMenu2);        //Rapid Mode
                window.draw(TitleModeMenu3);        //Exit
            window.draw(TitleTextMode);             //Bottom Text
            window.draw(TitleModeMenuSelector);     // ">" Selector
            break;

        case State::Classic:
                for (auto& tile : grassTiles) window.draw(tile);    //Tile creation
                window.draw(bushleft);                                     //Left Bush
                window.draw(bushright);                                    //Right Bush
            break;

        case State::Rapid:
                window.draw(rapidBg);
            break;

        case State::Pause:
            break;

        case State::Resume:
            break;
        }

        //Display Everything Drawn
        window.display();
    }

    return 0;
}
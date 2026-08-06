#include <iostream>
#include <SFML/Graphics.hpp>
#include "DEFINITIONS.hpp"

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
    TitleModeMenu1.setString("Classic");
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
    sf::Text ClassicText(font,"You Entered Classic Mode!!!!",30);
    ClassicText.setFillColor(sf::Color::White);
    ClassicText.setPosition({320.f, 350.f});

    //Rapid Text
    sf::Text RapidText(font,"You Entered Classic Mode!!!!",30);
    RapidText.setFillColor(sf::Color::White);
    RapidText.setPosition({320.f, 350.f});



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
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            //Key Pressing Recorder
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                //Returning to Title Screen on Backsapce
                if (keyPressed->code == sf::Keyboard::Key::Backspace) {
                    currentState=State::Title;
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
            window.draw(TitleText);
                window.draw(TitleModeMenu1);
                window.draw(TitleModeMenu2);
                window.draw(TitleModeMenu3);
            window.draw(TitleTextMode);
            window.draw(TitleModeMenuSelector);
            break;

        case State::Classic:
                window.draw(ClassicText);
            break;

        case State::Rapid:
                window.draw(RapidText);
            break;

        case State::Pause:
            break;

        case State::Resume:
            break;
        }

        window.display();
    }

    return 0;
}
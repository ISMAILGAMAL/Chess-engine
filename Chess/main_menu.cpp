#include<SFML/Graphics.hpp>
#include<iostream>
#include "button.cpp"

using namespace std;

struct MainMenu {
private:
    sf::Texture backgroundTexture;
public:
    sf::RectangleShape background;
    Button singlePlayer = Button("assets/SinglePlayer.png", "assets/SinglePlayerIcon.png");
    Button multiPlayer = Button("assets/MultiPlayer.png", "assets/MultiPlayerIcon.png");
    Button settings = Button("assets/Settings.png", "assets/SettingsIcon.png");
    Button exit = Button("assets/Exit.png", "assets/ExitIcon.png");


    //Pass window.getSize() only to setSize of the background
    MainMenu(sf::Vector2u windowSize) {

        if (backgroundTexture.loadFromFile("assets/backgroundNew2.png") == false) {
            cout << "Can\'t load background\n";
        }

        //set background texture and size
        background.setTexture(&backgroundTexture);
        background.setSize(sf::Vector2f(windowSize.x, windowSize.y));

        //Change the position buttons to be Centered in the window
        singlePlayer.setPosition(windowSize.x / 2, windowSize.y / 2 - 70);
        multiPlayer.setPosition(windowSize.x / 2, windowSize.y / 2 + 40);
        settings.setPosition(windowSize.x / 2, windowSize.y / 2 + 150);
        exit.setPosition(windowSize.x / 2, windowSize.y / 2 + 260);
    }

    //Draw MainMenu
    void DisplayMainMenu(sf::RenderWindow& window) {
        window.draw(background);
        singlePlayer.DisplayButton(window);
        multiPlayer.DisplayButton(window);
        settings.DisplayButton(window);
        exit.DisplayButton(window);
    }
};
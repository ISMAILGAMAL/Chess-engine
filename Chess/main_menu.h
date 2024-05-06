#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
#include "button.h"

struct SettingsPage {
private:
    sf::Texture backgroundTexture, soundOnIconTexture, soundOffIconTexture, themesTexture[5];
    sf::Texture whiteTexture, blackTexture, selectedTexture, backButtonTexture;
    sf::Font font;
    bool soundState = 1;

    int current_color = 1;
    // 1 -> white color first time
    // 2 -> black color first time
public:
    string current_theme = "1w";
    sf::RectangleShape background, selectedTheme, selectedColor;
    sf::Sprite soundIcon, whiteColor, blackColor, themes[5], backButton;
    sf::Text soundIconText, themeText, colorText;
    SettingsPage() {
        if (backgroundTexture.loadFromFile("assets/SettingsBackgroundSquare.png") == false)
            cout << "Can\'t load Settings background\n";
        if (soundOnIconTexture.loadFromFile("assets/sound_on.png") == false)
            cout << "Can\'t load Sonud icon on \n";
        if (soundOffIconTexture.loadFromFile("assets/sound_off.png") == false)
            cout << "Can\'t load Sonud icon off \n";
        if (whiteTexture.loadFromFile("assets/whiteknight.png") == false)
            cout << "Can\'t load white knight \n";
        if (blackTexture.loadFromFile("assets/blackknight.png") == false)
            cout << "Can\'t load black knight \n";
        if (selectedTexture.loadFromFile("assets/Selected.png") == false)
            cout << "Can\'t load Selected theme \n";
        if (font.loadFromFile("assets/lucida.ttf") == false)
            cout << "Can\'t load font\n";
        if (backButtonTexture.loadFromFile("assets/BackButton.png") == false)
            cout << "Can\'t load back button \n";
        for (int i = 1; i <= 5; i++)
        {
            string path = "assets/theme" + to_string(i) + ".png";
            themesTexture[i - 1].loadFromFile(path);
            themes[i - 1].setTexture(themesTexture[i - 1]);
            themes[i - 1].setScale(sf::Vector2f(0.22f, 0.22f));
            themes[i - 1].setOrigin(themes[i - 1].getLocalBounds().getSize() / 2.f);
        }

        background.setTexture(&backgroundTexture);
        soundIcon.setTexture(soundOnIconTexture);
        blackColor.setTexture(blackTexture);
        whiteColor.setTexture(whiteTexture);
        selectedColor.setTexture(&selectedTexture);
        selectedTheme.setTexture(&selectedTexture);
        backButton.setTexture(backButtonTexture);

        soundIcon.setOrigin(soundIcon.getLocalBounds().getSize() / 2.f);
        soundIconText.setOrigin(soundIcon.getLocalBounds().getSize() / 2.f);

        selectedColor.setSize(sf::Vector2f(80, 80));
        selectedColor.setOrigin(selectedColor.getLocalBounds().getSize() / 2.f);

        selectedTheme.setSize(sf::Vector2f(120, 120));
        selectedTheme.setOrigin(selectedTheme.getLocalBounds().getSize() / 2.f);

        whiteColor.setOrigin(whiteColor.getLocalBounds().getSize() / 2.f);
        blackColor.setOrigin(whiteColor.getLocalBounds().getSize() / 2.f);

        backButton.setOrigin(backButton.getLocalBounds().getSize() / 2.f);
        backButton.setScale(0.85, 0.85);

        soundIconText.setString("Sound : ");
        themeText.setString("Themes");
        colorText.setString("Player One : ");

        soundIconText.setCharacterSize(40);
        themeText.setCharacterSize(60);
        colorText.setCharacterSize(40);

        soundIconText.setFillColor(sf::Color(237.0, 218.0, 185.0));
        themeText.setFillColor(sf::Color(237.0, 218.0, 185.0));
        colorText.setFillColor(sf::Color(237.0, 218.0, 185.0));

        soundIconText.setFont(font);
        themeText.setFont(font);
        colorText.setFont(font);
    }

    void updateTheme();

    void setSettingsPageSize(sf::Vector2u);

    bool getSoundState();

    void setSoundState(bool state);

    void changeSoundState(sf::Vector2f);

    int getColor();

    void setColor(int);

    void changeTheme(sf::Vector2f, sf::Vector2u);

    void changeColor(sf::Vector2f, sf::Vector2u);

    void SettingsMouseClicks(sf::Vector2f, sf::Vector2u, int&);

    void backToMainMenu(sf::Vector2f, int&);

    void DisplaySettings(sf::RenderWindow&);
};

struct MainMenu {
private:
    sf::Texture backgroundTexture;
public:
    sf::RectangleShape background;
    Button singlePlayer = Button("assets/SinglePlayer.png", "assets/SinglePlayerIcon.png");
    Button multiPlayer = Button("assets/MultiPlayer.png", "assets/MultiPlayerIcon.png");
    Button settings = Button("assets/Settings.png", "assets/SettingsIcon.png");
    Button exit = Button("assets/Exit.png", "assets/ExitIcon.png");
    SettingsPage settingsPage;
    int loadedMenu = 0;
    /*
    * 0 - main menu
    * 1 - single player menu
    * 2 - multi player menu
    * 3 - settings
    */

    //Pass window.getSize() only to setSize of the background
    MainMenu(sf::Vector2u windowSize) {

        if (backgroundTexture.loadFromFile("assets/SquareBackground.png") == false) {
            cout << "Can\'t load background\n";
        }

        settingsPage.setSettingsPageSize(windowSize);
        //set background texture and size
        background.setTexture(&backgroundTexture);
        background.setSize(sf::Vector2f(windowSize.x, windowSize.y));

        //Change the position buttons to be Centered in the window
        singlePlayer.setPosition(windowSize.x / 2, windowSize.y / 2 - 70);
        multiPlayer.setPosition(windowSize.x / 2, windowSize.y / 2 + 40);
        settings.setPosition(windowSize.x / 2, windowSize.y / 2 + 150);
        exit.setPosition(windowSize.x / 2, windowSize.y / 2 + 260);
    }

    // Buttons Animations
    void MenuButtonsAnimation(sf::RenderWindow&);

    //Draw MainMenu
    void DisplayMainMenu(sf::RenderWindow&);

    // All mouse Clicks in Main Menu
    void MainMenuMouseClick(sf::Vector2f, sf::RenderWindow&);
};
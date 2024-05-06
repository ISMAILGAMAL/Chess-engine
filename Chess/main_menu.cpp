#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
#include "main_menu.h"
#include "button.h"

using namespace std;

// Buttons Animations
void MainMenu::MenuButtonsAnimation(sf::RenderWindow& window) {
    auto mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    singlePlayer.IconAnimation(mousePosition);
    multiPlayer.IconAnimation(mousePosition);
    settings.IconAnimation(mousePosition);
    exit.IconAnimation(mousePosition);
}

//Draw MainMenu
void MainMenu::DisplayMainMenu(sf::RenderWindow& window) {
    window.draw(background);
    singlePlayer.DisplayButton(window);
    multiPlayer.DisplayButton(window);
    settings.DisplayButton(window);
    exit.DisplayButton(window);
}// All mouse Clicks in Main Menu

void MainMenu::MainMenuMouseClick(sf::Vector2f mousePosition, sf::RenderWindow& window) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (singlePlayer.buttonsprite.getGlobalBounds().contains(mousePosition)) {
            loadedMenu = 1;
        }
        else if (multiPlayer.buttonsprite.getGlobalBounds().contains(mousePosition)) {
            loadedMenu = 2;
        }
        else if (settings.buttonsprite.getGlobalBounds().contains(mousePosition)) {
            loadedMenu = 3;
        }
        else if (exit.buttonsprite.getGlobalBounds().contains(mousePosition)) {
            window.close();
        }
    }
}

void SettingsPage::setSettingsPageSize(sf::Vector2u windowSize) {
    background.setSize(sf::Vector2f(windowSize.x, windowSize.y));

    soundIcon.setPosition(windowSize.x / 2 - 50, windowSize.y / 2 - 102);
    soundIconText.setPosition(windowSize.x / 2 - 220, windowSize.y / 2 - 100);

    colorText.setPosition(windowSize.x / 2 - 250, windowSize.y / 2 - 30);
    whiteColor.setPosition(windowSize.x / 2 + 70, windowSize.y / 2 - 10);
    blackColor.setPosition(windowSize.x / 2 + 160, windowSize.y / 2 - 10);
    selectedColor.setPosition(windowSize.x / 2 + 70, windowSize.y / 2 - 10);

    themeText.setPosition(windowSize.x / 2 - themeText.getLocalBounds().width / 2, windowSize.y / 2 + 50);
    selectedTheme.setPosition(windowSize.x / 2 - 240, windowSize.y / 2 + 200);
    themes[0].setPosition(windowSize.x / 2 - 240, windowSize.y / 2 + 200);
    themes[1].setPosition(windowSize.x / 2 - 120, windowSize.y / 2 + 200);
    themes[2].setPosition(windowSize.x / 2, windowSize.y / 2 + 200);
    themes[3].setPosition(windowSize.x / 2 + 120, windowSize.y / 2 + 200);
    themes[4].setPosition(windowSize.x / 2 + 240, windowSize.y / 2 + 200);


    backButton.setPosition(130, 128);
}

bool SettingsPage::getSoundState() {
    return soundState;
}

void SettingsPage::setSoundState(bool state) {
    soundState = state;
}

void SettingsPage::changeSoundState(sf::Vector2f mousePosition) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (soundIcon.getGlobalBounds().contains(mousePosition)) {
            if (getSoundState()) {
                setSoundState(0);

                soundIcon.setTexture(soundOffIconTexture);
            }
            else {
                setSoundState(1);

                soundIcon.setTexture(soundOnIconTexture);
            }
        }
    }
}

void SettingsPage::changeTheme(sf::Vector2f mousePosition, sf::Vector2u windowSize) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        for (int i = 0; i < 5; i++)
        {
            if (themes[i].getGlobalBounds().contains(mousePosition)) {
                if (current_color == 1)
                    current_theme = to_string(i+1) + "w";
                else
					current_theme = to_string(i+1) + "b";
                selectedTheme.setPosition(themes[i].getPosition());
                break;
            }
        }
    }
}

void SettingsPage::setColor(int color) {
    current_color = color;
}

int SettingsPage::getColor() {
    return current_color;
}

void SettingsPage::changeColor(sf::Vector2f mousePosition, sf::Vector2u windowSize) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (whiteColor.getGlobalBounds().contains(mousePosition)) {
            setColor(1);
            current_theme[1] = 'w';
            selectedColor.setPosition(whiteColor.getPosition());
        }
        else if (blackColor.getGlobalBounds().contains(mousePosition)) {
            setColor(2);
            current_theme[1] = 'b';
            selectedColor.setPosition(blackColor.getPosition());
        }
    }
}

void SettingsPage::SettingsMouseClicks(sf::Vector2f mousePosition, sf::Vector2u windowSize, int& menu) {
    changeTheme(mousePosition, windowSize);
    changeColor(mousePosition, windowSize);
    changeSoundState(mousePosition);
    backToMainMenu(mousePosition, menu);
}

void SettingsPage::backToMainMenu(sf::Vector2f mousePosition, int& menu) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (backButton.getGlobalBounds().contains(mousePosition)) {
            menu = 0;
        }
    }
}

void SettingsPage::DisplaySettings(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(soundIcon);
    window.draw(soundIconText);
    window.draw(colorText);
    window.draw(whiteColor);
    window.draw(blackColor);
    window.draw(selectedColor);
    window.draw(themeText);
    for (int i = 0; i < 5; i++)
    {
        window.draw(themes[i]);
    }
    window.draw(selectedTheme);
    window.draw(backButton);
}

void SettingsPage::updateTheme() {
    switch (current_theme[0]-'0')
    {
    case 1:
        selectedTheme.setPosition(themes[0].getPosition());
        break;
    case 2:
        selectedTheme.setPosition(themes[1].getPosition());
        break;
    case 3:
        selectedTheme.setPosition(themes[2].getPosition());
        break;
    case 4:
        selectedTheme.setPosition(themes[3].getPosition());
        break;
    case 5:
        selectedTheme.setPosition(themes[4].getPosition());
        break;
    }
}


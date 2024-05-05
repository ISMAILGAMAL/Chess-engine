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

    soundIcon.setPosition(windowSize.x / 2 - 30, windowSize.y / 2 - 102);
    soundIconText.setPosition(windowSize.x / 2 - 200, windowSize.y / 2 - 100);

    themeText.setPosition(windowSize.x / 2 - 235, windowSize.y / 2 + 10);
    basicTheme.setPosition(windowSize.x / 2 + 30, windowSize.y / 2 + 40);
    selectedTheme.setPosition(windowSize.x / 2 + 30, windowSize.y / 2 + 40);

    colorText.setPosition(windowSize.x / 2 - 235, windowSize.y / 2 + 180);
    whiteColor.setPosition(windowSize.x / 2 + 70, windowSize.y / 2 + 205);
    blackColor.setPosition(windowSize.x / 2 + 160, windowSize.y / 2 + 205);
    selectedColor.setPosition(windowSize.x / 2 + 70, windowSize.y / 2 + 205);

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

void SettingsPage::setTheme(int themeNumber) {
    current_theme = themeNumber;
}

int SettingsPage::getTheme() {
    return current_theme;
}

void SettingsPage::changeTheme(sf::Vector2f mousePosition, sf::Vector2u windowSize) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (basicTheme.getGlobalBounds().contains(mousePosition)) {
            current_theme = 1;
            selectedTheme.setPosition(windowSize.x / 2 + 30, windowSize.y / 2 + 40);
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
            setColor(3);
            selectedColor.setPosition(windowSize.x / 2 + 70, windowSize.y / 2 + 205);
        }
        else if (blackColor.getGlobalBounds().contains(mousePosition)) {
            setColor(4);
            selectedColor.setPosition(windowSize.x / 2 + 160, windowSize.y / 2 + 205);
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
    window.draw(basicTheme);
    window.draw(selectedTheme);
    window.draw(backButton);
}
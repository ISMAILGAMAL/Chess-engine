#include<SFML/Graphics.hpp>
#include<iostream>
#include "button.h"
using namespace std;

	//Draw button Sprites
void Button::DisplayButton(sf::RenderWindow& window) {
	window.draw(buttonsprite);
	window.draw(iconsprite);
}

	//Set the position of the sprites
void Button::setPosition(float x, float y) {
	buttonsprite.setPosition(x, y);
	iconsprite.setPosition(x - 135, y - 10);
}

	/////Set the scale of the iconSprite
	//use this function to make the icon scale bigger when
	//we hover on it with mouse

void Button::IconAnimation(sf::Vector2f mousePosition) {
	if (buttonsprite.getGlobalBounds().contains(mousePosition))
		iconsprite.setScale(0.8, 0.8);
	else
		iconsprite.setScale(0.7, 0.7);
}
#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
using namespace std;

struct Button {
private:
	//Load the Button and Icon Textures 
	sf::Texture buttontexture, icontexture;
public:
	sf::Sprite buttonsprite, iconsprite;
	Button(string buttonTexture, string iconTexture) {
		//Check all loaded textures
		if (buttontexture.loadFromFile(buttonTexture) == false)
			cout << "Error loading button texture" << endl;
		if (icontexture.loadFromFile(iconTexture) == false)
			cout << "Error loading icon texture" << endl;

		//Set Textures for the selected sprites
		buttonsprite.setTexture(buttontexture);
		iconsprite.setTexture(icontexture);

		//Set the Origin of the sprites in the middle
		buttonsprite.setOrigin(buttonsprite.getLocalBounds().width / 2, buttonsprite.getLocalBounds().height / 2);
		iconsprite.setOrigin(iconsprite.getLocalBounds().width / 2, iconsprite.getLocalBounds().height / 2);

		//Set Scale of Sprites to fit window
		buttonsprite.setScale(0.3, 0.3);
		iconsprite.setScale(0.7, 0.7);
	}

	//Draw button Sprites
	void DisplayButton(sf::RenderWindow&);

	//Set the position of the sprites
	void setPosition(float, float);

	/////Set the scale of the iconSprite
	//use this function to make the icon scale bigger when
	//we hover on it with mouse
	void IconAnimation(sf::Vector2f);
};
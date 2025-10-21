#pragma once
#include <glut.h>

class Door {
private:
	float x;
	float y;

	// Dimensions
	float doorWidth;
	float doorHeight;
	float handleRadius;
	float keyHoleSize;

	// Colors
	float doorColor[3];
	float handleColor[3];
	float keyHoleColor[3];
	float openDoorColor[3];

	// State
	bool isOpen;

public:
	Door(float startX, float startY, float width = 50.0f, float height = 80.0f);

	void render();

	void open();
	bool getIsOpen() const { return isOpen; }

	float getX() const { return x; }
	float getY() const { return y; }
	float getWidth() const { return doorWidth; }
	float getHeight() const { return doorHeight; }

	void setPosition(float newX, float newY);
};

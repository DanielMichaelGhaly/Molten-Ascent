#pragma once
#include <glut.h>

// will be a rectangle and on top of it semi-triangle to show irregular shape
class Rock {
	float x;
	float y;

	float baseWidth;
	float baseHeight;
	float peakHeight;

	float rockColor[3];

	Rock(float startX, float startY, float width = 40.0f, float height = 30.0f);

	void render();

	float getX() const { return x; }
	float getY() const { return y; }
	float getWidth() const {return baseWidth;}
	float getHeight() const { return baseHeight + peakHeight; }

	void setPosition(float newX, float newY);
};
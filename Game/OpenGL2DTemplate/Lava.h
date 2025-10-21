#pragma once
#include <glut.h>

class Lava {

private:
	float x;
	float y;

	float width;
	float height;
	float maxHeight;
	float growthRate;

	float lavaColor[3];
	float bubbleColor[3];

	float animationTime;
	float bubbleOffset1;
	float bubbleOffset2;
	float bubbleOffset3;

public:
	Lava(float screenWidth, float startY = 0.0f, float initialHeight = 1.0f);

	void update(float deltaTime);
	void render();

	void startGrowing();
	void setGrowthRate(float rate);

	bool isTouching(float objY);

	float getX() const { return x; }
	float getY() const { return y; }
	float getWidth() const { return width; }
	float getHeight() const { return height; }
	float getTopY() const { return y + height; }

	void setPosition(float newX, float newY);
	void setHeight(float newHeight);
	void setMaxHeight(float maxH);
};
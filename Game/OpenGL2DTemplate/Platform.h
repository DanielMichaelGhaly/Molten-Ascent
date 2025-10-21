#pragma once
#include <glut.h>

class Platform {
private:

	float x;
	float y;

	float width;
	float height;
	
	float baseColor[3];
	float edgeColor[3];
	float decorColor[3];

	float animationTime;
	float bobOffset;

	bool isVisible;

public:

	Platform(float startX, float startY, float platformWidth = 120.0f, float platformHeight = 20.0f);

	void update(float deltaTime);
	void render();

	void hide();
	void show();
	bool getIsVisible() const { return isVisible; }

	bool isPlayerOnTop(float playerX, float playerY, float playerWidth, float playerHeight) const;
	bool isPlayerColliding(float playerX, float playerY, float playerWidth, float playerHeight) const;

	float getX() const { return x; }
	float getY() const { return y; }
	float getWidth() const { return width; }
	float getHeight() const { return height; }
	float getTop() const { return y + height; }
	float getBottom() const { return y; }
	float getLeft() const { return x - width / 2; }
	float getRight() const { return x + width / 2; }

	void setPosition(float newX, float newY);
};

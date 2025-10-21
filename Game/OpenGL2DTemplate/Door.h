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

	bool isOpen;
	bool isUnlocking;  
	
	// Animation
	float animationProgress;  // 0.0 to 1.0
	float animationSpeed;     // Speed of animation
	float rotationAngle;      // Current rotation during animation
	float scaleAmount;        // Current scale during animation

public:
	Door(float startX, float startY, float width = 50.0f, float height = 80.0f);

	void update(float deltaTime);
	void render();

	void unlock();  
	bool getIsOpen() const { return isOpen; }
	bool getIsUnlocking() const { return isUnlocking; }

	float getX() const { return x; }
	float getY() const { return y; }
	float getWidth() const { return doorWidth; }
	float getHeight() const { return doorHeight; }

	void setPosition(float newX, float newY);
};

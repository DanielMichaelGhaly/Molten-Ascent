#pragma once
#include <glut.h>

class Collectable {
private:
	// Position
	float x;
	float y;

	// Dimensions
	float size;
	
	// Colors
	float gemColor[3];
	float sparkleColor[3];
	float baseColor[3];

	// Animation
	float rotationAngle;
	float scaleAnimation;
	float animationTime;
	bool isVisible;

public:
	// Constructor
	Collectable(float startX, float startY, float gemSize = 20.0f);

	// Update and render
	void update(float deltaTime);
	void render();

	// Collection
	void collect();
	bool getIsVisible() const { return isVisible; }

	// Collision detection
	bool isColliding(float objX, float objY, float objRadius) const;

	// Getters
	float getX() const { return x; }
	float getY() const { return y; }
	float getSize() const { return size; }

	// Setters
	void setPosition(float newX, float newY);
};
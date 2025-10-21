#pragma once
#include <glut.h>

enum class PowerUpType {
	SPEED_BOOST,
	SHIELD
};

class PowerUp {
private:
	// Position
	float x;
	float y;

	// Properties
	PowerUpType type;
	float size;
	float duration;        // How long power-up lasts when active
	float lifeTime;        // How long it stays on screen if not collected
	float remainingLife;   // Time left before disappearing
	
	// Colors
	float primaryColor[3];
	float secondaryColor[3];
	float accentColor[3];

	// Animation
	float rotationAngle;
	float scaleAnimation;
	float pulseAnimation;
	float animationTime;
	bool isVisible;
	bool isActive;         // When collected and effect is active

public:
	// Constructor
	PowerUp(PowerUpType powerType, float startX, float startY, float powerSize = 25.0f);

	// Update and render
	void update(float deltaTime);
	void render();
	void renderActiveEffect();  // Visual cue when power-up is active

	// Collection and activation
	void collect();
	void activate();
	void deactivate();
	bool getIsVisible() const { return isVisible; }
	bool getIsActive() const { return isActive; }
	float getRemainingDuration() const { return duration; }

	// Collision detection
	bool isColliding(float objX, float objY, float objRadius) const;

	// Getters
	float getX() const { return x; }
	float getY() const { return y; }
	float getSize() const { return size; }
	PowerUpType getType() const { return type; }

	// Setters
	void setPosition(float newX, float newY);
};

#pragma once
#include <glut.h>

class Player {

	// Player position
	float x;
	float y;

	// Velocity
	float vx;
	float vy;

	// Dimensions for the primitives (head,torso,arms,legs, eyes, cap) => (triangle_fan, quads, quads, quads, points, triangle)
	float headRadius;
	float torsoWidth;
	float torsoHeight;
	float armWidth;
	float armLength;
	float legWidth;
	float legLength;
	float capSize;

	float headColor[3];
	float torsoColor[3];
	float legColor[3];
	float armColor[3];
	float capColor[3];

	float jumpOffset;
	bool isJumping;

	Player(float startX, float startY);
	
	void update(float deltaTime);
	void render();

	void moveLeft();
	void moveRight();
	void jump();
	
	float getX() const { return x; }
	float getY() const { return y; }
	float getVX() const { return vx; }
	float getVY() const { return vy; }
	float getWidth() const { return torsoWidth; }
	float getHeight() const { return headRadius * 2 + torsoHeight + legLength; }


	void setPosition(float newX, float newY);
	void setVelocity(float newVX, float newVY);
	void stopHorizontalMovement();
	void setGrounded(bool grounded);
};
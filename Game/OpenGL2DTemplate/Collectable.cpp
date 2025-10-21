#include "Collectable.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Collectable::Collectable(float startX, float startY, float gemSize)
	: x(startX), y(startY), size(gemSize),
	rotationAngle(0.0f), scaleAnimation(1.0f), animationTime(0.0f),
	isVisible(true)
{
	// Gem color (blue crystal)
	gemColor[0] = 0.2f; gemColor[1] = 0.6f; gemColor[2] = 1.0f;

	// Sparkle color (white)
	sparkleColor[0] = 1.0f; sparkleColor[1] = 1.0f; sparkleColor[2] = 1.0f;

	// Base color (dark blue)
	baseColor[0] = 0.1f; baseColor[1] = 0.3f; baseColor[2] = 0.5f;
}

void Collectable::update(float deltaTime) {
	if (!isVisible) return;

	animationTime += deltaTime;

	// Continuous rotation
	rotationAngle += 120.0f * deltaTime;  // 120 degrees per second
	if (rotationAngle >= 360.0f) {
		rotationAngle -= 360.0f;
	}

	// Pulsing scale animation
	scaleAnimation = 1.0f + 0.2f * sin(animationTime * 3.0f);
}

void Collectable::render() {
	if (!isVisible) return;

	glPushMatrix();
	glTranslatef(x, y, 0.0f);
	glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
	glScalef(scaleAnimation, scaleAnimation, 1.0f);

	// 1. Base (circle - triangle fan)
	glColor3fv(baseColor);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.0f, 0.0f);
	for (int i = 0; i <= 20; i++) {
		float angle = 2.0f * M_PI * i / 20;
		glVertex2f(cos(angle) * size * 0.6f, sin(angle) * size * 0.6f);
	}
	glEnd();

	// 2. Main gem (hexagon using triangles)
	glColor3fv(gemColor);
	float hexRadius = size * 0.5f;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.0f, 0.0f);
	for (int i = 0; i <= 6; i++) {
		float angle = 2.0f * M_PI * i / 6;
		glVertex2f(cos(angle) * hexRadius, sin(angle) * hexRadius);
	}
	glEnd();

	// 3. Sparkle points (small triangles)
	glColor3fv(sparkleColor);
	float sparkleSize = size * 0.15f;
	
	// Top sparkle
	glBegin(GL_TRIANGLES);
	glVertex2f(0.0f, size * 0.7f);
	glVertex2f(-sparkleSize, size * 0.4f);
	glVertex2f(sparkleSize, size * 0.4f);
	glEnd();

	// Left sparkle
	glBegin(GL_TRIANGLES);
	glVertex2f(-size * 0.6f, 0.0f);
	glVertex2f(-size * 0.3f, -sparkleSize);
	glVertex2f(-size * 0.3f, sparkleSize);
	glEnd();

	// Right sparkle
	glBegin(GL_TRIANGLES);
	glVertex2f(size * 0.6f, 0.0f);
	glVertex2f(size * 0.3f, -sparkleSize);
	glVertex2f(size * 0.3f, sparkleSize);
	glEnd();

	glPopMatrix();
}

void Collectable::collect() {
	isVisible = false;
}

bool Collectable::isColliding(float objX, float objY, float objRadius) const {
	if (!isVisible) return false;

	float dx = x - objX;
	float dy = y - objY;
	float distance = sqrt(dx * dx + dy * dy);
	return distance < (size * 0.5f + objRadius);
}

void Collectable::setPosition(float newX, float newY) {
	x = newX;
	y = newY;
}
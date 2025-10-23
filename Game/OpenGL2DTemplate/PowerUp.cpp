#include "PowerUp.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

PowerUp::PowerUp(PowerUpType powerType, float startX, float startY, float powerSize)
	: type(powerType), x(startX), y(startY), size(powerSize),
	duration(8.0f), lifeTime(15.0f), remainingLife(15.0f),
	rotationAngle(0.0f), scaleAnimation(1.0f), pulseAnimation(0.0f),
	animationTime(0.0f), isVisible(true), isActive(false)
{
	switch (type) {
	case PowerUpType::SPEED_BOOST:
		// Lightning/Speed - Yellow/Orange
		primaryColor[0] = 1.0f; primaryColor[1] = 0.9f; primaryColor[2] = 0.0f;
		secondaryColor[0] = 1.0f; secondaryColor[1] = 0.5f; secondaryColor[2] = 0.0f;
		accentColor[0] = 1.0f; accentColor[1] = 1.0f; accentColor[2] = 1.0f;
		break;
	case PowerUpType::SHIELD:
		// Shield - Blue/Purple
		primaryColor[0] = 0.2f; primaryColor[1] = 0.4f; primaryColor[2] = 1.0f;
		secondaryColor[0] = 0.6f; secondaryColor[1] = 0.2f; secondaryColor[2] = 0.9f;
		accentColor[0] = 0.8f; accentColor[1] = 0.8f; accentColor[2] = 1.0f;
		break;
	}
}

void PowerUp::update(float deltaTime) {
	if (!isVisible && !isActive) return;

	animationTime += deltaTime;

	if (isVisible && !isActive) {
		// Countdown to disappear if not collected
		remainingLife -= deltaTime;
		if (remainingLife <= 0.0f) {
			isVisible = false;
			return;
		}
	}

	if (isActive) {
		// Countdown active duration
		duration -= deltaTime;
		if (duration <= 0.0f) {
			deactivate();
			return;
		}
	}

	// Continuous rotation
	rotationAngle += 90.0f * deltaTime;
	if (rotationAngle >= 360.0f) {
		rotationAngle -= 360.0f;
	}

	// Pulsing scale
	scaleAnimation = 1.0f + 0.3f * sin(animationTime * 4.0f);

	// Pulse effect for low time warning
	if (isVisible && remainingLife < 5.0f) {
		pulseAnimation = 0.5f + 0.5f * sin(animationTime * 8.0f);
	} else {
		pulseAnimation = 1.0f;
	}
}

void PowerUp::render() {
	if (!isVisible) return;

	glPushMatrix();
	glTranslatef(x, y, 0.0f);
	glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
	glScalef(scaleAnimation, scaleAnimation, 1.0f);

	float alpha = pulseAnimation;

	switch (type) {
	case PowerUpType::SPEED_BOOST:
		{
			// Lightning bolt design - 3 primitives
			// 1. Main lightning body (triangles forming zigzag)
			glColor3f(primaryColor[0] * alpha, primaryColor[1] * alpha, primaryColor[2] * alpha);
			float boltSize = size * 0.4f;
			
			// Top part of lightning
			glBegin(GL_TRIANGLES);
			glVertex2f(0.0f, boltSize);
			glVertex2f(-boltSize * 0.3f, boltSize * 0.2f);
			glVertex2f(boltSize * 0.2f, boltSize * 0.2f);
			glEnd();

			// Middle part
			glBegin(GL_TRIANGLES);
			glVertex2f(boltSize * 0.1f, boltSize * 0.2f);
			glVertex2f(-boltSize * 0.2f, 0.0f);
			glVertex2f(boltSize * 0.3f, 0.0f);
			glEnd();

			// Bottom part
			glBegin(GL_TRIANGLES);
			glVertex2f(boltSize * 0.2f, 0.0f);
			glVertex2f(-boltSize * 0.1f, -boltSize * 0.2f);
			glVertex2f(boltSize * 0.4f, -boltSize);
			glEnd();

			// 2. Energy ring (circle)
			glColor3f(secondaryColor[0] * alpha * 0.6f, secondaryColor[1] * alpha * 0.6f, secondaryColor[2] * alpha * 0.6f);
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(0.0f, 0.0f);
			for (int i = 0; i <= 16; i++) {
				float angle = 2.0f * M_PI * i / 16;
				glVertex2f(cos(angle) * size * 0.6f, sin(angle) * size * 0.6f);
			}
			glEnd();

			// 3. Speed lines (quads)
			glColor3f(accentColor[0] * alpha * 0.8f, accentColor[1] * alpha * 0.8f, accentColor[2] * alpha * 0.8f);
			float lineWidth = size * 0.05f;
			for (int i = 0; i < 4; i++) {
				float angle = i * 90.0f;
				glPushMatrix();
				glRotatef(angle, 0.0f, 0.0f, 1.0f);
				glBegin(GL_QUADS);
				glVertex2f(-lineWidth, size * 0.7f);
				glVertex2f(lineWidth, size * 0.7f);
				glVertex2f(lineWidth, size * 0.9f);
				glVertex2f(-lineWidth, size * 0.9f);
				glEnd();
				glPopMatrix();
			}
		}
		break;

	case PowerUpType::SHIELD:
		{
			// Shield design - 3 primitives
			// 1. Shield outline (hexagon using triangles)
			glColor3f(primaryColor[0] * alpha, primaryColor[1] * alpha, primaryColor[2] * alpha);
			float shieldRadius = size * 0.5f;
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(0.0f, 0.0f);
			for (int i = 0; i <= 6; i++) {
				float angle = 2.0f * M_PI * i / 6;
				glVertex2f(cos(angle) * shieldRadius, sin(angle) * shieldRadius);
			}
			glEnd();

			// 2. Inner shield (smaller hexagon)
			glColor3f(secondaryColor[0] * alpha * 0.7f, secondaryColor[1] * alpha * 0.7f, secondaryColor[2] * alpha * 0.7f);
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(0.0f, 0.0f);
			for (int i = 0; i <= 6; i++) {
				float angle = 2.0f * M_PI * i / 6;
				glVertex2f(cos(angle) * shieldRadius * 0.6f, sin(angle) * shieldRadius * 0.6f);
			}
			glEnd();

			// 3. Cross pattern (quads)
			glColor3f(accentColor[0] * alpha, accentColor[1] * alpha, accentColor[2] * alpha);
			float crossWidth = size * 0.08f;
			float crossLength = size * 0.4f;
			
			// Vertical cross
			glBegin(GL_QUADS);
			glVertex2f(-crossWidth, -crossLength);
			glVertex2f(crossWidth, -crossLength);
			glVertex2f(crossWidth, crossLength);
			glVertex2f(-crossWidth, crossLength);
			glEnd();

			// Horizontal cross
			glBegin(GL_QUADS);
			glVertex2f(-crossLength, -crossWidth);
			glVertex2f(crossLength, -crossWidth);
			glVertex2f(crossLength, crossWidth);
			glVertex2f(-crossLength, crossWidth);
			glEnd();
		}
		break;
	}

	glPopMatrix();
}

void PowerUp::renderActiveEffect() {
	if (!isActive) return;

	// Visual cue when power-up is active - glowing ring around player
	glPushMatrix();
	
	float effectRadius = 40.0f + 10.0f * sin(animationTime * 6.0f);
	float effectAlpha = 0.3f + 0.2f * sin(animationTime * 8.0f);

	glColor3f(primaryColor[0] * effectAlpha, primaryColor[1] * effectAlpha, primaryColor[2] * effectAlpha);
	
	// Outer ring
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.0f, 0.0f);
	for (int i = 0; i <= 20; i++) {
		float angle = 2.0f * M_PI * i / 20;
		glVertex2f(cos(angle) * effectRadius, sin(angle) * effectRadius);
	}
	glEnd();

	// Inner ring (to create hollow effect)
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.0f, 0.0f);
	for (int i = 0; i <= 20; i++) {
		float angle = 2.0f * M_PI * i / 20;
		glVertex2f(cos(angle) * (effectRadius - 5.0f), sin(angle) * (effectRadius - 5.0f));
	}
	glEnd();

	glPopMatrix();
}

void PowerUp::collect() {
	isVisible = false;
	activate();
}

void PowerUp::activate() {
	isActive = true;
	duration = 8.0f;  
}

void PowerUp::deactivate() {
	isActive = false;
}

void PowerUp::remove() {
	isVisible = false;
}

bool PowerUp::isColliding(float objX, float objY, float objRadius) const {
	if (!isVisible) return false;

	float dx = x - objX;
	float dy = y - objY;
	float distance = sqrt(dx * dx + dy * dy);
	return distance < (size * 0.5f + objRadius);
}

void PowerUp::setPosition(float newX, float newY) {
	x = newX;
	y = newY;
}
#include "Lava.h"
#include <cmath>

Lava::Lava(float screenWidth, float startY, float initialHeight)
	: x(0.0f), y(startY), width(screenWidth), height(initialHeight),
	maxHeight(600.0f), growthRate(0.0f),
	animationTime(0.0f), bubbleOffset1(0.0f), bubbleOffset2(0.0f),
	bubbleOffset3(0.0f)
{
	// Lava color (bright orange-red)
	lavaColor[0] = 1.0f;
	lavaColor[1] = 0.3f;
	lavaColor[2] = 0.0f;

	// Bubble color (darker red)
	bubbleColor[0] = 0.8f;
	bubbleColor[1] = 0.1f;
	bubbleColor[2] = 0.0f;
}

void Lava::update(float deltaTime) {
	height += growthRate * deltaTime;

	animationTime += deltaTime;
	bubbleOffset1 = sin(animationTime * 2.0f) * 4.0f;
	bubbleOffset2 = sin(animationTime * 2.5f + 1.0f) * 3.5f;
	bubbleOffset3 = sin(animationTime * 3.0f + 2.0f) * 4.5f;
}

void Lava::render() {
	glPushMatrix();

	// Draw main lava body (quad) - spans full screen width
	glColor3fv(lavaColor);
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();

	// Draw animated bubbles across the lava surface
	glColor3fv(bubbleColor);
	float bubbleRadius = 5.0f;
	float bubbleY = y + height - 10.0f;

	// Bubble 1 (left side)
	float bubble1X = width * 0.2f;
	float bubble1Y = bubbleY + bubbleOffset1;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(bubble1X, bubble1Y);
	for (int i = 0; i <= 12; i++) {
		float angle = 2.0f * 3.1415 * i / 12;
		glVertex2f(bubble1X + cos(angle) * bubbleRadius, bubble1Y + sin(angle) * bubbleRadius);
	}
	glEnd();

	// Bubble 2 (left-center)
	float bubble2X = width * 0.4f;
	float bubble2Y = bubbleY + bubbleOffset2;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(bubble2X, bubble2Y);
	for (int i = 0; i <= 12; i++) {
		float angle = 2.0f * 3.1415 * i / 12;
		glVertex2f(bubble2X + cos(angle) * bubbleRadius * 0.9f, bubble2Y + sin(angle) * bubbleRadius * 0.9f);
	}
	glEnd();

	// Bubble 3 (right-center)
	float bubble3X = width * 0.65f;
	float bubble3Y = bubbleY + bubbleOffset3;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(bubble3X, bubble3Y);
	for (int i = 0; i <= 12; i++) {
		float angle = 2.0f * 3.1415 * i / 12;
		glVertex2f(bubble3X + cos(angle) * bubbleRadius * 1.1f, bubble3Y + sin(angle) * bubbleRadius * 1.1f);
	}
	glEnd();

	glPopMatrix();
}

void Lava::startGrowing() {
	growthRate = 10.0f;
}

void Lava::setGrowthRate(float rate) {
	growthRate = rate;
}

bool Lava::isTouching(float objY) {

	float lavaTop = y + height;

	bool verticalOverlap = objY <= lavaTop;

	return verticalOverlap;
}

void Lava::setPosition(float newX, float newY) {
	x = newX;
	y = newY;
}

void Lava::setHeight(float newHeight) {
	height = newHeight;
}

void Lava::setMaxHeight(float maxH){
	maxHeight = maxH;
}


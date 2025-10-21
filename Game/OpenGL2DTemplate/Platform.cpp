#include "Platform.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Platform::Platform(float startX, float startY, float platformWidth, float platformHeight)
	: x(startX), y(startY), width(platformWidth), height(platformHeight),
	animationTime(0.0f), bobOffset(0.0f), isVisible(true)
{
	// Base platform color (stone gray)
	baseColor[0] = 0.6f; baseColor[1] = 0.6f; baseColor[2] = 0.6f;

	// Edge color (darker gray)
	edgeColor[0] = 0.4f; edgeColor[1] = 0.4f; edgeColor[2] = 0.4f;

	// Decoration color (lighter gray)
	decorColor[0] = 0.8f; decorColor[1] = 0.8f; decorColor[2] = 0.8f;
}

void Platform::update(float deltaTime) {
	if (!isVisible) return;

	animationTime += deltaTime;

	// Subtle floating animation
	bobOffset = sin(animationTime * 1.5f) * 1.5f;
}

void Platform::render() {
	if (!isVisible) return;

	glPushMatrix();
	glTranslatef(x, y + bobOffset, 0.0f);

	// 1. Main platform body (quad/rectangle)
	glColor3fv(baseColor);
	glBegin(GL_QUADS);
	glVertex2f(-width / 2, 0.0f);
	glVertex2f(width / 2, 0.0f);
	glVertex2f(width / 2, height);
	glVertex2f(-width / 2, height);
	glEnd();

	// 2. Platform edges/borders (quads for depth)
	glColor3fv(edgeColor);
	float edgeThickness = 3.0f;

	// Top edge
	glBegin(GL_QUADS);
	glVertex2f(-width / 2, height);
	glVertex2f(width / 2, height);
	glVertex2f(width / 2, height + edgeThickness);
	glVertex2f(-width / 2, height + edgeThickness);
	glEnd();

	// Left edge
	glBegin(GL_QUADS);
	glVertex2f(-width / 2 - edgeThickness, 0.0f);
	glVertex2f(-width / 2, 0.0f);
	glVertex2f(-width / 2, height + edgeThickness);
	glVertex2f(-width / 2 - edgeThickness, height + edgeThickness);
	glEnd();

	// Right edge
	glBegin(GL_QUADS);
	glVertex2f(width / 2, 0.0f);
	glVertex2f(width / 2 + edgeThickness, 0.0f);
	glVertex2f(width / 2 + edgeThickness, height + edgeThickness);
	glVertex2f(width / 2, height + edgeThickness);
	glEnd();

	// 3. Decorative elements (triangular supports)
	glColor3fv(decorColor);
	float supportSize = height * 0.6f;
	float supportSpacing = width / 4.0f;

	// Left support triangle
	glBegin(GL_TRIANGLES);
	glVertex2f(-supportSpacing, 0.0f);
	glVertex2f(-supportSpacing - supportSize / 2, 0.0f);
	glVertex2f(-supportSpacing, supportSize);
	glEnd();

	// Center support triangle
	glBegin(GL_TRIANGLES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(-supportSize / 3, 0.0f);
	glVertex2f(0.0f, supportSize * 0.8f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(supportSize / 3, 0.0f);
	glVertex2f(0.0f, supportSize * 0.8f);
	glEnd();

	// Right support triangle
	glBegin(GL_TRIANGLES);
	glVertex2f(supportSpacing, 0.0f);
	glVertex2f(supportSpacing + supportSize / 2, 0.0f);
	glVertex2f(supportSpacing, supportSize);
	glEnd();

	// 4. Surface texture/pattern (small quads)
	glColor3f(decorColor[0] * 0.9f, decorColor[1] * 0.9f, decorColor[2] * 0.9f);
	float tileSize = 8.0f;
	int tilesX = (int)(width / tileSize);
	
	for (int i = 0; i < tilesX; i++) {
		float tileX = -width / 2 + i * tileSize + tileSize / 2;
		float tileY = height - 2.0f;
		
		if (i % 2 == 0) {
			glBegin(GL_QUADS);
			glVertex2f(tileX - tileSize / 3, tileY);
			glVertex2f(tileX + tileSize / 3, tileY);
			glVertex2f(tileX + tileSize / 3, tileY + 3.0f);
			glVertex2f(tileX - tileSize / 3, tileY + 3.0f);
			glEnd();
		}
	}

	glPopMatrix();
}

void Platform::hide() {
	isVisible = false;
}

void Platform::show() {
	isVisible = true;
}

bool Platform::isPlayerOnTop(float playerX, float playerY, float playerWidth, float playerHeight) const {
	if (!isVisible) return false;

	float platformTop = y + height + bobOffset;
	float platformLeft = x - width / 2;
	float platformRight = x + width / 2;
	
	float playerLeft = playerX - playerWidth / 2;
	float playerRight = playerX + playerWidth / 2;
	float playerBottom = playerY;

	// Check if player is horizontally within platform bounds
	bool horizontalOverlap = playerRight > platformLeft && playerLeft < platformRight;
	
	// Check if player is standing on top (within a small tolerance)
	bool onTop = playerBottom >= platformTop - 5.0f && playerBottom <= platformTop + 5.0f;

	return horizontalOverlap && onTop;
}

bool Platform::isPlayerColliding(float playerX, float playerY, float playerWidth, float playerHeight) const {
	if (!isVisible) return false;

	float platformTop = y + height + bobOffset;
	float platformBottom = y + bobOffset;
	float platformLeft = x - width / 2;
	float platformRight = x + width / 2;
	
	float playerLeft = playerX - playerWidth / 2;
	float playerRight = playerX + playerWidth / 2;
	float playerTop = playerY + playerHeight;
	float playerBottom = playerY;

	bool horizontalOverlap = playerRight > platformLeft && playerLeft < platformRight;
	bool verticalOverlap = playerTop > platformBottom && playerBottom < platformTop;

	return horizontalOverlap && verticalOverlap;
}

void Platform::setPosition(float newX, float newY) {
	x = newX;
	y = newY;
}
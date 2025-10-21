#include "HUD.h"
#include <string>
#include <cmath>


HUD::HUD(float screenW, float screenH)
	: screenWidth(screenW), screenHeight(screenH),
	maxLives(3), currentLives(3),
	currentLavaHeight(0.0f), maxLavaHeight(600.0f),
	score(0)
{
	// Heart position (top-left corner)
	heartX = 30.0f;
	heartY = screenHeight - 40.0f;
	heartSize = 15.0f;

	// Heart colors
	heartColor[0] = 1.0f; heartColor[1] = 0.0f; heartColor[2] = 0.0f; 
	emptyHeartColor[0] = 0.3f; emptyHeartColor[1] = 0.3f; emptyHeartColor[2] = 0.3f;  

	// Lava bar position (top-right corner)
	lavaBarX = screenWidth - 220.0f;
	lavaBarY = screenHeight - 40.0f;
	lavaBarWidth = 200.0f;
	lavaBarHeight = 25.0f;

	// Lava colors
	lavaColor[0] = 1.0f; lavaColor[1] = 0.3f; lavaColor[2] = 0.0f;  
	lavaBgColor[0] = 0.3f; lavaBgColor[1] = 0.3f; lavaBgColor[2] = 0.3f;  

	// Score position (top-center)
	scoreX = screenWidth / 2 - 40.0f;
	scoreY = screenHeight - 40.0f;
}

void HUD::render() {

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, screenWidth, 0, screenHeight);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	// === HEALTH HUD (3 hearts) ===
	float heartSpacing = heartSize * 2.5f;
	for (int i = 0; i < maxLives; i++) {
		float x = heartX + i * heartSpacing;
		bool filled = i < currentLives;
		renderHeart(x, heartY, heartSize, filled);
	}

	
	// 1. Lava bar background (quad)
	glColor3fv(lavaBgColor);
	glBegin(GL_QUADS);
	glVertex2f(lavaBarX, lavaBarY);
	glVertex2f(lavaBarX + lavaBarWidth, lavaBarY);
	glVertex2f(lavaBarX + lavaBarWidth, lavaBarY + lavaBarHeight);
	glVertex2f(lavaBarX, lavaBarY + lavaBarHeight);
	glEnd();

	// 2. Lava bar fill (quad) - proportional to current lava height
	float lavaPercent = currentLavaHeight / maxLavaHeight;
	if (lavaPercent > 1.0f) lavaPercent = 1.0f;
	float lavaFillWidth = lavaBarWidth * lavaPercent;
	
	glColor3fv(lavaColor);
	glBegin(GL_QUADS);
	glVertex2f(lavaBarX, lavaBarY);
	glVertex2f(lavaBarX + lavaFillWidth, lavaBarY);
	glVertex2f(lavaBarX + lavaFillWidth, lavaBarY + lavaBarHeight);
	glVertex2f(lavaBarX, lavaBarY + lavaBarHeight);
	glEnd();

	// Lava bar outline (line loop)
	glColor3f(0.8f, 0.8f, 0.8f);
	glLineWidth(2.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(lavaBarX, lavaBarY);
	glVertex2f(lavaBarX + lavaBarWidth, lavaBarY);
	glVertex2f(lavaBarX + lavaBarWidth, lavaBarY + lavaBarHeight);
	glVertex2f(lavaBarX, lavaBarY + lavaBarHeight);
	glEnd();

	// Lava label
	glColor3f(1.0f, 1.0f, 1.0f);
	renderText("LAVA", lavaBarX + 5.0f, lavaBarY + 8.0f);

	// === SCORE (number displayed on screen) ===
	glColor3f(1.0f, 1.0f, 1.0f);
	renderText("SCORE:", scoreX - 50.0f, scoreY + 8.0f);
	renderNumber(score, scoreX + 10.0f, scoreY + 8.0f);

	// Re-enable depth test
	glEnable(GL_DEPTH_TEST);

	// Restore previous state
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void HUD::renderHeart(float x, float y, float size, bool filled) {
	// Heart made from 2 circles (top) and 1 triangle (bottom)
	
	if (filled) {
		glColor3fv(heartColor);  // Red for filled hearts
	} else {
		glColor3fv(emptyHeartColor);  // Gray for empty hearts
	}

	float circleRadius = size * 0.5f;
	float circleOffset = size * 0.35f;

	// Left circle (triangle fan)
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x - circleOffset, y + size * 0.3f);
	for (int i = 0; i <= 20; i++) {
		float angle = 2.0f * 3.1415 * i / 20;
		glVertex2f(x - circleOffset + cos(angle) * circleRadius, 
		          y + size * 0.3f + sin(angle) * circleRadius);
	}
	glEnd();

	// Right circle (triangle fan)
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x + circleOffset, y + size * 0.3f);
	for (int i = 0; i <= 20; i++) {
		float angle = 2.0f * 3.1415 * i / 20;
		glVertex2f(x + circleOffset + cos(angle) * circleRadius, 
		          y + size * 0.3f + sin(angle) * circleRadius);
	}
	glEnd();

	// Bottom triangle (pointing down)
	glBegin(GL_TRIANGLES);
	glVertex2f(x - size * 0.85f, y + size * 0.3f);
	glVertex2f(x + size * 0.85f, y + size * 0.3f);
	glVertex2f(x, y - size * 0.7f);
	glEnd();

	// Draw outline for empty hearts
	if (!filled) {
		glColor3f(0.6f, 0.6f, 0.6f);
		glLineWidth(2.0f);
		
		// Outline around left circle
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i <= 20; i++) {
			float angle = 2.0f * 3.1415 * i / 20;
			glVertex2f(x - circleOffset + cos(angle) * circleRadius, 
			          y + size * 0.3f + sin(angle) * circleRadius);
		}
		glEnd();
		
		// Outline around right circle
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i <= 20; i++) {
			float angle = 2.0f * 3.1415 * i / 20;
			glVertex2f(x + circleOffset + cos(angle) * circleRadius, 
			          y + size * 0.3f + sin(angle) * circleRadius);
		}
		glEnd();
		
		// Outline for triangle
		glBegin(GL_LINE_LOOP);
		glVertex2f(x - size * 0.85f, y + size * 0.3f);
		glVertex2f(x + size * 0.85f, y + size * 0.3f);
		glVertex2f(x, y - size * 0.7f);
		glEnd();
	}
}

void HUD::setLives(int lives) {
	currentLives = lives;
	if (currentLives < 0) currentLives = 0;
	if (currentLives > maxLives) currentLives = maxLives;
}

void HUD::loseLife() {
	if (currentLives > 0) {
		currentLives--;
	}
}

void HUD::setLavaHeight(float lavaHeight) {
	currentLavaHeight = lavaHeight;
}

void HUD::setMaxLavaHeight(float maxHeight) {
	maxLavaHeight = maxHeight;
}

void HUD::setScore(int newScore) {
	score = newScore;
}

void HUD::addScore(int points) {
	score += points;
}

void HUD::renderText(const std::string& text, float x, float y) {
	glRasterPos2f(x, y);
	for (char c : text) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
	}
}

void HUD::renderNumber(int number, float x, float y) {
	std::string numStr = std::to_string(number);
	glRasterPos2f(x, y);
	for (char c : numStr) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}
}
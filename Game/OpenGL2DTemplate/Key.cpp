#include "Key.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Key::Key(float startX, float startY, float size)
	: x(startX), y(startY), keySize(size),
	rotationAngle(0.0f), floatAnimation(0.0f), scaleAnimation(1.0f),
	animationTime(0.0f), isVisible(true)
{
	// Key color (golden)
	keyColor[0] = 1.0f; keyColor[1] = 0.84f; keyColor[2] = 0.0f;

	// Shine color (bright yellow)
	shineColor[0] = 1.0f; shineColor[1] = 1.0f; shineColor[2] = 0.7f;

	// Shadow color (dark orange)
	shadowColor[0] = 0.8f; shadowColor[1] = 0.5f; shadowColor[2] = 0.0f;
}

void Key::update(float deltaTime) {
	if (!isVisible) return;

	animationTime += deltaTime;

	// Continuous rotation
	rotationAngle += 60.0f * deltaTime;  // 60 degrees per second
	if (rotationAngle >= 360.0f) {
		rotationAngle -= 360.0f;
	}

	// Floating up and down
	floatAnimation = sin(animationTime * 2.0f) * 8.0f;

	// Gentle scaling
	scaleAnimation = 1.0f + 0.1f * sin(animationTime * 4.0f);
}

void Key::render() {
	if (!isVisible) return;

	glPushMatrix();
	glTranslatef(x, y + floatAnimation, 0.0f);
	glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
	glScalef(scaleAnimation, scaleAnimation, 1.0f);

	// 1. Key shaft (rectangle/quad)
	glColor3fv(shadowColor);
	float shaftWidth = keySize * 0.15f;
	float shaftLength = keySize * 0.6f;
	glBegin(GL_QUADS);
	glVertex2f(-shaftWidth / 2, -shaftLength);
	glVertex2f(shaftWidth / 2, -shaftLength);
	glVertex2f(shaftWidth / 2, 0.0f);
	glVertex2f(-shaftWidth / 2, 0.0f);
	glEnd();

	// Key shaft highlight
	glColor3fv(keyColor);
	glBegin(GL_QUADS);
	glVertex2f(-shaftWidth / 2, -shaftLength + 2.0f);
	glVertex2f(shaftWidth / 2 - 1.0f, -shaftLength + 2.0f);
	glVertex2f(shaftWidth / 2 - 1.0f, -2.0f);
	glVertex2f(-shaftWidth / 2, -2.0f);
	glEnd();

	// 2. Key head (circle - triangle fan)
	glColor3fv(shadowColor);
	float headRadius = keySize * 0.25f;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.0f, 0.0f);
	for (int i = 0; i <= 20; i++) {
		float angle = 2.0f * M_PI * i / 20;
		glVertex2f(cos(angle) * headRadius, sin(angle) * headRadius);
	}
	glEnd();

	// Key head highlight
	glColor3fv(keyColor);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(-1.5f, 1.5f);
	for (int i = 0; i <= 20; i++) {
		float angle = 2.0f * M_PI * i / 20;
		glVertex2f(-1.5f + cos(angle) * (headRadius - 2.0f), 
		          1.5f + sin(angle) * (headRadius - 2.0f));
	}
	glEnd();

	// 3. Key teeth (triangles)
	glColor3fv(shadowColor);
	float toothSize = keySize * 0.08f;
	
	// First tooth
	glBegin(GL_TRIANGLES);
	glVertex2f(shaftWidth / 2, -shaftLength * 0.3f);
	glVertex2f(shaftWidth / 2 + toothSize, -shaftLength * 0.3f + toothSize);
	glVertex2f(shaftWidth / 2 + toothSize, -shaftLength * 0.3f - toothSize);
	glEnd();

	// Second tooth
	glBegin(GL_TRIANGLES);
	glVertex2f(shaftWidth / 2, -shaftLength * 0.6f);
	glVertex2f(shaftWidth / 2 + toothSize * 0.7f, -shaftLength * 0.6f + toothSize * 0.7f);
	glVertex2f(shaftWidth / 2 + toothSize * 0.7f, -shaftLength * 0.6f - toothSize * 0.7f);
	glEnd();

	// 4. Shine effect (small triangles)
	glColor3fv(shineColor);
	float shineSize = keySize * 0.06f;
	
	// Shine on head
	glBegin(GL_TRIANGLES);
	glVertex2f(-headRadius * 0.4f, headRadius * 0.4f);
	glVertex2f(-headRadius * 0.4f - shineSize, headRadius * 0.4f - shineSize);
	glVertex2f(-headRadius * 0.4f + shineSize, headRadius * 0.4f - shineSize);
	glEnd();

	// Shine on shaft
	glBegin(GL_TRIANGLES);
	glVertex2f(0.0f, -shaftLength * 0.5f);
	glVertex2f(-shineSize * 0.5f, -shaftLength * 0.5f - shineSize);
	glVertex2f(shineSize * 0.5f, -shaftLength * 0.5f - shineSize);
	glEnd();

	glPopMatrix();
}

void Key::collect() {
	isVisible = false;
}

bool Key::isColliding(float objX, float objY, float objRadius) const {
	if (!isVisible) return false;

	float dx = x - objX;
	float dy = (y + floatAnimation) - objY;
	float distance = sqrt(dx * dx + dy * dy);
	return distance < (keySize * 0.4f + objRadius);
}

void Key::setPosition(float newX, float newY) {
	x = newX;
	y = newY;
}
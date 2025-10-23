#include "Player.h"
#include <cmath>

Player::Player(float startX, float startY)
	: x(startX), y(startY), vx(0), vy(0),
	headRadius(15.0f), torsoWidth(20.0f), torsoHeight(30.0f),
	armWidth(5.0f), armLength(20.0f), legWidth(5.0f), legLength(25.0f),
	capSize(10.0f), jumpOffset(0.0f), isJumping(false
{
	headColor[0] = 1.0f; headColor[1] = 0.85f; headColor[2] = 0.7f;
	torsoColor[0] = 0.2f; torsoColor[1] = 0.4f; torsoColor[2] = 0.8f;
	legColor[0] = 0.9f; legColor[1] = 0.75f; legColor[2] = 0.6f;
	armColor[0] = 0.9f; armColor[1] = 0.75f; armColor[2] = 0.6f;
	capColor[0] = 1.0f; capColor[1] = 0.0f; capColor[2] = 0.0f;
}

void Player::update(float deltaTime) {
	x += vx * deltaTime;
	y += vy * deltaTime;

	if (vy > 0) {
		isJumping = true;
		jumpOffset = 3.0f;
	}
	else if (vy < 0) {
		jumpOffset = 0.0f;
	}
	else {
		isJumping = false;
		jumpOffset = 0.0f;
	}
}

void Player::render() {
	glPushMatrix();
	glTranslatef(x, y + jumpOffset, 0.0f);

	float bodyBottom = 0.0f;
	float legTop = bodyBottom;
	float torsoBottom = legTop + legLength;
	float torsoTop = torsoBottom + torsoHeight;
	float headBottom = torsoTop;
	float headCenter = headBottom + headRadius;
	float capBottom = headCenter + headRadius * 0.5f;

	// Draw Legs
	glColor3fv(legColor);
	float legOffset = torsoWidth * 0.25f;

	// Left leg
	glBegin(GL_QUADS);
	glVertex2f(-legOffset - legWidth / 2, legTop);
	glVertex2f(-legOffset + legWidth / 2, legTop);
	glVertex2f(-legOffset + legWidth / 2, legTop + legLength);
	glVertex2f(-legOffset - legWidth / 2, legTop + legLength);
	glEnd();

	// Right leg
	glBegin(GL_QUADS);
	glVertex2f(legOffset - legWidth / 2, legTop);
	glVertex2f(legOffset + legWidth / 2, legTop);
	glVertex2f(legOffset + legWidth / 2, legTop + legLength);
	glVertex2f(legOffset - legWidth / 2, legTop + legLength);
	glEnd();

	// Draw torso
	glColor3fv(torsoColor);
	glBegin(GL_QUADS);
	glVertex2f(-torsoWidth / 2, torsoBottom);
	glVertex2f(torsoWidth / 2, torsoBottom);
	glVertex2f(torsoWidth / 2, torsoTop);
	glVertex2f(-torsoWidth / 2, torsoTop);
	glEnd();

	// Draw arms 
	glColor3fv(armColor);
	float armOffset = torsoWidth / 2 + armWidth / 2;
	float armY = torsoTop - 5.0f;

	// Left arm
	glBegin(GL_QUADS);
	glVertex2f(-armOffset - armWidth / 2, armY);
	glVertex2f(-armOffset + armWidth / 2, armY);
	glVertex2f(-armOffset + armWidth / 2, armY - armLength);
	glVertex2f(-armOffset - armWidth / 2, armY - armLength);
	glEnd();

	// Right arm
	glBegin(GL_QUADS);
	glVertex2f(armOffset - armWidth / 2, armY);
	glVertex2f(armOffset + armWidth / 2, armY);
	glVertex2f(armOffset + armWidth / 2, armY - armLength);
	glVertex2f(armOffset - armWidth / 2, armY - armLength);
	glEnd();

	// Draw head 
	glColor3fv(headColor);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.0f, headCenter);  
	for (int i = 0; i <= 20; i++) {
		float angle = 2.0f * 3.14159 * i / 20;
		glVertex2f(cos(angle) * headRadius, headCenter + sin(angle) * headRadius);
	}
	glEnd();

	// Draw eyes (points)
	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(4.0f);
	float eyeOffset = headRadius * 0.35f;
	float eyeY = headCenter + headRadius * 0.2f;

	glBegin(GL_POINTS);
	glVertex2f(-eyeOffset, eyeY);  // Left eye
	glVertex2f(eyeOffset, eyeY);   // Right eye
	glEnd();

	// Draw cap 
	glColor3fv(capColor);
	glBegin(GL_TRIANGLES);
	glVertex2f(-capSize / 2, capBottom);
	glVertex2f(capSize / 2, capBottom);
	glVertex2f(0.0f, capBottom + capSize);
	glEnd();

	glPopMatrix();

}

void Player::moveLeft() {
	vx = -200.0f;
}

void Player::moveRight() {
	vx = 200.0f;
}

void Player::jump() {
	if (!isJumping) {
		vy = 300.0f;
	}
}

void Player::stopHorizontalMovement() {
	vx = 0.0f;
}

void Player::setPosition(float newX, float newY) {
	x = newX;
	y = newY;
}

void Player::setVelocity(float newVX, float newVY) {
	vx = newVX;
	vy = newVY;
}

void Player::setGrounded(bool grounded) {
	if (grounded) {
		isJumping = false;
		jumpOffset = 0.0f;
	}
}
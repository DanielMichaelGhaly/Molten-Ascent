#include "Door.h"
#include <cmath>

Door::Door(float startX, float startY, float width, float height)
	: x(startX), y(startY), doorWidth(width), doorHeight(height),
	handleRadius(4.0f), keyHoleSize(3.0f), isOpen(false)
{
	// Closed door color (brown wood)
	doorColor[0] = 0.55f;
	doorColor[1] = 0.27f;
	doorColor[2] = 0.07f;

	// Handle color (gold/brass)
	handleColor[0] = 0.85f;
	handleColor[1] = 0.65f;
	handleColor[2] = 0.13f;

	// Key hole color (dark)
	keyHoleColor[0] = 0.1f;
	keyHoleColor[1] = 0.1f;
	keyHoleColor[2] = 0.1f;

	// Open door color (lighter brown)
	openDoorColor[0] = 0.65f;
	openDoorColor[1] = 0.4f;
	openDoorColor[2] = 0.2f;
}

void Door::render() {
	glPushMatrix();
	glTranslatef(x, y, 0.0f);

	if (!isOpen)
	{
		// 1. Main door body (quad)
		glColor3fv(doorColor);
		glBegin(GL_QUADS);
		glVertex2f(-doorWidth / 2, 0.0f);
		glVertex2f(doorWidth / 2, 0.0f);
		glVertex2f(doorWidth / 2, doorHeight);
		glVertex2f(-doorWidth / 2, doorHeight);
		glEnd();

		// 2. Door handle (circle - triangle fan)
		glColor3fv(handleColor);
		float handleX = doorWidth * 0.3f;
		float handleY = doorHeight * 0.5f;
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(handleX, handleY);
		for (int i = 0; i <= 16; i++) {
			float angle = 2.0f * 3.1415 * i / 16;
			glVertex2f(handleX + cos(angle) * handleRadius,
				handleY + sin(angle) * handleRadius);
		}
		glEnd();

		// 3. Key lock (triangle pointing down)
		glColor3fv(keyHoleColor);
		float lockX = handleX;
		float lockY = handleY - handleRadius - 8.0f;
		glBegin(GL_TRIANGLES);
		glVertex2f(lockX, lockY);
		glVertex2f(lockX - keyHoleSize, lockY + keyHoleSize);
		glVertex2f(lockX + keyHoleSize, lockY + keyHoleSize);
		glEnd();

		// Key lock hole (small circle on top of triangle)
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(lockX, lockY + keyHoleSize);
		for (int i = 0; i <= 12; i++) {
			float angle = 2.0f * 3.1415 * i / 12;
			glVertex2f(lockX + cos(angle) * keyHoleSize * 0.6f,
				lockY + keyHoleSize + sin(angle) * keyHoleSize * 0.6f);
		}
		glEnd();
	}
	else {
		// 1. Door frame (quad outline) - what remains visible
		glColor3f(0.4f, 0.2f, 0.05f);
		float frameThickness = 5.0f;

		// Left frame
		glBegin(GL_QUADS);
		glVertex2f(-doorWidth / 2 - frameThickness, 0.0f);
		glVertex2f(-doorWidth / 2, 0.0f);
		glVertex2f(-doorWidth / 2, doorHeight);
		glVertex2f(-doorWidth / 2 - frameThickness, doorHeight);
		glEnd();

		// Right frame
		glBegin(GL_QUADS);
		glVertex2f(doorWidth / 2, 0.0f);
		glVertex2f(doorWidth / 2 + frameThickness, 0.0f);
		glVertex2f(doorWidth / 2 + frameThickness, doorHeight);
		glVertex2f(doorWidth / 2, doorHeight);
		glEnd();

		// Top frame
		glBegin(GL_QUADS);
		glVertex2f(-doorWidth / 2 - frameThickness, doorHeight);
		glVertex2f(doorWidth / 2 + frameThickness, doorHeight);
		glVertex2f(doorWidth / 2 + frameThickness, doorHeight + frameThickness);
		glVertex2f(-doorWidth / 2 - frameThickness, doorHeight + frameThickness);
		glEnd();

		// 2. Opened door (triangle) - door swung to the side
		glColor3fv(openDoorColor);
		float openDoorOffset = doorWidth * 0.8f;
		glBegin(GL_TRIANGLES);
		glVertex2f(doorWidth / 2, 0.0f);
		glVertex2f(doorWidth / 2 + openDoorOffset, doorHeight * 0.3f);
		glVertex2f(doorWidth / 2, doorHeight);
		glEnd();

		// 3. Door handle on opened door (circle)
		glColor3fv(handleColor);
		float openHandleX = doorWidth / 2 + openDoorOffset * 0.6f;
		float openHandleY = doorHeight * 0.5f;
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(openHandleX, openHandleY);
		for (int i = 0; i <= 16; i++) {
			float angle = 2.0f * 3.1415 * i / 16;
			glVertex2f(openHandleX + cos(angle) * handleRadius,
				openHandleY + sin(angle) * handleRadius);
		}
		glEnd();
	}

	glPopMatrix();
}

void Door::open() {
	isOpen = true;
}

void Door::setPosition(float newX, float newY) {
	x = newX;
	y = newY;
}
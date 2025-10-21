#include "Rock.h"

Rock::Rock(float startX, float startY, float width, float height) :
	x(startX), y(startY), baseWidth(width), baseHeight(height), peakHeight(height * 0.3f)
{
	rockColor[0] = 0.5f;
	rockColor[1] = 0.5f;
	rockColor[2] = 0.5f;
}

void Rock::render() {
	glPushMatrix();
	glTranslatef(x, y, 0.0f);

	//Draw base (quad)
	glColor3fv(rockColor);
	glBegin(GL_QUADS);
	glVertex2f(-baseWidth / 2, 0.0f);
	glVertex2f(baseWidth / 2, 0.0f);
	glVertex2f(baseWidth / 2, baseHeight);
	glVertex2f(-baseWidth / 2, baseHeight);
	glEnd();

	// Draw peak (triangle) - top of rock for irregular shape
	glColor3fv(rockColor);  
	glBegin(GL_TRIANGLES);
	glVertex2f(-baseWidth / 2, baseHeight);
	glVertex2f(baseWidth / 2, baseHeight);
	glVertex2f(0.0f, baseHeight + peakHeight);
	glEnd();

	glPopMatrix();
}

void Rock::setPosition(float newX, float newY) {
	x = newX;
	y = newY;
}
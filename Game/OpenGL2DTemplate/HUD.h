#pragma once
#include <glut.h>
#include <string>

class HUD {
private:
	// Screen dimensions
	float screenWidth;
	float screenHeight;

	// Health HUD (hearts)
	float heartX;
	float heartY;
	float heartSize;
	int maxLives;
	int currentLives;
	float heartColor[3];
	float emptyHeartColor[3];

	// Lava HUD
	float lavaBarX;
	float lavaBarY;
	float lavaBarWidth;
	float lavaBarHeight;
	float currentLavaHeight;
	float maxLavaHeight;
	float lavaColor[3];
	float lavaBgColor[3];

	// Score
	int score;
	float scoreX;
	float scoreY;

public:
	HUD(float screenW, float screenH);
	void render();

	void setLives(int lives);
	void loseLife();
	void setLavaHeight(float lavaHeight);
	void setMaxLavaHeight(float maxHeight);
	void setScore(int newScore);
	void addScore(int points);

private:
	void renderHeart(float x, float y, float size, bool filled);
	void renderText(const std::string& text, float x, float y);
	void renderNumber(int number, float x, float y);
};

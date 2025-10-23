#pragma once
#include <vector>
#include <string>
#include "Player.h"
#include "Platform.h"
#include "Rock.h"
#include "Collectable.h"
#include "Key.h"
#include "PowerUp.h"
#include "Lava.h"
#include "Door.h"
#include "HUD.h"
#include "Audio.h"

enum class GameState { Playing, Won, Lost };

enum class Ability { None, Speed, Shield };

class Game {
public:
	Game(int screenW, int screenH);
	void update(float dt);
	void render();

	// Input
	void onKeyDown(unsigned char key);
	void onKeyUp(unsigned char key);
	void onSpecialDown(int key);
	void onSpecialUp(int key);

	// Accessors
	GameState getState() const { return state; }

private:
	int screenW;
	int screenH;

	// Core systems
	Player player;
	Lava lava;
	Door door;
	HUD hud;

	// Entities
	std::vector<Platform> platforms;
	std::vector<Rock> rocks;
	std::vector<Collectable> collectables;
	std::vector<PowerUp> powerups;
	Key key;

	// Timers
	float timeSinceStart;
	float rockSpawnTimer;
	float nextRockSpawn;
	float powerupSpawnTimer;
	float nextPowerupSpawn;
	float lavaSpeed;
	float lavaAccel;

	// State
	int lives;
	int score;
	int collectedCount;
	bool keySpawned;
	bool hasKey;
	GameState state;
	Ability activeAbility;
	float abilityTimeLeft;

	// Input state
	bool leftHeld;
	bool rightHeld;

	// Helpers
	void initLevel();
	void spawnRock();
	void spawnPowerUp();
	bool aabbOverlap(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh);
	bool placeWithoutOverlap(float x, float y, float w, float h);
	void checkCollisions(float dt);
	void handlePlayerMovement(float dt);
	void updateLava(float dt);
	void trySpawnKey();
	void win();
	void lose();
};

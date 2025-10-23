#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

static float frand(float a, float b) { return a + (b - a) * (rand() / (float)RAND_MAX); }

Game::Game(int w, int h)
	: screenW(w), screenH(h),
	player(w * 0.5f, 40.0f),
	lava((float)w, 0.0f, 1.0f),
	door(w * 0.5f, (float)h - 120.0f, 60.0f, 100.0f),
	hud((float)w, (float)h),
	key(w * 0.5f, 200.0f),
	timeSinceStart(0.0f), rockSpawnTimer(0.0f), nextRockSpawn(2.0f), powerupSpawnTimer(0.0f), nextPowerupSpawn(7.0f),
	lives(3), score(0), collectedCount(0), keySpawned(false), hasKey(false), state(GameState::Playing),
	activeAbility(Ability::None), abilityTimeLeft(0.0f), leftHeld(false), rightHeld(false), lavaSpeed(6.0f), lavaAccel(0.3f)
{
	srand((unsigned)time(nullptr));
	initLevel();
	lava.setGrowthRate(lavaSpeed);
	Audio::PlayMusic("assets/music_bg.wav");
}

void Game::initLevel() {
	platforms.clear();
	// at least 3 different sizes; ascending vertical level
	platforms.emplace_back(screenW * 0.5f, 20.0f, 240.0f, 24.0f); // ground
	platforms.emplace_back(screenW * 0.25f, 120.0f, 120.0f, 20.0f);
	platforms.emplace_back(screenW * 0.75f, 220.0f, 160.0f, 20.0f);
	platforms.emplace_back(screenW * 0.35f, 320.0f, 100.0f, 20.0f);
	platforms.emplace_back(screenW * 0.65f, 420.0f, 140.0f, 20.0f);
	platforms.emplace_back(screenW * 0.5f, screenH - 80.0f, 160.0f, 20.0f); // near door

	// collectables placed without overlap
	collectables.clear();
	for (int i = 0; i < 7; ++i) {
		float cx = frand(60.0f, screenW - 60.0f);
		float cy = 80.0f + i * 60.0f;
		collectables.emplace_back(cx, cy);
	}

	powerups.clear();
	rocks.clear();
}

void Game::update(float dt) {
	if (state != GameState::Playing) return;
	timeSinceStart += dt;

	// Input movement
	handlePlayerMovement(dt);

	// Update entities
	for (auto& p : platforms) p.update(dt);
	for (auto& r : rocks) {
		r.setPosition(r.getX(), r.getY() - 120.0f * dt); // falling
	}
	for (auto& c : collectables) c.update(dt);
	for (auto& pu : powerups) pu.update(dt);
	key.update(dt);
	player.update(dt);

	// Update lava speed (accelerate gradually)
	lavaSpeed += lavaAccel * dt;
	lava.setGrowthRate(lavaSpeed);
	updateLava(dt);

	// Spawn rocks randomly
	rockSpawnTimer += dt;
	if (rockSpawnTimer >= nextRockSpawn) {
		spawnRock();
		rockSpawnTimer = 0.0f;
		nextRockSpawn = frand(0.8f, 2.2f);
	}

	// Spawn powerups occasionally (ensure 2 different ones appear during game)
	powerupSpawnTimer += dt;
	if (powerupSpawnTimer >= nextPowerupSpawn) {
		spawnPowerUp();
		powerupSpawnTimer = 0.0f;
		nextPowerupSpawn = frand(8.0f, 14.0f);
	}

	// Collisions and game rules
	checkCollisions(dt);
	trySpawnKey();

	// HUD
	hud.setLives(lives);
	hud.setLavaHeight(lava.getHeight());
	hud.setScore(score);
}

void Game::render() {
	for (auto& p : platforms) p.render();
	for (auto& c : collectables) c.render();
	for (auto& pu : powerups) pu.render();
	if (key.getIsVisible()) key.render();
	for (auto& r : rocks) r.render();
	lava.render();
	player.render();
	door.render();
	hud.render();
}

void Game::handlePlayerMovement(float dt) {
	float speed = (activeAbility == Ability::Speed) ? 260.0f : 200.0f;
	if (leftHeld && !rightHeld) player.setVelocity(-speed, player.getVY());
	else if (rightHeld && !leftHeld) player.setVelocity(speed, player.getVY());
	else player.stopHorizontalMovement();
}

void Game::spawnRock() {
	float x = frand(40.0f, screenW - 40.0f);
	float sizes[3] = { 40.0f, 55.0f, 70.0f };
	float s = sizes[rand() % 3];
	Rock r(x, (float)screenH + 30.0f, s, s * 0.7f);
	rocks.push_back(r);
}

void Game::spawnPowerUp() {
	float x = frand(80.0f, screenW - 80.0f);
	float y = frand(160.0f, (float)screenH - 120.0f);
	PowerUpType t = (rand() % 2 == 0) ? PowerUpType::SPEED_BOOST : PowerUpType::SHIELD; // two types at least once
	powerups.emplace_back(t, x, y);
}

bool Game::aabbOverlap(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh) {
	float aL = ax - aw * 0.5f, aR = ax + aw * 0.5f, aB = ay, aT = ay + ah;
	float bL = bx - bw * 0.5f, bR = bx + bw * 0.5f, bB = by, bT = by + bh;
	return aR > bL && aL < bR && aT > bB && aB < bT;
}

void Game::checkCollisions(float dt) {
	// Player with platforms - grounding
	bool grounded = false;
	for (auto& p : platforms) {
		if (p.isPlayerOnTop(player.getX(), player.getY(), player.getWidth(), player.getHeight())) grounded = true;
	}
	player.setGrounded(grounded);

	// Player with collectables
	for (auto& c : collectables) {
		if (c.getIsVisible() && c.isColliding(player.getX(), player.getY() + player.getHeight() * 0.5f, 12.0f)) {
			c.collect();
			score += 10;
			collectedCount++;
			Audio::PlaySfx("assets/sfx_collect.wav");
		}
	}

	// Player with key
	if (key.getIsVisible() && key.isColliding(player.getX(), player.getY() + player.getHeight() * 0.5f, 12.0f)) {
		key.collect();
		hasKey = true;
		Audio::PlaySfx("assets/sfx_key.wav");
	}

	// Player with powerups
	for (auto& pu : powerups) {
		if (pu.getIsVisible() && pu.isColliding(player.getX(), player.getY() + player.getHeight() * 0.5f, 12.0f)) {
			pu.collect();
			if (pu.getType() == PowerUpType::SPEED_BOOST) { activeAbility = Ability::Speed; abilityTimeLeft = 8.0f; }
			if (pu.getType() == PowerUpType::SHIELD) { activeAbility = Ability::Shield; abilityTimeLeft = 8.0f; }
			Audio::PlaySfx("assets/sfx_powerup.wav");
		}
	}
	if (activeAbility != Ability::None) {
		abilityTimeLeft -= dt;
		if (abilityTimeLeft <= 0.0f) activeAbility = Ability::None;
	}

	// Falling rocks hit player
	for (auto& r : rocks) {
		if (aabbOverlap(player.getX(), player.getY(), player.getWidth(), player.getHeight(), r.getX(), r.getY(), r.getWidth(), r.getHeight())) {
			if (activeAbility != Ability::Shield) {
				if (lives > 0) lives -= 1;
				Audio::PlaySfx("assets/sfx_hit.wav");
				if (lives <= 0) lose();
			}
		}
	}

	// Lava kills player instantly
	if (lava.isTouching(player.getY())) {
		lose();
		Audio::PlaySfx("assets/sfx_lose.wav");
	}

	// Lava removes objects it touches
	for (auto& c : collectables) if (lava.isTouching(c.getY())) c.collect();
	for (auto& pu : powerups) if (lava.isTouching(pu.getY())) pu.remove();
	for (auto& r : rocks) if (lava.isTouching(r.getY())) { /* rock disappears */ }

	// Door unlock when player has key
	if (hasKey) door.unlock();
	door.update(dt);

	// Win if player reaches door top area
	if (hasKey && player.getY() + player.getHeight() > screenH - 120.0f && fabs(player.getX() - door.getX()) < 60.0f) {
		win();
	}
}

void Game::updateLava(float dt) {
	lava.update(dt);
	// Lava HUD is driven by height in update()
}

void Game::trySpawnKey() {
	if (!keySpawned && collectedCount >= 5) {
		// place key above current lava height
		float safeY = std::max(lava.getTopY() + 80.0f, 200.0f);
		key.setPosition(screenW * 0.5f, std::min((float)screenH - 120.0f, safeY));
		keySpawned = true;
	}
}

void Game::win() {
	state = GameState::Won;
	Audio::StopMusic();
	Audio::PlaySfx("assets/sfx_win.wav");
}

void Game::lose() {
	state = GameState::Lost;
	Audio::StopMusic();
	Audio::PlaySfx("assets/sfx_lose.wav");
}

void Game::onKeyDown(unsigned char key) {
	if (state != GameState::Playing) return;
	switch (key) {
	case 'a': case 'A': leftHeld = true; break;
	case 'd': case 'D': rightHeld = true; break;
	case 'w': case 'W': player.jump(); break;
	}
}
void Game::onKeyUp(unsigned char key) {
	switch (key) {
	case 'a': case 'A': leftHeld = false; break;
	case 'd': case 'D': rightHeld = false; break;
	}
}
void Game::onSpecialDown(int key) {
}
void Game::onSpecialUp(int key) {
}

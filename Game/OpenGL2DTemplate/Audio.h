#pragma once
#include <string>

class Audio {
public:
	// Starts looping background music from a WAV file
	static void PlayMusic(const std::string& wavPath);
	static void StopMusic();

	// Plays a one-shot sound effect from a WAV file
	static void PlaySfx(const std::string& wavPath);
};

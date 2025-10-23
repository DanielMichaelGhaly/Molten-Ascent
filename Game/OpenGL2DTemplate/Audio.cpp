#include "Audio.h"

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#endif

void Audio::PlayMusic(const std::string& wavPath) {
#ifdef _WIN32
	PlaySoundA(wavPath.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
#endif
}

void Audio::StopMusic() {
#ifdef _WIN32
	PlaySoundA(NULL, 0, 0);
#endif
}

void Audio::PlaySfx(const std::string& wavPath) {
#ifdef _WIN32
	PlaySoundA(wavPath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
#endif
}

#ifndef __FMOD_H__
#define __FMOD_H__

#include"fmod.hpp"
#include "cocos2d.h"

USING_NS_CC;
using namespace FMOD;

class AudioEngine{
public:
	static AudioEngine* getInstance();
	void create(const char* songname);
	void createLoop(const char* songname);
	void play();
	void pause();
	void resume();
	void stop();
	void close();
	bool isPlaying();
	bool hasBeat();
	int getLength();
	int getPosition();
	float getBPM();
	float* getSpectrum();
private:
	AudioEngine();
	void init();
	System* system;
	Sound* sound;
	Channel* channel;
	FMOD_RESULT result;
};
#endif 
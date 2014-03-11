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
	void createNRT(const char* songname);
	int getLength();
	char* getName();
	int getPosition();
	float* getSpectrum(int FFTSize);
	bool isPlaying();
	void play();
	void playNRT();
	void pause();
	void release();
	void resume();
	void stop();
	void update();
private:
	AudioEngine();
	void init();
	void initNRT();
	System* system;
	System* systemNRT;
	Channel* channel;
	Sound* sound;
	FMOD_RESULT result;
};
#endif 
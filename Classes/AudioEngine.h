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
	void play();
	void playNRT();
	void pause();
	void resume();
	void stop();
	void close();
	void update();
	bool isPlaying();
	bool hasBeat();
	int getLength();
	int getPosition();
	float getBPM();
	float* getSpectrum();	
private:
	AudioEngine();
	void init();
	void initNRT();
	System* system;
	Channel* channel;
	Sound* sound;
	System* systemNRT;
	FMOD_RESULT result;
};
#endif 
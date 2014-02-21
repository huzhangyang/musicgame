#include "AudioEngine.h"

static AudioEngine* engine = nullptr;//单例

AudioEngine::AudioEngine()
{

}

AudioEngine* AudioEngine::getInstance()
{
	if (!engine)
	{
		engine = new AudioEngine();
		engine->init();
	}
	return engine;
}

void AudioEngine::init()
{
	channel = 0;
	result = FMOD::System_Create(&system);
	result = system->init(1, FMOD_INIT_NORMAL, 0);
}

void AudioEngine::create(char* songname)
{
	result = system->createStream(songname, FMOD_DEFAULT, 0, &sound);
}

void AudioEngine::createLoop(char* songname)
{
	result = system->createStream(songname, FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);
}

void AudioEngine::play()
{
	result = system->playSound(sound, 0, false, &channel);
}

void AudioEngine::pause()
{
	channel->setPaused(TRUE);
}

void AudioEngine::resume()
{
	channel->setPaused(FALSE);
}

void AudioEngine::close()
{
	result = system->release();
}

bool AudioEngine::isPlaying()
{
	bool x;
	result = channel->isPlaying(&x);
	return x;
}

int AudioEngine::getLength()
{
	unsigned int x;
	result = sound->getLength(&x, FMOD_TIMEUNIT_MS);
	x = x * 60 / 1000;//换算成帧数
	return x;
}

int AudioEngine::getPosition()
{
	unsigned int x;
	result = channel->getPosition(&x, FMOD_TIMEUNIT_MS);
	x = x * 60 / 1000;//换算成帧数
	return x;
}


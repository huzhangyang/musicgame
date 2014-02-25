#include "MapGenerator.h"

void MapGenerator::generateMap(const char* songname)
{
	AudioEngine::getInstance()->createNRT(songname);
	AudioEngine::getInstance()->playNRT();
	auto pos = AudioEngine::getInstance()->getPosition();
	auto len = AudioEngine::getInstance()->getLength();
	while (AudioEngine::getInstance()->isPlaying())
	{
		AudioEngine::getInstance()->update();
		float* specData = new float[127];
		specData = AudioEngine::getInstance()->getSpectrum();
		pos = AudioEngine::getInstance()->getPosition();
		delete[] specData;
	}
	log("%s","finished");
}
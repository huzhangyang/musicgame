#include "MapGenerator.h"

void MapGenerator::generateMap(const char* songname)
{
	AudioEngine::getInstance()->createNRT(songname);
	AudioEngine::getInstance()->play();
	auto pos = AudioEngine::getInstance()->getPosition();
	auto len = AudioEngine::getInstance()->getLength();
	while (pos<len)
	{
		AudioEngine::getInstance()->update();
		float* specData = new float[127];
		specData = AudioEngine::getInstance()->getSpectrum();
		pos = AudioEngine::getInstance()->getPosition();
	}
	log("%s","finished");
}
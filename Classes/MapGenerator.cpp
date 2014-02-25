#include "MapGenerator.h"

void MapGenerator::generateMap(const char* songname)
{
	AudioEngine::getInstance()->createNRT(songname);
	AudioEngine::getInstance()->play();
	AudioEngine::getInstance()->update();
	while (AudioEngine::getInstance()->isPlaying())
	{
		AudioEngine::getInstance()->update();
		float* specData = new float[127];
		specData = AudioEngine::getInstance()->getSpectrum();
	}
	log("%s","finished");
}
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
		engine->initNRT();
	}
	return engine;
}

void AudioEngine::init()
{
	channel = 0;
	result = FMOD::System_Create(&system);
	result = system->init(1, FMOD_INIT_NORMAL, 0);
}

void AudioEngine::initNRT()
{
	channel = 0;
	result = FMOD::System_Create(&systemNRT);
	result = systemNRT->setOutput(FMOD_OUTPUTTYPE_NOSOUND_NRT);
	result = systemNRT->init(1, FMOD_INIT_STREAM_FROM_UPDATE, 0);
}

void AudioEngine::create(const char* songname)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	{
		ssize_t size = 0;
		auto data = FileUtils::getInstance()->getFileData(songname, "r", &size);
		FMOD_CREATESOUNDEXINFO exinfo;
		memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
		exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		exinfo.length = size;
		result = system->createStream((const char*)data, FMOD_OPENMEMORY | FMOD_ACCURATETIME, &exinfo, &sound);
	}
#else
	result = system->createStream(songname, FMOD_DEFAULT | FMOD_ACCURATETIME, 0, &sound);
#endif

}

void AudioEngine::createLoop(const char* songname)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	{
		ssize_t size = 0;
		auto data = FileUtils::getInstance()->getFileData(songname, "r", &size);
		FMOD_CREATESOUNDEXINFO exinfo;
		memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
		exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		exinfo.length = size;
		result = system->createStream((const char*)data, FMOD_OPENMEMORY | FMOD_LOOP_NORMAL, &exinfo, &sound);
	}
#else
	result = system->createStream(songname, FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);
#endif
}

void AudioEngine::createNRT(const char* songname)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	{
		ssize_t size = 0;
		auto data = FileUtils::getInstance()->getFileData(songname, "r", &size);
		FMOD_CREATESOUNDEXINFO exinfo;
		memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
		exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		exinfo.length = size;
		result = systemNRT->createStream((const char*)data, FMOD_OPENMEMORY | FMOD_SOFTWARE, &exinfo, &sound);
	}
#else
	result = systemNRT->createStream(songname, FMOD_SOFTWARE, 0, &sound);
#endif
}

int AudioEngine::getLength()
{
	unsigned int x;
	result = sound->getLength(&x, FMOD_TIMEUNIT_MS);
	x = x * 60 / 1000;//换算成帧数
	return x;
}

char* AudioEngine::getName()
{
	FMOD_TAG *tag=new FMOD_TAG;
	char* x="";
	result = sound->getTag("TITLE", 0, tag);
	if (tag->datatype == FMOD_TAGDATATYPE_STRING)
		x = (char*)tag->data;
	return x;
}

int AudioEngine::getPosition()
{
	unsigned int x;
	result = channel->getPosition(&x, FMOD_TIMEUNIT_MS);
	x = x * 60 / 1000;//换算成帧数
	return x;
}

float* AudioEngine::getSpectrum(int FFTSize)
{
	float* specData = new float[FFTSize];
	float* specLeft = new float[FFTSize];
	float* specRight = new float[FFTSize];
	result = channel->getSpectrum(specLeft, FFTSize, 0, FMOD_DSP_FFT_WINDOW_BLACKMANHARRIS);//得到左声道频谱
	result = channel->getSpectrum(specRight, FFTSize, 1, FMOD_DSP_FFT_WINDOW_BLACKMANHARRIS);//得到右声道频谱
	for (int i = 0; i < FFTSize; i++)
		specData[i] = (specLeft[i] + specRight[i]) / 2;//平均左右声道
	return specData;//返回
	delete[] specData;
	delete[] specLeft;
	delete[] specRight;
}

bool AudioEngine::isPlaying()
{
	bool x;
	result = channel->isPlaying(&x);
	return x;
}

void AudioEngine::play()
{
	result = system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
}

void AudioEngine::playNRT()
{
	result = systemNRT->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
}

void AudioEngine::pause()
{
	result = channel->setPaused(true);
}

void AudioEngine::release()
{
	result = sound->release();
	result = system->release();
}

void AudioEngine::resume()
{
	result = channel->setPaused(false);
}

void AudioEngine::stop()
{
	result = channel->stop();
}

void AudioEngine::update()
{
	systemNRT->update();
}
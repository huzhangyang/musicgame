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
	result = system->init(5, FMOD_INIT_NORMAL, 0);
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
		result = system->createStream((const char*)data, FMOD_OPENMEMORY | FMOD_ACCURATETIME, &exinfo, &stream);
	}
#else
	result = system->createStream(songname, FMOD_DEFAULT | FMOD_ACCURATETIME, 0, &stream);
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
		result = system->createStream((const char*)data, FMOD_OPENMEMORY | FMOD_LOOP_NORMAL, &exinfo, &stream);
	}
#else
	result = system->createStream(songname, FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &stream);
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
		result = systemNRT->createStream((const char*)data, FMOD_OPENMEMORY | FMOD_SOFTWARE, &exinfo, &stream);
	}
#else
	result = systemNRT->createStream(songname, FMOD_SOFTWARE, 0, &stream);
#endif
}

void AudioEngine::createSound(const char* songname)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	{
		ssize_t size = 0;
		auto data = FileUtils::getInstance()->getFileData(songname, "r", &size);
		FMOD_CREATESOUNDEXINFO exinfo;
		memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
		exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		exinfo.length = size;
		result = system->createSound((const char*)data, FMOD_OPENMEMORY , &exinfo, &sound);
	}
#else
	result = system->createSound(songname, FMOD_DEFAULT, 0, &sound);
#endif
}

int AudioEngine::getLength()
{
	unsigned int x;
	result = stream->getLength(&x, FMOD_TIMEUNIT_MS);
	x = x * 60 / 1000;//换算成帧数
	return x;
}

char* AudioEngine::getName()
{
	FMOD_TAG *tag = new FMOD_TAG;
	char* x = "";
	result = stream->getTag("TITLE", 0, tag);
	if (tag->datatype == FMOD_TAGDATATYPE_STRING)
		x = (char*)tag->data;
	return x;
}

int AudioEngine::getPosition()
{
	unsigned int x;
	bool y;
	result = soundchannel->isPlaying(&y);
	if (y)
		result = soundchannel->getPosition(&x, FMOD_TIMEUNIT_MS);
	else
		result = channel->getPosition(&x, FMOD_TIMEUNIT_MS);
	x = x * 60 / 1000;//换算成帧数
	return x;
}

float* AudioEngine::getSpectrum(int FFTSize)
{
	float* specData = new float[FFTSize];
	float* specLeft = new float[FFTSize];
	float* specRight = new float[FFTSize];
	result = soundchannel->getSpectrum(specLeft, FFTSize, 0, FMOD_DSP_FFT_WINDOW_BLACKMANHARRIS);//得到左声道频谱
	result = soundchannel->getSpectrum(specRight, FFTSize, 1, FMOD_DSP_FFT_WINDOW_BLACKMANHARRIS);//得到右声道频谱
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

bool AudioEngine::isPlayingSound()
{
	bool x;
	result = soundchannel->isPlaying(&x);
	return x;
}

void AudioEngine::play()
{
	result = system->playSound(FMOD_CHANNEL_FREE, stream, false, &channel);
}

void AudioEngine::playSound()
{
	result = system->playSound(FMOD_CHANNEL_FREE, sound, false, &soundchannel);
}

void AudioEngine::playNRT()
{
	result = systemNRT->playSound(FMOD_CHANNEL_FREE, stream, false, &soundchannel);
}

void AudioEngine::pause()
{
	result = channel->setPaused(true);
}

void AudioEngine::release()
{
	result = sound->release();
	result = stream->release();
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
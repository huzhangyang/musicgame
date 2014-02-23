#include "AudioEngine.h"

static AudioEngine* engine = nullptr;//单例
const int SPEC_LEN = 128;
const float beatThresholdVolume = 0.3f;//拍点音量阀值
const int beatSustain = 150;//
const int beatPostIgnore = 250;
const int beatTrackCutoff = 10000;

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

void AudioEngine::LoadFileIntoMemory(const char *name, void **buff, int *length)
{
	FILE *fp = fopen(name, "rb");
	fseek(fp, 0, SEEK_END);
	*length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	*buff = malloc(*length);
	fread(*buff, *length, 1, fp);
	fclose(fp);
}

void AudioEngine::create(const char* songname)
{
	std::string file = FileUtils::getInstance()->fullPathForFilename(songname);
	//auto data = FileUtils::getInstance()->getDataFromFile(file);这个用不了，是BUG吗
	void *buff = 0;
	int length = 0;
	LoadFileIntoMemory(file.c_str(), &buff, &length);
	FMOD_CREATESOUNDEXINFO exinfo;
	memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
	exinfo.length = length;
	result = system->createStream((const char*)buff, FMOD_OPENMEMORY | FMOD_LOOP_OFF, &exinfo, &sound);
}

void AudioEngine::createLoop(const char* songname)
{
	std::string file = FileUtils::getInstance()->fullPathForFilename(songname);
	//auto data = FileUtils::getInstance()->getDataFromFile(file);这个用不了，是BUG吗
	void *buff = 0;
	int length = 0;
	LoadFileIntoMemory(file.c_str(), &buff, &length);
	FMOD_CREATESOUNDEXINFO exinfo;
	memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
	exinfo.length = length;
	result = system->createStream((const char*)buff, FMOD_OPENMEMORY | FMOD_LOOP_NORMAL, &exinfo, &sound);
}

void AudioEngine::play()
{
	result = system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
}

void AudioEngine::pause()
{
	result = channel->setPaused(true);
}

void AudioEngine::resume()
{
	result = channel->setPaused(false);
}

void AudioEngine::stop()
{
	result = channel->stop();
}

void AudioEngine::close()
{
	result = sound->release();
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

float* AudioEngine::getSpectrum()
{
	system->update();//貌似要先刷新？
	float* specData = new float[SPEC_LEN];
	float* specLeft = new float[SPEC_LEN];
	float* specRight = new float[SPEC_LEN];
	result = channel->getSpectrum(specLeft, SPEC_LEN, 0, FMOD_DSP_FFT_WINDOW_RECT);//得到左声道频谱
	result = channel->getSpectrum(specRight, SPEC_LEN, 1, FMOD_DSP_FFT_WINDOW_RECT);//得到右声道频谱
	for (int i = 0; i < SPEC_LEN; i++)
		specData[i] = (specLeft[i] + specRight[i]) / 2;//平均左右声道
	return specData;//返回
	delete[] specData;
	delete[] specLeft;
	delete[] specRight;
}

float AudioEngine::getBPM()
{
	float bpmEstimate;//待估计BPM值
	int beatThresholdBar = 0;//检测区域
	int beatLastTick = 0;//上个拍子时间
	int beatIgnoreLastTick = 0;//忽略拍点
	std::queue<int> beatTimes;//节拍出现序列

	float* specData = new float[SPEC_LEN];
	specData = this->getSpectrum();
	if (specData[beatThresholdBar] >= beatThresholdVolume && beatLastTick == 0 && beatIgnoreLastTick == 0)
	{
		beatLastTick = GetTickCount();//检测到拍子，记录时间
		beatTimes.push(beatLastTick);//记录到节拍出现序列中
		while (GetTickCount() - beatTimes.front() > beatTrackCutoff)//如果最老的拍点时间太老了就弹出
		{
			beatTimes.pop();
			if (beatTimes.size() == 0)
				break;
		}
	}

	if (GetTickCount() - beatLastTick < beatSustain)
		int x = 1;//出现了beat

	else if (beatIgnoreLastTick == 0 && beatLastTick != 0)
	{
		beatLastTick = 0;
		beatIgnoreLastTick = GetTickCount();//延迟时间内不允许检测下个拍点
	}

	if (GetTickCount() - beatIgnoreLastTick >= beatPostIgnore)
		beatIgnoreLastTick = 0;//延迟时间已过，允许检测下个拍点

	if (beatTimes.size() >= 2)
	{
		float msPerBeat = (beatTimes.back() - beatTimes.front()) / static_cast<float>(beatTimes.size() - 1);//每毫秒beat
		bpmEstimate = 60000 / msPerBeat;
	}
	else
		bpmEstimate = 0;
	return bpmEstimate;
}


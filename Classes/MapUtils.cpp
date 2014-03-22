#include "MapUtils.h"
#include "MainScene.h"
#include <fstream>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define FFT_SIZE 256
#else
#define FFT_SIZE 1024
#endif

const int THRESHOLD_WINDOW_SIZE = 10;
const float MULTIPLIER = 1.5;

Noteline noteline;
AnalyzeInfo info;
MusicInfo musicinfo;
std::ifstream fin;//输入流
FILE* fout;//输出文件
std::string mapname;
float FramePerBeat;
int lastBeatTime = 0, beatBar, lastBeatBar;

MusicInfo MapUtils::loadMap(std::string filename)
{
	mapname = FileUtils::getInstance()->getWritablePath() + filename + ".gnm";
	fin.open(mapname);//打开自动生成测试谱面
	std::string infostring;
	getline(fin, infostring);
	musicinfo.NoteNumber_Easy = atoi(infostring.substr(0, 4).c_str());
	musicinfo.NoteNumber_Hard = atoi(infostring.substr(5, 9).c_str());
	musicinfo.Level_Easy = atoi(infostring.substr(10, 11).c_str());
	musicinfo.Level_Hard = atoi(infostring.substr(12, 13).c_str());;
	getNoteline();//读取第一行
	return musicinfo;
}

void MapUtils::closeMap()
{
	fin.close();
}

void MapUtils::getNoteline()
{
	std::string notestring;
	int difficulty = UserDefault::getInstance()->getIntegerForKey("difficulty");
	if (getline(fin, notestring))
	{
		noteline.time = atoi(notestring.substr(0, 5).c_str());
		noteline.difficulty = atoi(notestring.substr(6, 7).c_str());
		noteline.type = atoi(notestring.substr(8, 9).c_str());
		noteline.length = atoi(notestring.substr(10, 13).c_str());
		noteline.posX = atoi(notestring.substr(14, 18).c_str());
		noteline.posY = atoi(notestring.substr(19, 22).c_str());
		if (noteline.difficulty > difficulty)
			getNoteline();
	}
	else
		noteline.time = 0;//结束标识符
}

Point MapUtils::getNextPos()
{
	Point ret;
	ret.x = noteline.posX;
	ret.y = noteline.posY;
	return ret;
}

void MapUtils::generateMap(const char* songname)
{
	mapname = songname;
	mapname = FileUtils::getInstance()->getWritablePath() + mapname.substr(mapname.find_last_of('/') + 1, mapname.find_last_of('.') - mapname.find_last_of('/') - 1) + ".gnm";
	fout = fopen(mapname.c_str(), "w");//打开测试谱面
	fprintf(fout, "///////////////");
	std::thread workthread(generate, songname);
	workthread.detach();
}

void MapUtils::generate(const char* songname)
{
	FramePerBeat = 3600 / BPM;//最小节奏持续帧数
	musicinfo.NoteNumber_Easy = 0;
	musicinfo.NoteNumber_Hard = 0;
	///////////////
	AudioEngine::getInstance()->createNRT(songname);
	AudioEngine::getInstance()->playNRT();
	float* spectrum = new float[FFT_SIZE];
	float* lastSpectrum = new float[FFT_SIZE];
	std::vector<float> spectralFlux;
	std::vector<float> threshold;
	std::vector<float> prunnedSpectralFlux;
	std::vector<float> peaks;
	//////////////////扫描开始/////////////////////
	while (AudioEngine::getInstance()->isPlayingSound())
	{
		AudioEngine::getInstance()->update();
		lastSpectrum = spectrum;
		spectrum = AudioEngine::getInstance()->getSpectrum(FFT_SIZE);
		float flux = 0;
		for (int bar = 0; bar < FFT_SIZE - 1; bar++)
		{
			float value = (spectrum[bar] - lastSpectrum[bar]);
			flux += value < 0 ? 0 : value;
		}
		spectralFlux.push_back(flux);
	}
	for (int i = 0; i < spectralFlux.size(); i++)
	{
		int start = MAX(0, i - THRESHOLD_WINDOW_SIZE);
		int end = MIN(spectralFlux.size() - 1, i + THRESHOLD_WINDOW_SIZE);
		float mean = 0;
		for (int j = start; j <= end; j++)
			mean += spectralFlux.at(j);
		mean /= (end - start);
		threshold.push_back(mean * MULTIPLIER);
	}
	for (int i = 0; i < threshold.size(); i++)
	{
		if (threshold.at(i) <= spectralFlux.at(i))
			prunnedSpectralFlux.push_back(spectralFlux.at(i) - threshold.at(i));
		else
			prunnedSpectralFlux.push_back((float)0);
	}
	for (int i = 0; i < prunnedSpectralFlux.size() - 1; i++)
	{
		if (prunnedSpectralFlux.at(i) > prunnedSpectralFlux.at(i + 1))
			peaks.push_back(prunnedSpectralFlux.at(i));
		else
			peaks.push_back((float)0);
	}
	noteline.type = 0;
	for (int i = 1; i < peaks.size(); i++)
	{
		if (peaks.at(i)>0.001)
		{
			noteline.time = i * 1024 * 60 / 44100;
			noteline.difficulty = 1;
			noteline.posY = genPosY(noteline.time);
			noteline.posX = genPosX(noteline.posY);
			noteline.length = noteline.time - lastBeatTime;
			if (noteline.length < FramePerBeat / 2)
				noteline.type = CCRANDOM_0_1() * 2 + 1;
			else
			{
				if (noteline.length > FramePerBeat)
					noteline.difficulty = 0;
				writeNoteline();
				noteline.type = 0;
			}
			lastBeatTime = noteline.time;
		}
	}
	//////////////////扫描结束/////////////////////
	rewind(fout);
	musicinfo.Level_Easy = musicinfo.NoteNumber_Easy * 360 / AudioEngine::getInstance()->getLength();
	if (musicinfo.Level_Easy > 9)
		musicinfo.Level_Easy = 9;
	musicinfo.Level_Hard = musicinfo.NoteNumber_Hard * 360 / AudioEngine::getInstance()->getLength();
	if (musicinfo.Level_Hard > 9)
		musicinfo.Level_Hard = 9;
	fprintf(fout, "%4d %4d %1d %1d\n", musicinfo.NoteNumber_Easy, musicinfo.NoteNumber_Hard, musicinfo.Level_Easy, musicinfo.Level_Hard);
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([]
	{
		MainScene::loadingEnd();
	});
	fclose(fout);
}

void MapUtils::writeNoteline()
{
	fprintf(fout, "%.5d,", noteline.time);
	fprintf(fout, "%.1d,", noteline.difficulty);
	fprintf(fout, "%.1d,", noteline.type);
	fprintf(fout, "%.3d,", noteline.length);
	fprintf(fout, "%.4d,", noteline.posX);
	fprintf(fout, "%.4d\n", noteline.posY);
	if (noteline.difficulty == 0)
	{
		musicinfo.NoteNumber_Easy++;
		musicinfo.NoteNumber_Hard++;
	}
	else
	{
		musicinfo.NoteNumber_Hard++;
	}
	log("%d %d %d %d", noteline.time, noteline.type, noteline.posX, noteline.posY);
}

int MapUtils::genPosX(int posY)
{
	static int hand = 0;//0为中间，1为左手，2为右手
	static int x = 675;
	hand = CCRANDOM_0_1() * 3;
	if (noteline.time - lastBeatTime < FramePerBeat*1.5 || noteline.type == 1)//间隔较短，换手操作
	{
		if (hand == 1)
		{
			hand = 2;
			x = 175 + CCRANDOM_0_1() * 500;
		}
		else if (hand == 2)
		{
			hand = 1;
			x = 675 + CCRANDOM_0_1() * 500;
		}
		else
			x = 675 + CCRANDOM_MINUS1_1() * 500;
	}
	else
	{
		hand = CCRANDOM_0_1() * 3;
		if (hand == 1)
		{
			x -= 150;
			if (x<175)
				x = 325 + CCRANDOM_0_1() * 350;
		}
		else if (hand == 2)
		{
			x += 150;
			if (x>1175)
				x = 675 + CCRANDOM_0_1() * 350;
		}
		else
			x = x;
	}
	return x;
}

int MapUtils::genPosY(int time)
{
	int y = 0;
	y = time % (int)(FramePerBeat * 4);
	if (y < FramePerBeat)
	{
		y = 305 - 240 * y / FramePerBeat;
	}
	else if (y < FramePerBeat * 2)
	{
		y = 65 + 240 * (y - FramePerBeat) / FramePerBeat;
	}
	else if (y < FramePerBeat * 3)
	{
		y = 305 + 240 * (y - FramePerBeat * 2) / FramePerBeat;
	}
	else if (y < FramePerBeat * 4)
	{
		y = 545 - 240 * (y - FramePerBeat * 3) / FramePerBeat;
	}
	return y;
}
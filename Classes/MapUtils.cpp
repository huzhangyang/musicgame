#include "MapUtils.h"
#include <fstream>

const float BEAT_THRESHOLD = 0.025f;//拍点音量阀值
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define FFT_SIZE 256
#else
#define FFT_SIZE 1024
#endif

Noteline noteline;
AnalyzeInfo info;
std::ifstream fin, fin2;//输入流
FILE* fout;//输出文件
std::string mapname;
int type = 0;
int maxBar, minBar, avgBar;
float FramePerBeat;

void MapUtils::loadMap(std::string filename)
{
	mapname = FileUtils::getInstance()->getWritablePath() + filename + ".gnm";
	fin.open(mapname);//打开自动生成测试谱面
	getNoteline();//读取第一行
}

void MapUtils::closeMap()
{
	fin.close();
}

int MapUtils::getNoteNumber()
{
	int difficulty = UserDefault::getInstance()->getIntegerForKey("difficulty");
	int ret = 0;
	std::string temp;
	fin2.open(mapname);//打开自动生成测试谱面
	while (getline(fin2, temp))
	{
		if (atoi(temp.substr(6, 7).c_str()) <= difficulty)
			ret++;
	}
	fin2.close();
	return ret;
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
	FramePerBeat = 3600 / BPM;//最小节奏持续帧数
	fout = fopen(mapname.c_str(), "w");//打开测试谱面
	AudioEngine::getInstance()->createNRT(songname);
	AudioEngine::getInstance()->playNRT();
	//////////////////初始化/////////////////////
	maxBar = 0;
	minBar = FFT_SIZE;
	int counter = 0;
	while (AudioEngine::getInstance()->isPlayingSound())
	{
		AudioEngine::getInstance()->update();
		analyzeBeat();
		if (info.beatBar >= 0)
		{
			counter++;
			if (info.beatBar > maxBar)maxBar = info.beatBar;
			if (info.beatBar < minBar)minBar = info.beatBar;
			avgBar += info.beatBar;
		}
	}
	avgBar = avgBar / counter;
	AudioEngine::getInstance()->createNRT(songname);
	AudioEngine::getInstance()->playNRT();
	//////////////////一轮扫描/////////////////////
	while (AudioEngine::getInstance()->isPlayingSound())
	{
		AudioEngine::getInstance()->update();
		analyzeBeatV2();
		if (info.beatBar<0)
			info.lastBeatBar = -99;
		else if (info.beatTick - info.lastbeatTick > FramePerBeat)//过了最小生成时间
		{
			if (info.beatBar <= info.lastBeatBar + sqrt(FFT_SIZE / 256) && info.beatBar >= info.lastBeatBar - sqrt(FFT_SIZE / 256))
			{
				if (info.beatTick - info.lastbeatTick > FramePerBeat * 1.5)
					type = 1;
			}
			else if (info.beatBar <= info.lastBeatBar + FFT_SIZE / 256 && info.beatBar >= info.lastBeatBar - FFT_SIZE / 256)
			{
				type = 2;
			}
			else//否则点击
			{
				writeNoteline(info.difficulty, type, info.beatTick - info.lastbeatTick);
				info.lastbeatTick = info.beatTick;
				type = 0;
			}
			info.lastBeatBar = info.beatBar;
		}
	}
	//////////////////二轮扫描/////////////////////
	fclose(fout);
}

void MapUtils::analyzeBeat()
{
	float DBavg = 0, DBmax = 0;//每一帧的平均dB和最大dB
	float* specData = new float[FFT_SIZE];
	specData = AudioEngine::getInstance()->getSpectrum(FFT_SIZE);
	for (int bar = 0; bar < FFT_SIZE - 1; bar++)
	{
		DBavg += specData[bar];
		if (specData[bar]>DBmax)
		{
			DBmax = specData[bar];
			info.beatBar = bar;
		}
	}
	DBavg = DBavg / FFT_SIZE;
	if ((DBmax / DBavg) < 2.5 || DBmax < BEAT_THRESHOLD)
		info.beatBar = -1;
	delete[] specData;
}

void MapUtils::analyzeBeatV2()
{
	float DBavg = 0, DBmax = 0;//每一帧的平均dB和最大dB
	float* specData = new float[FFT_SIZE];
	specData = AudioEngine::getInstance()->getSpectrum(FFT_SIZE);
	for (int bar = minBar; bar <= maxBar; bar++)
	{
		DBavg += specData[bar];
		if (specData[bar] > DBmax)
		{
			DBmax = specData[bar];
			info.beatBar = bar;
		}
	}
	DBavg = DBavg / (maxBar - minBar);
	if ((DBmax / DBavg) >= 2.5 &&DBmax >= BEAT_THRESHOLD)
	{
		info.beatTick = AudioEngine::getInstance()->getPosition();
		info.difficulty = 1;
		if ((DBmax / DBavg) >= 3 && DBmax >= BEAT_THRESHOLD * 3)
			info.difficulty = 0;
	}
	else info.beatBar = -1;
	delete[] specData;
}

void MapUtils::writeNoteline(int difficulty, int type, int length)
{
	noteline.time = info.beatTick;
	noteline.difficulty = difficulty;
	noteline.type = type;
	if (type == 1)
		noteline.length = length / 3;
	else
		noteline.length = FramePerBeat * 2;
	noteline.posY = genPosY();
	noteline.posX = genPosX(noteline.posY);
	fprintf(fout, "%.5d,", noteline.time);
	fprintf(fout, "%.1d,", noteline.difficulty);
	fprintf(fout, "%.1d,", noteline.type);
	fprintf(fout, "%.3d,", noteline.length);
	fprintf(fout, "%.4d,", noteline.posX);
	fprintf(fout, "%.4d\n", noteline.posY);
	log("%d %d %d %d", noteline.time, noteline.type, noteline.posX, noteline.posY);
}

int MapUtils::genPosX(int posY)
{
	int x = 0;
	if (info.beatBar < avgBar)
		x = 175 + 500 * (info.beatBar - minBar) / (avgBar - minBar);
	else
		x = 675 + 500 * (info.beatBar - avgBar) / (maxBar - avgBar);
	return x;
}

int MapUtils::genPosY()
{
	int y = 0;
	y = info.beatTick % (int)(FramePerBeat * 4);
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
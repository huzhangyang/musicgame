#include "MapUtils.h"
#include <fstream>

const float BEAT_THRESHOLD = 0.025f;//拍点音量阀值
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define FFT_SIZE 256
#else
#define FFT_SIZE 1024
#endif

Noteline noteline;
std::ifstream fin, fin2;//输入流
FILE* fout;//输出文件
std::string mapname;

int beatTick = 0, lastbeatTick = 0, beatBar = 0, lastBeatBar = 0;
int lastX = 0, lastY = 0, trend = 0;//0为往下生成，1为往上生成
float FramePerBeat;

void MapUtils::generateMap(const char* songname)
{
	mapname = songname;
	mapname = FileUtils::getInstance()->getWritablePath() + mapname.substr(mapname.find_last_of('/') + 1, mapname.find_last_of('.') - mapname.find_last_of('/') - 1) + ".gnm";
	FramePerBeat = 3600 / BPM;//最小节奏持续帧数
	int type = 0;
	fout = fopen(mapname.c_str(), "w");//打开测试谱面
	//////////////////初始化/////////////////////
	AudioEngine::getInstance()->createNRT(songname);
	AudioEngine::getInstance()->playNRT();
	while (AudioEngine::getInstance()->isPlayingSound())
	{
		AudioEngine::getInstance()->update();
		beatBar = getBeat();
		if (beatBar<0)
			lastBeatBar = -99;
		else if (beatTick - lastbeatTick > FramePerBeat)//过了最小生成时间
		{
			if (beatBar <= lastBeatBar + sqrt(FFT_SIZE / 256) && beatBar >= lastBeatBar - sqrt(FFT_SIZE / 256))
			{
				if (beatTick - lastbeatTick > FramePerBeat * 1.5)
					type = 1;
			}
			else if (beatBar <= lastBeatBar + FFT_SIZE / 256 && beatBar >= lastBeatBar - FFT_SIZE / 256)
			{
				type = 2;
			}
			else//否则点击
			{
				writeNoteline(type, beatTick - lastbeatTick);
				lastbeatTick = beatTick;
				type = 0;
			}
			lastBeatBar = beatBar;
		}
	}
	fclose(fout);
}

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

int MapUtils::getBeat()
{
	int maxBar;
	float DBavg = 0, DBmax = 0;//每一帧的平均dB和最大dB
	float* specData = new float[FFT_SIZE];
	specData = AudioEngine::getInstance()->getSpectrum(FFT_SIZE);
	for (int bar = 0; bar < FFT_SIZE - 1; bar++)
	{
		DBavg += specData[bar];
		if (specData[bar]>DBmax)
		{
			DBmax = specData[bar];
			maxBar = bar;
		}

	}
	delete[] specData;
	DBavg = DBavg / FFT_SIZE;
	if ((DBmax / DBavg) >= 2.5 &&DBmax >= BEAT_THRESHOLD)
	{
		beatTick = AudioEngine::getInstance()->getPosition();
		return maxBar;
	}
	return -1;
}

void MapUtils::writeNoteline(int type, int length)
{
	noteline.time = beatTick;
	noteline.difficulty = CCRANDOM_0_1() * 2;
	noteline.type = type;
	if (type == 1)
		noteline.length = length / 3;
	else
		noteline.length = FramePerBeat * 2;
	noteline.posY = genPosY(noteline.time);
	noteline.posX = genPosX(noteline.posY, noteline.length);
	fprintf(fout, "%.5d,", noteline.time);
	fprintf(fout, "%.1d,", noteline.difficulty);
	fprintf(fout, "%.1d,", noteline.type);
	fprintf(fout, "%.3d,", noteline.length);
	fprintf(fout, "%.4d,", noteline.posX);
	fprintf(fout, "%.4d\n", noteline.posY);
	lastX = noteline.posX;
	lastY = noteline.posY;
	//log("%d %d %d %d", noteline.time, noteline.type, noteline.posX, noteline.posY);
}

int MapUtils::genPosX(int posY, int length)
{
	int x = 0;
	x = (beatBar * 128 / FFT_SIZE) * 100 + 175;
	if (x<175 || x>1175)
	{
		log("%s", "invalid");
		x = CCRANDOM_0_1() * 1000 + 175;
	}

	return x;
}

int MapUtils::genPosY(int time)
{
	int y = 0;
	y = time % (int)(FramePerBeat * 4);
	if (y < FramePerBeat)
	{
		trend = 0;
		y = 305 - 240 * y / FramePerBeat;
	}
	else if (y < FramePerBeat * 2)
	{
		trend = 1;
		y = 65 + 240 * (y - FramePerBeat) / FramePerBeat;
	}
	else if (y < FramePerBeat * 3)
	{
		trend = 1;
		y = 305 + 240 * (y - FramePerBeat * 2) / FramePerBeat;
	}
	else if (y < FramePerBeat * 4)
	{
		trend = 0;
		y = 545 - 240 * (y - FramePerBeat * 3) / FramePerBeat;
	}
	return y;
}
#include "MapUtils.h"
#include <fstream>

const float BPM = 139.65f;
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
int lastX = 0, lastY = 0, trend;//0为往下生成，1为往上生成
float FramePerBeat;
int UseMap[10][10];

void MapUtils::generateMap(const char* songname)
{
	std::string mapname = songname;
	mapname = FileUtils::getInstance()->getWritablePath() + mapname.substr(mapname.find_last_of('/') + 1, mapname.find_last_of('.') - mapname.find_last_of('/') - 1) + ".gnm";
	FramePerBeat = 3600 / BPM;//最小节奏持续帧数
	int type = 0;
	for (int i = 1; i <= 9; i++)
		for (int j = 1; j <= 9; j++)
			UseMap[i][j] = 0;
	fout = fopen(mapname.c_str(), "w");//打开测试谱面
	//////////////////初始化/////////////////////
	AudioEngine::getInstance()->createNRT(songname);
	AudioEngine::getInstance()->playNRT();
	while (AudioEngine::getInstance()->isPlaying())
	{
		AudioEngine::getInstance()->update();
		for (int i = 1; i <= 9; i++)
			for (int j = 1; j <= 9; j++)
				if (UseMap[i][j] > 0)
					UseMap[i][j]--;
		beatBar = getBeat();
		if (beatBar<0)
			lastBeatBar = -99;
		else if (beatTick - lastbeatTick > FramePerBeat)//过了最小生成时间
		{
			if (beatBar <= lastBeatBar + sqrt(FFT_SIZE / 256) && beatBar >= lastBeatBar - sqrt(FFT_SIZE / 256))
			{
				if (beatTick - lastbeatTick > FramePerBeat * 2)
					type = 1;
				else
					type = 2;
			}
			else//否则点击
			{
				writeNoteline(type, (beatTick - lastbeatTick) / 2);
				log("%d %d", beatTick, beatBar);
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
		noteline.pos = atoi(notestring.substr(14, 16).c_str());
		if (noteline.difficulty > difficulty)
			getNoteline();
	}
	else
		noteline.time = 0;//结束标识符
}

Point MapUtils::getNextPos()
{
	Point ret;
	ret.x = 120 * (noteline.pos / 10) + 80;
	ret.y = 60 * (10 - noteline.pos % 10) + 5;
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
	noteline.length = length;
	noteline.pos = getPosY(noteline.time);
	noteline.pos += getPosX(noteline.pos, length) * 10;
	fprintf(fout, "%.5d,", noteline.time);
	fprintf(fout, "%.1d,", noteline.difficulty);
	fprintf(fout, "%.1d,", noteline.type);
	fprintf(fout, "%.3d,", noteline.length);
	fprintf(fout, "%.2d\n", noteline.pos);
	lastX = noteline.pos % 10;
	lastY = noteline.pos / 10;
}

int MapUtils::getPosX(int posY, int length)
{
	int x;
	if (lastY <= 1 || lastY >= 9)
	{
		if (beatBar == 0)
			x = 0;
		else
			x = beatBar * 128 / FFT_SIZE;
	}
	else if (lastY != posY)
		x = lastX;
	if (x > 9 || x < 1)
	{
		do
		{
			x = CCRANDOM_0_1() * 8 + 1;
		} while (UseMap[x][posY]>0);
	}
	UseMap[x][posY] = length;
	return x;
}

int MapUtils::getPosY(int time)
{
	int y;
	if (lastY <= 1)
	{
		trend = 0;
		y = beatTick % (int)(FramePerBeat * 4);
	}
	else if (lastY >= 9)
	{
		trend = 1;
		y = beatTick % (int)(FramePerBeat * 4);
	}
	else
	{
		if (trend == 0)
			y = lastY + 1;
		else y = lastY - 1;
		return y;
	}
	switch (y % 16)
	{
	case 0:return 5;
	case 1:return 6;
	case 2:return 7;
	case 3:return 8;
	case 4:return 9;
	case 5:return 8;
	case 6:return 7;
	case 7:return 6;
	case 8:return 5;
	case 9:return 4;
	case 10:return 3;
	case 11:return 2;
	case 12:return 1;
	case 13:return 2;
	case 14:return 3;
	case 15:return 4;
	}
	if (y > 9 || y < 1)
		y = CCRANDOM_0_1() * 8 + 1;
	return y;
}
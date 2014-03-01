#include "MapGenerator.h"
#include <fstream>

const float BPM = 133.21f;
const float BEAT_THRESHOLD = 0.025f;//拍点音量阀值
const int BEAT_MINLASTTIME = 27;//最小节奏持续帧数
const int FFT_SIZE = 256;

FILE* fout;//输出文件
int beatTick, lastbeatTick, beatBar, lastBeatBar;
int UseMap[9][9];

void MapGenerator::generateMap(const char* songname)
{
	AudioEngine::getInstance()->createNRT(songname);
	AudioEngine::getInstance()->playNRT();
	float* specData = new float[FFT_SIZE];
	std::string mapname = songname;
	mapname = FileUtils::getInstance()->getWritablePath() + mapname.substr(mapname.find_last_of('/') + 1, mapname.find_last_of('.') - mapname.find_last_of('/') - 1) + ".gnm";
	fout = fopen(mapname.c_str(), "w");//打开测试谱面

	while (AudioEngine::getInstance()->isPlaying())
	{
		AudioEngine::getInstance()->update();
		for (int i = 1; i <= 9; i++)
			for (int j = 1; j <= 9; j++)
				if (UseMap[i][j] > 0)
					UseMap[i][j]--;
		int beatBar = getBeat();
		if (beatBar >= 0)
		{
			if (beatBar <= lastBeatBar + 2 && beatBar >= lastBeatBar - 2)
			{

			}
			else if (lastbeatTick + BEAT_MINLASTTIME < beatTick)
			{
				writeNoteline(CCRANDOM_0_1() * 3);
				log("%d %d", beatTick, beatBar);
				lastbeatTick = beatTick;
			}
			lastBeatBar = beatBar;
		}
		else lastBeatBar = -99;
	}
	fclose(fout);
	delete[] specData;
}

int MapGenerator::getBeat()
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

void MapGenerator::writeNoteline(int type)
{
	int time = beatTick;
	int difficulty = CCRANDOM_0_1() * 2;
	int length = 60;
	int posY = getPosY(time);
	int posX = CCRANDOM_0_1() * 8 + 1;
	int desY = getPosY(time + length);
	int desX = getPosX(desY, length);
	fprintf(fout, "%.5d,", time);
	fprintf(fout, "%.1d,", difficulty);
	fprintf(fout, "%.1d,", type);
	fprintf(fout, "%.3d,", length);
	fprintf(fout, "%.1d%.1d,", posX, posY);
	fprintf(fout, "%.1d%.1d\n", desX, desY);
}

int MapGenerator::getPosX(int posY, int length)
{
	for (int i = 1; i <= 9; i++)
		if (UseMap[i][posY] == 0)
		{
			UseMap[i][posY] = length;
			return i;
		}
	return 0;
}

int MapGenerator::getPosY(int time)
{
	float FramePerBeat = 3600.0 / BPM;
	auto x = beatTick % (int)(FramePerBeat * 4);
	switch (x % 16)
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
	return 0;
}
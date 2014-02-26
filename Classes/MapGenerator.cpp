#include "MapGenerator.h"
#include <fstream>

const float BEAT_THRESHOLD = 0.025f;//拍点音量阀值
const int BEAT_MINLASTTIME = 27;//最小节奏持续帧数
const int FFT_SIZE = 1024;

FILE* fout;//输出文件
int beatTick, lastbeatTick, beatBar, lastBeatBar;


void MapGenerator::generateMap(const char* songname)
{
	AudioEngine::getInstance()->createNRT(songname);
	AudioEngine::getInstance()->playNRT();
	float* specData = new float[FFT_SIZE];
	std::string mapname = songname;
	mapname = FileUtils::getInstance()->getWritablePath() + mapname.substr(mapname.find_last_of('/') + 1, mapname.find_last_of('.') - mapname.find_last_of('/') - 1) + ".gnm";
	fopen_s(&fout, mapname.c_str(), "w");//打开测试谱面
	while (AudioEngine::getInstance()->isPlaying())
	{
		AudioEngine::getInstance()->update();
		int beatBar = getBeat();
		if (beatBar >= 0)
		{
			if (beatBar <= lastBeatBar + 2 && beatBar >= lastBeatBar - 2)
			{

			}
			else
			{
				writeNoteline();
				log("%d %d", beatTick, beatBar);
				lastbeatTick = beatTick;
			}
			lastBeatBar = beatBar;
		}
		else lastBeatBar = -99;
	}
	fclose(fout);
	delete[] specData;
	log("%s", "finished");
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

void MapGenerator::writeNoteline()
{
	int time = beatTick;
	int difficulty = CCRANDOM_0_1() * 2;
	//int type = CCRANDOM_0_1() * 3;
	int type = 0;
	int length = 60;
	//int posX = beatBar / 8 + 1;
	int posX = CCRANDOM_0_1() * 8 + 1;
	int posY = CCRANDOM_0_1() * 8 + 1;
	int desX = CCRANDOM_0_1() * 8 + 1;
	int desY = CCRANDOM_0_1() * 8 + 1;
	fprintf_s(fout, "%.5d,", time);
	fprintf_s(fout, "%.1d,", difficulty);
	fprintf_s(fout, "%.1d,", type);
	fprintf_s(fout, "%.3d,", length);
	fprintf_s(fout, "%.1d%.1d,", posX, posY);
	fprintf_s(fout, "%.1d%.1d\n", desX, desY);
}
#include "MapUtils.h"
#include <fstream>

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
int maxBar, minBar, lBar, rBar, currBar;
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
	//////////////////初始化/////////////////////
	AudioEngine::getInstance()->createNRT(songname);
	AudioEngine::getInstance()->playNRT();
	maxBar = 0;
	minBar = FFT_SIZE;
	lBar = 0;
	rBar = 0;
	int counter[FFT_SIZE];
	for (int i = 0; i < FFT_SIZE; i++)
		counter[i] = 0;
	while (AudioEngine::getInstance()->isPlayingSound())
	{
		AudioEngine::getInstance()->update();
		analyzeBeat();
		if (info.beatBar >= 0)
		{
			counter[info.beatBar]++;
			if (info.beatBar > maxBar)maxBar = info.beatBar;//找到所有出现beat的Bar的最大值
			if (info.beatBar < minBar)minBar = info.beatBar;//找到所有出现beat的Bar的最小值
		}
	}
	int countFirst = 0, countSecond = 0;//找到出现beat最多和第二多的Bar
	for (int i = 0; i < FFT_SIZE; i++)
	{
		if (counter[i]>countFirst)
		{
			if (abs(lBar - rBar)>(FFT_SIZE / 128))
			{
				countSecond = countFirst;
				lBar = rBar;
			}
			countFirst = counter[i];
			rBar = i;
		}
		else if (counter[i] > countSecond)
		{
			if (abs(lBar - i) > (FFT_SIZE / 128))
			{
				countSecond = counter[i];
				lBar = i;
			}
		}
	}
	//////////////////一轮扫描/////////////////////
	AudioEngine::getInstance()->createNRT(songname);
	AudioEngine::getInstance()->playNRT();
	noteline.type = -1;
	noteline.time = -1;
	while (AudioEngine::getInstance()->isPlayingSound())
	{
		AudioEngine::getInstance()->update();
		analyzeBeatV2();
		if (info.beatBar >= 0 && abs(info.lastBeatBar - info.beatBar) <= (FFT_SIZE / 128))//不到生成note的时间，通过分析频域决定note类型
		{
			if (info.beatTick - info.lastbeatTick > FramePerBeat * 2)
			{
				if (abs(info.lastBeatBar - info.beatBar) <= sqrt(FFT_SIZE / 256) && (info.beatTick - info.lastbeatTick < FramePerBeat * 4))
					noteline.type = 1;
				else
					noteline.type = 2;
			}
			else
			{
				noteline.type = 0;
			}
			if (noteline.time <= 0)
			{
				noteline.time = info.beatTick;
				noteline.difficulty = info.difficulty;
				currBar = info.beatBar;
			}
		}
		else if (noteline.type >= 0)//生成note
		{
			noteline.length = info.beatTick - noteline.time;
			noteline.posY = genPosY(noteline.time);
			noteline.posX = genPosX(noteline.posY);
			writeNoteline();
			info.lastbeatTick = noteline.time;
			info.lastBeatBar = currBar;
			noteline.type = -1;
			noteline.time = -1;
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
	if ((DBmax / DBavg) >= 2.5 && DBmax >= 0.025)
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
	if ((DBmax / DBavg) >= 2.5 && DBmax >= 0.025)
	{
		info.beatTick = AudioEngine::getInstance()->getPosition();
		info.difficulty = 1;
		if ((DBmax / DBavg) >= 5 && DBmax >= 0.1)
			info.difficulty = 0;
	}
	else info.beatBar = -1;
	delete[] specData;
}

void MapUtils::writeNoteline()
{
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
	static int hand = 0;//0为中间，1为左手，2为右手
	static int x = 0;
	if (noteline.time - info.lastbeatTick < FramePerBeat / 2)//间隔较短，换手操作
	{
		if (hand == 1)
		{
			hand = 2;
			x = 675 + 500 * (currBar - minBar) / (maxBar - minBar);
		}
		else if (hand == 2)
		{
			hand = 1;
			x = 175 + 500 * (currBar - minBar) / (maxBar - minBar);
		}
		else
			x = 675;
	}
	else if (noteline.time - info.lastbeatTick < FramePerBeat)//间隔较长，不换手
	{
		if (hand == 1)
		{
			x = 175 + 500 * (currBar - minBar) / (maxBar - minBar);
		}
		else if (hand == 2)
		{
			x = 675 + 500 * (currBar - minBar) / (maxBar - minBar);
		}
		else
			x = 675;
	}
	else
	{

	}
	if (currBar < info.lastBeatBar)
		x = 175 + 500 * 2 / 3 + 500 * 2 * (currBar - lBar) / (rBar - lBar) / 3;
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
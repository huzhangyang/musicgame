#include "MapUtils.h"
#include "MainScene.h"
#include <fstream>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define FFT_SIZE 1024
#else
#define FFT_SIZE 1024
#endif

Noteline noteline;
AnalyzeInfo info;
std::ifstream fin, fin2;//输入流
FILE* fout;//输出文件
std::string mapname;
int maxBar, minBar, firstBar, currBar;
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
	fout = fopen(mapname.c_str(), "w");//打开测试谱面
	std::thread workthread(generate, songname);
	workthread.detach();
}

void MapUtils::generate(const char* songname)
{
	FramePerBeat = 3600 / BPM;//最小节奏持续帧数
	///////////////
	AudioEngine::getInstance()->createNRT(songname);
	AudioEngine::getInstance()->playNRT();
	maxBar = 0;
	minBar = FFT_SIZE;
	firstBar = 0;
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
	int countFirst = 0; //找到出现beat最多的Bar
	for (int i = 0; i < FFT_SIZE; i++)
	{
		if (counter[i]>countFirst)
		{
			countFirst = counter[i];
			firstBar = i;
		}
	}
	//////////////////一轮扫描/////////////////////
	AudioEngine::getInstance()->playNRT();
	noteline.type = -1;
	noteline.time = -1;
	while (AudioEngine::getInstance()->isPlayingSound())
	{
		AudioEngine::getInstance()->update();
		analyzeBeatV2();
		if (info.beatBar >= 0 && abs(info.lastBeatBar - info.beatBar) <= (FFT_SIZE / 256))//不到生成note的时间，通过分析频域决定note类型
		{
			if (info.beatTick - noteline.time > FramePerBeat * 2)
			{
				noteline.type = 2;
			}
			else if (info.beatTick - noteline.time > FramePerBeat)
			{
				noteline.type = 1;
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
			if (noteline.time - info.lastbeatTick > FramePerBeat / 4)
			{
				noteline.length = info.beatTick - noteline.time;
				noteline.posY = genPosY(noteline.time);
				noteline.posX = genPosX(noteline.posY);
				noteline.difficulty = 1;
				if (noteline.time - info.lastbeatTick > FramePerBeat / 2)
					noteline.difficulty = 0;
				writeNoteline();
			}
			info.lastbeatTick = noteline.time;
			info.lastBeatBar = currBar;
			noteline.type = -1;
			noteline.time = -1;
		}
	}
	//////////////////二轮扫描/////////////////////
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([]
	{
		MainScene::loadingEnd();
	});
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
	if (DBmax >= 0.001)
	{
		info.beatTick = AudioEngine::getInstance()->getPosition();
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
	static int x = 675;
	if (noteline.time - info.lastbeatTick < FramePerBeat*1.5 || noteline.type == 1)//间隔较短，换手操作
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
		if (currBar < firstBar)
		{
			hand = 1;
		}
		else if (currBar>firstBar)
		{
			hand = 2;
		}
		else
			hand = 0;
		if (currBar <info.lastBeatBar - FFT_SIZE / 256)
		{
			x -= 150;
			if (x<175)
				x = 325 + CCRANDOM_0_1() * 350;
		}
		else if (currBar>info.lastBeatBar + FFT_SIZE / 256)
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
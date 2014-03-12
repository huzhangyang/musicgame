#ifndef __MAPUTILS_H__
#define __MAPUTILS_H__

#include "AudioEngine.h"
#include "Global.h"

class MapUtils{
public:
	static void loadMap(std::string filename);//加载谱面
	static void closeMap();//关闭谱面
	static int getNoteNumber();//得到当前难度下的总音符数
	static void getNoteline();//得到当前难度下的下一行谱面
	static Point getNextPos();//得到下个note的位置
	static void generateMap(const char* songname);//生成谱面
private:
	static void analyzeBeat();
	static void analyzeBeatV2();
	static void writeNoteline(int difficulty, int type, int length);
	static int genPosX(int posY);
	static int genPosY();
};
struct AnalyzeInfo
{
	int beatTick = 0;
	int lastbeatTick = 0;
	int beatBar = 0;
	int lastBeatBar = 0;
	int difficulty = 0;
};
#endif 

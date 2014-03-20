#ifndef __MAPUTILS_H__
#define __MAPUTILS_H__

#include "AudioEngine.h"
#include "Global.h"

class MapUtils{
public:
	static MusicInfo loadMap(std::string filename);//加载谱面
	static void closeMap();//关闭谱面
	static void getNoteline();//得到当前难度下的下一行谱面
	static Point getNextPos();//得到下个note的位置
	static void generateMap(const char* songname);//生成谱面
private:
	static void generate(const char* songname);
	static void analyzeBeat();
	static void analyzeBeatV2();
	static void writeNoteline();
	static int genPosX(int posY);
	static int genPosY(int time);
};
#endif 

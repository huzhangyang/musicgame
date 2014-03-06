#ifndef __MAPUTILS_H__
#define __MAPUTILS_H__

#include "AudioEngine.h"
#include "Global.h"

class MapUtils{
public:
	static void generateMap(const char* songname);//生成谱面
	static void loadMap(std::string filename);//加载谱面
	static void closeMap();//关闭谱面
	static int getNoteNumber();//得到当前难度下的总音符数
	static void getNoteline();//得到当前难度下的下一行谱面
	static Point getNextPos();//得到下个note的位置
private:
	static int getBeat();
	static void writeNoteline(int type, int length);
	static int getPosX(int posY, int length);
	static int getPosY(int time);
};
#endif 

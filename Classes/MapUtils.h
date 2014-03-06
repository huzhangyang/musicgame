#ifndef __MAPUTILS_H__
#define __MAPUTILS_H__

#include"AudioEngine.h"

class MapUtils{
public:
	static void generateMap(const char* songname);//生成谱面
	static void loadMap(std::string filename);//加载谱面
	static void getNoteline();//得到下一行谱面
	static int getNextPos();//得到下个note的位置
private:
	static int getBeat();
	static void writeNoteline(int type, int length);
	static int getPosX(int posY, int length);
	static int getPosY(int time);
};

struct Noteline//曲谱结构
{
	int time;//时间
	int difficulty;//难度
	int type;//类型
	int length;//长音符与滑动音符持续长度
	int pos;//出现位置
};
extern Noteline noteline;
#endif 

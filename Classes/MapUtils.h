#ifndef __MAPUTILS_H__
#define __MAPUTILS_H__

#include "AudioEngine.h"
#include "Global.h"

class MapUtils{
public:
	static void generateMap(const char* songname);//��������
	static void loadMap(std::string filename);//��������
	static void getNoteline();//�õ���һ������
	static Point getNextPos();//�õ��¸�note��λ��
private:
	static int getBeat();
	static void writeNoteline(int type, int length);
	static int getPosX(int posY, int length);
	static int getPosY(int time);
};
#endif 

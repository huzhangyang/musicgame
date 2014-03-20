#ifndef __MAPUTILS_H__
#define __MAPUTILS_H__

#include "AudioEngine.h"
#include "Global.h"

class MapUtils{
public:
	static MusicInfo loadMap(std::string filename);//��������
	static void closeMap();//�ر�����
	static void getNoteline();//�õ���ǰ�Ѷ��µ���һ������
	static Point getNextPos();//�õ��¸�note��λ��
	static void generateMap(const char* songname);//��������
private:
	static void generate(const char* songname);
	static void analyzeBeat();
	static void analyzeBeatV2();
	static void writeNoteline();
	static int genPosX(int posY);
	static int genPosY(int time);
};
#endif 

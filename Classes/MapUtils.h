#ifndef __MAPUTILS_H__
#define __MAPUTILS_H__

#include "AudioEngine.h"
#include "Global.h"

class MapUtils{
public:
	static void loadMap(std::string filename);//��������
	static void closeMap();//�ر�����
	static int getNoteNumber();//�õ���ǰ�Ѷ��µ���������
	static void getNoteline();//�õ���ǰ�Ѷ��µ���һ������
	static Point getNextPos();//�õ��¸�note��λ��
	static void generateMap(const char* songname);//��������
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

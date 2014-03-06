#ifndef __MAPUTILS_H__
#define __MAPUTILS_H__

#include "AudioEngine.h"
#include "Global.h"

class MapUtils{
public:
	static void generateMap(const char* songname);//��������
	static void loadMap(std::string filename);//��������
	static void closeMap();//�ر�����
	static int getNoteNumber();//�õ���ǰ�Ѷ��µ���������
	static void getNoteline();//�õ���ǰ�Ѷ��µ���һ������
	static Point getNextPos();//�õ��¸�note��λ��
private:
	static int getBeat();
	static void writeNoteline(int type, int length);
	static int getPosX(int posY, int length);
	static int getPosY(int time);
};
#endif 

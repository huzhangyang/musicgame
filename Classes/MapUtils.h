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
	static void generateMap(std::string name);//��������
private:
	static void generate(std::string name);
	static void writeNoteline();
	static int genPosX(int absY);
	static int genPosY(int time);
};
#endif 

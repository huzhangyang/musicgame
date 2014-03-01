#ifndef __MAPGENERATOR_H__
#define __MAPGENERATOR_H__

#include"AudioEngine.h"


class MapGenerator{
public:
	static void generateMap(const char* songname);
private:
	static int getBeat();
	static void writeNoteline(int type);
	static int getPosX(int posY);
	static int getPosY();
};
#endif 

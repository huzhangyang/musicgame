#ifndef __MAPGENERATOR_H__
#define __MAPGENERATOR_H__

#include"AudioEngine.h"


class MapGenerator{
public:
	static void generateMap(const char* songname);
private:
	static bool hasBeat();
	static void writeNoteline();
};
#endif 

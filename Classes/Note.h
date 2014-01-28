#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"

USING_NS_CC;

class Note : public cocos2d::Sprite
{
public:
	static Note* createAtPoint(Point p, int tag);
	void removeSelf(float dt);
	void update(float dt);
	int getLife();
	void setTouched();
private:
	int type;//0为普通音符，1为长按音符，2为滑动音符
	int life;//剩余帧数
	bool touched;//是否被触摸过
};

#endif 
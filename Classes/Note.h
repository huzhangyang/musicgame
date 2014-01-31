#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"

USING_NS_CC;

class Note : public cocos2d::Sprite
{
public:
	Note();
	virtual ~Note();
	static Note* createNote(int posX, int posY, int type);
	void update(float dt);
	void initNote(int posX, int posY, int type);

	void setLife(int life);
	int getLife();
	int getType();
	void setTouched();
	bool isTouched();
private:
	int type;//0为普通音符，1为长按音符，2为滑动音符
	int life;//剩余帧数
	bool touched;//是否被触摸过
};

#endif 
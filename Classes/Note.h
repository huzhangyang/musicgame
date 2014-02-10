#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"

USING_NS_CC;

class Note : public cocos2d::Sprite
{
public:
	Note();
	virtual ~Note();
	static Note* createNote(int type, int length, int pos, int des);
	void removeNote(float dt);
	void judge();

	int getType();
	int getDestX();
	int getDestY();
	int getLife();
	int getLifeSpan();
	void setTouched();
	bool isTouched();
	bool isInLifeSpan();
private:
	void update(float dt);
	void initNote(int type, int length, int pos, int des);
	int type;//0为普通音符，1为长按音符，2为滑动音符
	int destX, destY;//滑动音符的目的地
	bool touched;//是否被触摸过
	bool inLifeSpan;//是否在生命周期内
	int lifeSpan;//总生命长度
	int life;//剩余生命长度
};

#endif 
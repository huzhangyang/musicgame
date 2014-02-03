#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"

USING_NS_CC;

class Note : public cocos2d::Sprite
{
public:
	Note();
	virtual ~Note();
	static Note* createNote(int type, int pos, int des);
	void removeNote(float dt);

	int getType();
	int getDest();
	int getLife();
	int getLifeSpan();
	int getLifeTouched();
	void setTouched();
	bool isTouched();
	void setNotMissed();
	bool isMissed();
private:
	void update(float dt);
	void initNote(int type, int pos, int des);

	int type;//0为普通音符，1为长按音符，2为滑动音符
	int dest;//滑动音符的目的地
	bool touched;//是否被触摸过，如是则不接受二次触摸事件
	bool missed;//是否被错过
	int lifeSpan;//总生命长度
	int life;//剩余生命长度
	int lifeTouched;//保持触摸状态的长度
};

#endif 
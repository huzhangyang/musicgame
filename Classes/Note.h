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
	void removeNote(float dt);

	int getType();
	int getLife();
	int getLifeSpan();
	int getLifeTouched();
	void setTouched();
	bool isTouched();
	void setTouchEnded();
	bool isTouchEnded();
private:
	void update(float dt);
	void initNote(int posX, int posY, int type);
	
	int type;//0为普通音符，1为长按音符，2为滑动音符
	bool touched;//是否被触摸过
	bool touchEnded;//是否触摸已结束
	int lifeSpan;//总生命长度
	int life;//剩余生命长度
	int lifeTouched;//保持触摸状态的长度
};

#endif 
#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"

USING_NS_CC;

enum NoteStatus{ UNTOUCHED_PRELOAD = 0, UNTOUCHED_DEADLINE = 1, TOUCHED_UNACTIVATED = 2, TOUCHED_ACTIVATED = 3 };//音符状态枚举

class Note : public cocos2d::Sprite
{
public:
	Note();
	virtual ~Note();
	static Note* createNote(int type, int length, int pos, int des);
	void removeNote(float dt);
	void judge();

	int getType();
	int getLength();
	int getDestX();
	int getDestY();
	int getLife();
	NoteStatus getStatus();
	void setStatus(NoteStatus status);
	void setLife(int life);
private:
	void update(float dt);
	void initNote(int type, int length, int pos, int des);
	int type;//0为普通音符，1为长按音符，2为滑动音符
	int length;//总生命长度
	int destX, destY;//滑动音符的目的地
	int life;//剩余生命长度
	NoteStatus status;//当前状态

};

#endif 
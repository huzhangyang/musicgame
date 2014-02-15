#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"

USING_NS_CC;

enum NoteType{CLICK=0,LONGPRESS=1,SLIDE=2};//音符类型枚举
enum NoteStatus{ UNTOUCHED_PRELOAD = 0, UNTOUCHED_DEADLINE = 1, TOUCHED_UNACTIVATED = 2, TOUCHED_ACTIVATED = 3 };//音符状态枚举

class Note : public cocos2d::Sprite
{
public:
	Note();
	virtual ~Note();
	static Note* createNote(int type, int length, int pos, int des);
	void removeNote();
	void judge();

	int getLength();
	int getDestX();
	int getDestY();
	int getLife();
	NoteType getType();
	NoteStatus getStatus();
	void setStatus(NoteStatus status);
	void setLife(int life);
	void setTouchPoint(Point point);
private:
	void update(float dt);
	void initNote(int type, int length, int pos, int des);
	NoteType type;//音符类型
	NoteStatus status;//当前状态
	Point touchpoint;//触摸点
	int length;//总生命长度
	int destX, destY;//滑动音符的目的地
	int life;//剩余生命长度
	

};

#endif 
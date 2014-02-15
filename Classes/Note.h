#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"

USING_NS_CC;

enum NoteType{ CLICK = 0, LONGPRESS = 1, SLIDE = 2 };//音符类型枚举
enum NoteStatus{ UNTOUCHED = 0, TOUCHED_UNACTIVATED = 1, TOUCHED_ACTIVATED = 2 };//音符状态枚举

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
	void setTouchPoint(Point point);
	void setLifeTouchBegan(int lifeTouchBegan);
private:
	void update(float dt);
	void initNote(int type, int length, int pos, int des);
	NoteType type;//音符类型
	NoteStatus status;//当前状态
	Point touchpoint;//触摸点
	int length;//总生命长度
	int destX, destY;//滑动音符的目的地
	int life;//剩余生命长度
	int lifeTouchBegan;//触摸开始时的生命值
};

#endif 
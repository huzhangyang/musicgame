#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"

USING_NS_CC;

enum NoteType{ CLICK = 0, LONGPRESS = 1, SLIDE = 2 };//音符类型枚举

class Note : public Sprite
{
public:
	Note();
	virtual ~Note();
	static Note* createNote(int type, int length, int pos);
private:
	void initNote(int type, int length, int pos);
	void removeNote();
	void update(float dt);
	void judge(float slideAngle = 0);
	void createNoteListener();
	void addToNoteListener();
	NoteType type;//音符类型
	Sprite *judgePic;//评级
	bool isTouched;//是否被触摸
	bool isActivated;//是否开始生命周期
	bool isSlided;//滑动音符是否被滑过
	int length;//总生命长度
	int life;//剩余生命长度
	int lifeTouchBegan;//触摸开始时的生命值
};

#endif 
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
	static Note* createNote(int type, int length, int pos, int des);
private:
	void initNote(int type, int length, int pos, int des);
	void removeNote();
	void update(float dt);
	void judge(float slideAngle = 0);
	bool onTouchBegan(Touch *pTouch, Event  *pEvent);
	void onTouchMoved(Touch *pTouch, Event  *pEvent);
	void onTouchEnded(Touch *pTouch, Event  *pEvent);
	NoteType type;//音符类型
	Sprite *judgePic;//评级
	EventListenerTouchOneByOne *noteListener;//事件监听器
	bool isTouched;//是否被触摸
	bool isActivated;//是否开始生命周期
	bool isSlided;
	int length;//总生命长度
	int desX, desY;//滑动音符的目的地
	int life;//剩余生命长度
	int lifeTouchBegan;//触摸开始时的生命值
};

#endif 
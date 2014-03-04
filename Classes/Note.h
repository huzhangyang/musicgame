#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"

USING_NS_CC;

enum NoteType{ CLICK = 0, LONGPRESS = 1, SLIDE = 2 };//��������ö��

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
	NoteType type;//��������
	Sprite *judgePic;//����
	EventListenerTouchOneByOne *noteListener;//�¼�������
	bool isTouched;//�Ƿ񱻴���
	bool isActivated;//�Ƿ�ʼ��������
	bool isSlided;
	int length;//����������
	int desX, desY;//����������Ŀ�ĵ�
	int life;//ʣ����������
	int lifeTouchBegan;//������ʼʱ������ֵ
};

#endif 
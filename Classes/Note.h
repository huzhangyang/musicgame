#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"

USING_NS_CC;

enum NoteType{ CLICK = 0, LONGPRESS = 1, SLIDE = 2 };//��������ö��
enum NoteStatus{ UNTOUCHED_UNACTIVATED = 0, UNTOUCHED_ACTIVATED = 1, TOUCHED_UNACTIVATED = 2, TOUCHED_ACTIVATED = 3 };//����״̬ö��

class Note : public Sprite
{
public:
	Note();
	virtual ~Note();
	static Note* createNote(int type, int length, int pos, int des);
	void removeNote();
	void judge();

	int getLength();
	int getDesX();
	int getDesY();
	int getLife();
	NoteType getType();
	NoteStatus getStatus();
	void setStatus(NoteStatus status);
	void setTouchPoint(Point point);
	void setLifeTouchBegan(int lifeTouchBegan);
private:
	bool onTouchBegan(Touch *pTouch, Event  *pEvent);
	void onTouchMoved(Touch *pTouch, Event  *pEvent);
	void onTouchEnded(Touch *pTouch, Event  *pEvent);
	void update(float dt);
	void initNote(int type, int length, int pos, int des);
	NoteType type;//��������
	NoteStatus status;//��ǰ״̬
	Point touchpoint;//������
	Sprite *judgePic;//����
	int length;//����������
	int desX, desY;//����������Ŀ�ĵ�
	int life;//ʣ����������
	int lifeTouchBegan;//������ʼʱ������ֵ
};

#endif 
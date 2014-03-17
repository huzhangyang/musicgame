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
	static Note* createNote(int type, int length, int posX, int posY);
private:
	void initNote(int type, int length, int posX, int posY);
	void removeNote();
	void update(float dt);
	void judge();
	void createNoteListener();
	void addToNoteListener();
	NoteType type;//��������
	Sprite *judgePic;//����
	bool isTouched;//�Ƿ񱻴���
	bool isActivated;//�Ƿ�ʼ��������
	bool isSlided;//���������Ƿ񱻻���
	int length;//����������
	int life;//ʣ����������
	int lifeTouchBegan;//������ʼʱ������ֵ
	float slideAngle;
	float slideDistance;
};

#endif 
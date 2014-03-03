#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "CocosGUI.h"  
#include "cocostudio/CocoStudio.h"   

#include "Global.h"
#include "AudioEngine.h" 

USING_NS_CC;
using namespace gui;

class GameScene : public Layer
{
public:
	static Scene* createScene(std::string filename);
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExitTransitionDidStart();
	void menuCloseCallback(Object* pSender);
	CREATE_FUNC(GameScene);
	static void judgeNote(int judgeResult);
private:
	void update(float dt);
	void startGame(float dt);//��ʼ��Ϸ
	void addNewNote(int type, int length, int pos, int des);//������note
	void getNoteline();//�õ��µ�һ������
	void touchEvent(Object* obj, TouchEventType eventType);
	Text *labelInfo;
	LoadingBar *loadingBar;
	ImageView *bgPause, *labelDifficulty;
	Node *UINode, *PauseNode;
	Button *buttonRetry, *buttonResume, *buttonReturn, *buttonOption;
};

struct Noteline//���׽ṹ
{
	int time;//ʱ��
	int difficulty;//�Ѷ�
	int type;//����
	int length;//�������뻬��������������
	int pos;//����λ��
	int des;//��������Ŀ��λ��
};

#endif 

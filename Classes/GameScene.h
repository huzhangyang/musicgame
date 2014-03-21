#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h" 
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"   

#include "Global.h"
#include "AudioEngine.h" 

USING_NS_CC;
using namespace ui;

class GameScene : public Layer
{
public:
	static Scene* createScene(std::string filename);
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExitTransitionDidStart();
	void menuCloseCallback(Ref* pSender);
	CREATE_FUNC(GameScene);
	static void judgeNote(int judgeResult);
private:
	void update(float dt);
	void startGame(float dt);//��ʼ��Ϸ
	void addNewNote(int type, int length, int posX, int posY);//������note
	void addScanline();//���ɨ����
	void setScanline();//����ɨ����λ��
	void touchEvent(Ref* obj, TouchEventType eventType);
	void checkboxEvent(Ref* obj, CheckBoxEventType eventType);
	void sliderEvent(Ref* obj, SliderEventType eventType);
	Sprite* scanline;
};

#endif 

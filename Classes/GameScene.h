#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h" 
#include "cocostudio/CocoStudio.h"  
#include "CocosGUI.h"  

USING_NS_CC;
using namespace gui;

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	void menuCloseCallback(Object* pSender);
	CREATE_FUNC(GameScene);
	static void judgeNote(int judge);
private:
	void update(float dt);
	void addNewNote(int type, int length, int pos, int des);//生成新note
	void addArrow(int posX, int posY, int desX, int desY);//为滑动note生成箭头和目标点
	void addRandomNote(int type);//随机生成note
	bool onTouchBegan(Touch *pTouch, Event  *pEvent);
	void onTouchMoved(Touch *pTouch, Event  *pEvent);
	void onTouchEnded(Touch *pTouch, Event  *pEvent);
	void touchEvent(Object* obj, gui::TouchEventType eventType);
};

#endif 

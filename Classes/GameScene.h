#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "Resources.h"
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
	bool onTouchBegan(Touch *pTouch, Event  *pEvent);
	void menuCloseCallback(Object* pSender);
	CREATE_FUNC(GameScene);
	void addNewNote(int posX, int posY, int type);
	void addRandomNote(float dt);
	static void setCondition(int cond);
	void touchEvent(Object* obj, gui::TouchEventType eventType);
private:

};

#endif 

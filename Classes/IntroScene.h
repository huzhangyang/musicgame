#ifndef __INTRO_SCENE_H__
#define __INTRO_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h" 
#include "cocostudio/CocoStudio.h"  
#include "CocosGUI.h" 

USING_NS_CC;
using namespace gui;

class IntroScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void onExit();
	void menuCloseCallback(Object* pSender);
	CREATE_FUNC(IntroScene);
private:
	void playIntro(float dt);
	void touchEvent(Object* obj, gui::TouchEventType eventType);
};

#endif 

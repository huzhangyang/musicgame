#ifndef __INTRO_SCENE_H__
#define __INTRO_SCENE_H__

#include "cocos2d.h"
#include "CocosGUI.h"  
#include "cocostudio/CocoStudio.h"   

#include "Global.h"
#include "AudioEngine.h" 

USING_NS_CC;
using namespace gui;

class IntroScene : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	virtual void onExitTransitionDidStart();
	void menuCloseCallback(Object* pSender);
	CREATE_FUNC(IntroScene);
private:
	void playIntro();
	void touchEvent(Object* obj, TouchEventType eventType);
};

#endif 

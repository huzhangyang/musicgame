#ifndef __INTRO_SCENE_H__
#define __INTRO_SCENE_H__

#include "cocos2d.h" 
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"   

#include "Global.h"
#include "AudioEngine.h" 

USING_NS_CC;
using namespace ui;

class IntroScene : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	virtual void onExitTransitionDidStart();
	void menuCloseCallback(Ref* pSender);
	CREATE_FUNC(IntroScene);
private:
	void playIntro();
	void touchEvent(Ref* obj, TouchEventType eventType);
};

#endif 

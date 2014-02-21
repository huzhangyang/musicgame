#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "CocosGUI.h"  
#include "cocostudio/CocoStudio.h"  
#include "cocos2d.h"
#include "AudioEngine.h" 

USING_NS_CC;
using namespace gui;

class MainScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	void menuCloseCallback(Object* pSender);
	CREATE_FUNC(MainScene);
private:
	void touchEvent(Object* obj, TouchEventType eventType);
};

#endif 

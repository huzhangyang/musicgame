#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "Resources.h"
#include "CocosGUI.h"  
#include "cocostudio/CocoStudio.h"  
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h" 

USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;

class MainScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	void menuCloseCallback(Object* pSender);
	CREATE_FUNC(MainScene);
	void touchEvent(Object* obj, gui::TouchEventType eventType);
private:
};

#endif 

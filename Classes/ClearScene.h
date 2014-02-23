#ifndef __CLEAR_SCENE_H__
#define __CLEAR_SCENE_H__

#include "cocos2d.h"
#include "CocosGUI.h"  
#include "cocostudio/CocoStudio.h"   

#include "Global.h"
#include "AudioEngine.h" 

USING_NS_CC;
using namespace gui;

class ClearScene : public cocos2d::Layer
{
public:
	static Scene* createScene(std::string filename);
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExitTransitionDidStart();
	void menuCloseCallback(Object* pSender);
	CREATE_FUNC(ClearScene);
private:
	void touchEvent(Object* obj, TouchEventType eventType);
};

#endif 

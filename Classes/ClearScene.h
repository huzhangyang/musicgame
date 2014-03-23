#ifndef __CLEAR_SCENE_H__
#define __CLEAR_SCENE_H__

#include "cocos2d.h" 
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"   

#include "Global.h"
#include "AudioEngine.h" 

USING_NS_CC;
using namespace ui;

class ClearScene : public cocos2d::Layer
{
public:
	static Scene* createScene(std::string filename, std::string level);
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExitTransitionDidStart();
	void menuCloseCallback(Ref* pSender);
	CREATE_FUNC(ClearScene);
private:
	void touchEvent(Ref* obj, TouchEventType eventType);
	Node* UINode;
};

#endif 

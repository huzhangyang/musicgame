#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"   

#include "Global.h"
#include "AudioEngine.h" 

USING_NS_CC;
using namespace ui;

class MainScene : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExitTransitionDidStart();
	static void loadingEnd();
	void menuCloseCallback(Ref* pSender);
	CREATE_FUNC(MainScene);
private:
	void touchEvent(Ref* obj, TouchEventType eventType);
	void checkboxEvent(Ref* obj, CheckBoxEventType eventType);
	void sliderEvent(Ref* obj, SliderEventType eventType);
	void createDialog(std::string key);
};

#endif 

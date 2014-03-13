#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "CocosGUI.h"  
#include "cocostudio/CocoStudio.h"   

#include "Global.h"
#include "AudioEngine.h" 

USING_NS_CC;
using namespace gui;

class MainScene : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExitTransitionDidStart();
	void menuCloseCallback(Object* pSender);
	CREATE_FUNC(MainScene);
private:
	void touchEvent(Object* obj, TouchEventType eventType);
	void createDialog(std::string key);
	Node *UINode, *ExitNode, *DialogNode, *LoadingNode;
	Text *labelWord;
	ImageView *objectBox, *objectDialog, *objectWords, *objectLight, *objectBG;
	Button *buttonCheck, *buttonCross;
};

#endif 

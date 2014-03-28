#ifndef __SELECT_SCENE_H__
#define __SELECT_SCENE_H__

#include "cocos2d.h" 
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"   

#include "Global.h"
#include "AudioEngine.h" 

USING_NS_CC;
using namespace ui;

class SelectScene : public Layer
{
public:
	static Scene* createScene(int mode);//0为制谱模式，1为选曲模式
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExitTransitionDidStart();
	void menuCloseCallback(Ref* pSender);
	static void loadingEnd();
	CREATE_FUNC(SelectScene);
private:
	void touchEvent(Ref* obj, TouchEventType eventType);
	void listViewEvent(Ref* obj, ListViewEventType eventType);
	Node *UINode;
};

#endif 

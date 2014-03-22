#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h" 
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"   

#include "Global.h"
#include "AudioEngine.h" 

USING_NS_CC;
using namespace ui;

class GameScene : public Layer
{
public:
	static Scene* createScene(std::string filename);
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExitTransitionDidStart();
	void menuCloseCallback(Ref* pSender);
	CREATE_FUNC(GameScene);
	static void judgeNote(int judgeResult);
private:
	void update(float dt);
	void startGame(float dt);//开始游戏
	void addNewNote(int type, int length, int posX, int posY);//生成新note
	void addScanline();//添加扫描线
	void setScanline(int time);//设置扫描线位置
	void touchEvent(Ref* obj, TouchEventType eventType);
	Sprite* scanline;
};

#endif 

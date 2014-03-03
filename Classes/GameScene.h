#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "CocosGUI.h"  
#include "cocostudio/CocoStudio.h"   

#include "Global.h"
#include "AudioEngine.h" 

USING_NS_CC;
using namespace gui;

class GameScene : public Layer
{
public:
	static Scene* createScene(std::string filename);
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExitTransitionDidStart();
	void menuCloseCallback(Object* pSender);
	CREATE_FUNC(GameScene);
	static void judgeNote(int judgeResult);
private:
	void update(float dt);
	void startGame(float dt);//开始游戏
	void addNewNote(int type, int length, int pos, int des);//生成新note
	void getNoteline();//得到新的一行谱面
	void touchEvent(Object* obj, TouchEventType eventType);
	Text *labelInfo;
	LoadingBar *loadingBar;
	ImageView *bgPause, *labelDifficulty;
	Node *UINode, *PauseNode;
	Button *buttonRetry, *buttonResume, *buttonReturn, *buttonOption;
};

struct Noteline//曲谱结构
{
	int time;//时间
	int difficulty;//难度
	int type;//类型
	int length;//长音符与滑动音符持续长度
	int pos;//出现位置
	int des;//滑动音符目标位置
};

#endif 

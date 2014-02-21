#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "AudioEngine.h" 
#include "cocostudio/CocoStudio.h"  
#include "CocosGUI.h"  

USING_NS_CC;
using namespace gui;

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	void menuCloseCallback(Object* pSender);
	CREATE_FUNC(GameScene);
	static void judgeNote(int judge);
private:
	void update(float dt);
	void addNewNote(int type, int length, int pos, int des);//生成新note
	void addRandomNote(float dt);//随机生成note
	void addArrow(int posX, int posY, int desX, int desY);//为滑动note生成箭头和目标点
	void getNoteline();//得到新的一行谱面
	void startGame();//开始游戏
	void touchEvent(Object* obj, TouchEventType eventType);
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

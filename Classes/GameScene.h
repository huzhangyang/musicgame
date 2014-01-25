#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h" 
#include "cocostudio/CocoStudio.h"  

USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	void menuCloseCallback(Object* pSender);
	CREATE_FUNC(GameScene);
	void addNewNote(Point p);
	void GameScene::addRandomNote(float dt);
	void removeNote(float dt);
private:
};

#endif 

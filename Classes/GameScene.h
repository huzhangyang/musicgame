#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void menuCloseCallback(Object* pSender);
	CREATE_FUNC(GameScene);
	void addNewSpriteAtPosition(Point p);
	virtual void onEnter() override;
	virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)override;
private:
};

#endif 

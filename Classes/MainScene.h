#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class MainScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void menuCloseCallback(Object* pSender);
	CREATE_FUNC(MainScene);// implement the "static create()" method manually
	virtual void onEnter() override;
	void setPhyWorld(PhysicsWorld* world){ m_world = world; }
	virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)override;
	void addNewSpriteAtPosition(Point p);
private:
	PhysicsWorld* m_world;
};

#endif 

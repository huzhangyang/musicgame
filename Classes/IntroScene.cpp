#include "IntroScene.h"
#include "MainScene.h"
#include "cocostudio/CocoStudio.h"  

USING_NS_CC;
USING_NS_CC_EXT;

Scene* IntroScene::createScene()
{
	auto scene = Scene::create();
	auto layer = IntroScene::create();
	scene->addChild(layer);
	return scene;
}

bool IntroScene::init()
{
	//super init
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////////////////////////////
	auto sprite = Sprite::create("intro.jpg");
	sprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	addChild(sprite);
	sprite->runAction(Sequence::create(DelayTime::create(3), CallFunc::create(this, callfunc_selector(IntroScene::toggleStart)),NULL));
	return true;
}

void IntroScene::toggleStart()
{
	auto scene = MainScene::createScene();
	CCDirector::sharedDirector()->replaceScene(TransitionFade::create(1, scene));
}

void IntroScene::menuCloseCallback(Object* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
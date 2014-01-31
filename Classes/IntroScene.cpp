#include "IntroScene.h"
#include "MainScene.h"

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
	auto sprite = Sprite::create("intro.png");
	sprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	addChild(sprite);
	sprite->scheduleOnce(schedule_selector(IntroScene::gotoMainScene), 2);
	return true;
}

void IntroScene::gotoMainScene(float dt)
{
	auto scene = MainScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
}

void IntroScene::menuCloseCallback(Object* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
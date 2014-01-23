#include "GameScene.h"

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

void GameScene::addNewSpriteAtPosition(Point p)
{
	auto sp = Sprite::create("1.png");
	sp->setTag(1);
	sp->setPosition(p);
	this->addChild(sp);
}

void GameScene::onTouchesEnded(const std::vector<Touch*>& touches, Event *event)
{
	for (auto touch : touches)
	{
		auto location = touch->getLocation();
		addNewSpriteAtPosition(location);
	}
}

void GameScene::onEnter()
{
	Layer::onEnter();
}

bool GameScene::init()
{
	//super init
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////////////////////////////

	return true;
}

void GameScene::menuCloseCallback(Object* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
#include "GameScene.h"

static int notecount;

const int POS_X1 = 270;
const int POS_X2 = 420;
const int POS_X3 = 570;
const int POS_X4 = 720;
const int POS_X5 = 870;
const int POS_X6 = 1020;
const int POS_Y1 = 105;
const int POS_Y2 = 200;
const int POS_Y3 = 295;
const int POS_Y4 = 390;
const int POS_Y5 = 485;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);

	notecount = 0;//¼ÇÂ¼Òô·ûÊý

	return scene;
}

void GameScene::addNewNote(Point p)
{
	auto note = Sprite::create("1.png");
	note->setTag(++notecount);
	note->setPosition(p);
	this->addChild(note);
	note->runAction(DelayTime::create(1));
	//note->removeFromParent();
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
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("test.mp3");
	auto sprite = Sprite::create("gameSceneBackground.png");
	sprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	addChild(sprite);
	return true;
}

void GameScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("test.mp3");
	addNewNote(Point(POS_X1, POS_Y1));
}

void GameScene::menuCloseCallback(Object* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
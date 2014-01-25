#include "GameScene.h"

static int notecount;

const int POS_X1 = 260;
const int POS_X2 = 410;
const int POS_X3 = 560;
const int POS_X4 = 710;
const int POS_X5 = 860;
const int POS_X6 = 1010;
const int POS_Y1 = 470;
const int POS_Y2 = 380;
const int POS_Y3 = 290;
const int POS_Y4 = 200;
const int POS_Y5 = 110;

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
	auto note = Sprite::create("note.png");
	note->setZOrder(1);
	note->setTag(++notecount);
	note->setPosition(p);
	note->scheduleOnce(schedule_selector(GameScene::removeNote), 0.8);
	addChild(note);
}

void GameScene::addRandomNote(float dt)
{
	if (!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		this->unschedule(schedule_selector(GameScene::addRandomNote));
	}
	int randomX = CCRANDOM_0_1() * 5 + 1;
	int randomY = CCRANDOM_0_1() * 5 + 1;
	switch (10 * randomX + randomY)
	{
	case 11:addNewNote(Point(POS_X1, POS_Y1)); break;
	case 12:addNewNote(Point(POS_X1, POS_Y2)); break;
	case 13:addNewNote(Point(POS_X1, POS_Y3)); break;
	case 14:addNewNote(Point(POS_X1, POS_Y4)); break;
	case 15:addNewNote(Point(POS_X1, POS_Y5)); break;
	case 21:addNewNote(Point(POS_X2, POS_Y1)); break;
	case 22:addNewNote(Point(POS_X2, POS_Y2)); break;
	case 23:addNewNote(Point(POS_X2, POS_Y3)); break;
	case 24:addNewNote(Point(POS_X2, POS_Y4)); break;
	case 25:addNewNote(Point(POS_X2, POS_Y5)); break;
	case 31:addNewNote(Point(POS_X3, POS_Y1)); break;
	case 32:addNewNote(Point(POS_X3, POS_Y2)); break;
	case 33:addNewNote(Point(POS_X3, POS_Y3)); break;
	case 34:addNewNote(Point(POS_X3, POS_Y4)); break;
	case 35:addNewNote(Point(POS_X3, POS_Y5)); break;
	case 41:addNewNote(Point(POS_X4, POS_Y1)); break;
	case 42:addNewNote(Point(POS_X4, POS_Y2)); break;
	case 43:addNewNote(Point(POS_X4, POS_Y3)); break;
	case 44:addNewNote(Point(POS_X4, POS_Y4)); break;
	case 45:addNewNote(Point(POS_X4, POS_Y5)); break;
	case 51:addNewNote(Point(POS_X5, POS_Y1)); break;
	case 52:addNewNote(Point(POS_X5, POS_Y2)); break;
	case 53:addNewNote(Point(POS_X5, POS_Y3)); break;
	case 54:addNewNote(Point(POS_X5, POS_Y4)); break;
	case 55:addNewNote(Point(POS_X5, POS_Y5)); break;
	case 61:addNewNote(Point(POS_X6, POS_Y1)); break;
	case 62:addNewNote(Point(POS_X6, POS_Y2)); break;
	case 63:addNewNote(Point(POS_X6, POS_Y3)); break;
	case 64:addNewNote(Point(POS_X6, POS_Y4)); break;
	case 65:addNewNote(Point(POS_X6, POS_Y5)); break;
	}
}

void GameScene::removeNote(float dt)
{
	this->removeFromParentAndCleanup(true);
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
	auto sceneNode = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("gameScene.json");
	sceneNode->setZOrder(0);
	addChild(sceneNode);
	auto child = sceneNode->getChildByTag(10004);
	auto reader = (cocostudio::ComRender*)child->getComponent("GUIComponent");
	auto layer = (Layer*)reader->getNode();
	return true;
}

void GameScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	/////////////////////////////////////////////////////
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("test.mp3");
	this->schedule(schedule_selector(GameScene::addRandomNote), 0.6);
}

void GameScene::menuCloseCallback(Object* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
#include "MainScene.h"
#include "GameScene.h"

const std::string FILENAME = "test";//曲子文件名
Node* ExitNode;

Scene* MainScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainScene::create();
	scene->addChild(layer);
	return scene;
}

bool MainScene::init()
{
	//super init
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->createLoop("music/main.mp3");
	auto sceneNode = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("mainScene.json");
	addChild(sceneNode);

	auto UINode = sceneNode->getChildByTag(10005);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("mainSceneUI");
	auto UIlayer = UIComponent->getNode();
	auto objectTable = dynamic_cast<Button*>(UIlayer->getChildByTag(MAINSCENE_TABLE));
	objectTable->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto objectPaper = dynamic_cast<Button*>(UIlayer->getChildByTag(MAINSCENE_PAPER));
	objectPaper->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto objectShelf = dynamic_cast<Button*>(UIlayer->getChildByTag(MAINSCENE_SHELF));
	objectShelf->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto objectClock = dynamic_cast<Button*>(UIlayer->getChildByTag(MAINSCENE_CLOCK));
	objectClock->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto objectCat = dynamic_cast<Button*>(UIlayer->getChildByTag(MAINSCENE_CAT));
	objectCat->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto buttonOption = dynamic_cast<Button*>(UIlayer->getChildByTag(MAINSCENE_OPTION));
	buttonOption->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto buttonHelp = dynamic_cast<Button*>(UIlayer->getChildByTag(MAINSCENE_HELP));
	buttonHelp->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto buttonExit = dynamic_cast<Button*>(UIlayer->getChildByTag(MAINSCENE_EXIT));
	buttonExit->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));

	ExitNode = sceneNode->getChildByTag(10004);
	auto ExitComponent = (cocostudio::ComRender*) ExitNode->getComponent("exitSelectUI");
	auto Exitlayer = ExitComponent->getNode();
	auto buttonCheck = dynamic_cast<Button*>(Exitlayer->getChildByTag(MAINSCENE_CHECKMARK));
	buttonCheck->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto buttonCross = dynamic_cast<Button*>(Exitlayer->getChildByTag(MAINSCENE_CROSSMARK));
	buttonCross->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));

	return true;
}

void MainScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->play();
}

void MainScene::onExitTransitionDidStart()
{
	Layer::onExitTransitionDidStart();
	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->stop();
}

void MainScene::menuCloseCallback(Object* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void MainScene::touchEvent(Object* obj, gui::TouchEventType eventType)
{
	auto button = dynamic_cast<Button*>(obj);
	int tag = button->getTag();
	switch (eventType)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		if (tag == MAINSCENE_TABLE)
		{
		}
		else if (tag == MAINSCENE_PAPER)
		{
		}
		else if (tag == MAINSCENE_SHELF)
		{
			auto scene = GameScene::createScene(FILENAME);
			Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
		}
		else if (tag == MAINSCENE_CLOCK)
		{
		}
		else if (tag == MAINSCENE_CAT)
		{
		}
		else if (tag == MAINSCENE_OPTION)
		{
			UserDefault::getInstance()->setIntegerForKey("difficulty", 0);
		}
		else if (tag == MAINSCENE_HELP)
		{
		}
		else if (tag == MAINSCENE_EXIT)
		{
			ExitNode->setVisible(true);
		}
		else if (tag == MAINSCENE_CHECKMARK)
		{
			Director::getInstance()->popScene();
		}
		else if (tag == MAINSCENE_CROSSMARK)
		{
			ExitNode->setVisible(false);
		}
		break;
	}
}


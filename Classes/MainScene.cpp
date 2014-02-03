#include "Global.h"
#include "MainScene.h"
#include "GameScene.h"

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
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/main.mp3");
	auto sceneNode = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("mainScene.json");
	addChild(sceneNode);
	auto UINode = sceneNode->getChildByTag(10004);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("GUIComponent");
	auto UIlayer = UIComponent->getNode();
	auto objectShelf = dynamic_cast<Button*>(UIlayer->getChildByTag(MAINSCENE_SHELF));
	objectShelf->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto objectInk = dynamic_cast<Button*>(UIlayer->getChildByTag(MAINSCENE_INK));
	objectInk->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto buttonOption = dynamic_cast<Button*>(UIlayer->getChildByTag(MAINSCENE_OPTION));
	buttonOption->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto buttonHelp = dynamic_cast<Button*>(UIlayer->getChildByTag(MAINSCENE_HELP));
	buttonHelp->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto buttonExit = dynamic_cast<Button*>(UIlayer->getChildByTag(MAINSCENE_EXIT));
	buttonExit->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	return true;
}

void MainScene::onEnterTransitionDidFinish()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/main.mp3");
}

void MainScene::onExit()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
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
		if (tag == MAINSCENE_SHELF)
		{
			auto scene = GameScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
		}
		else if (tag == MAINSCENE_INK)
		{
		}
		else if (tag == MAINSCENE_OPTION)
		{
		}
		else if (tag == MAINSCENE_HELP)
		{
		}
		else if (tag == MAINSCENE_EXIT)
		{
			Director::getInstance()->popScene();
		}
		break;
	}
}


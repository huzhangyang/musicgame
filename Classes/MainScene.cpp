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
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bg.mp3");
	auto sceneNode = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("mainScene.json");
	addChild(sceneNode);
	auto child = sceneNode->getChildByTag(10004);
	auto reader = (cocostudio::ComRender*)child->getComponent("GUIComponent");
	auto layer = (Layer*)reader->getNode();
	auto shelf = dynamic_cast<Button*>(layer->getChildByTag(MAINSCENE_SHELF));
	shelf->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto ink = dynamic_cast<Button*>(layer->getChildByTag(MAINSCENE_INK));
	ink->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto option = dynamic_cast<Button*>(layer->getChildByTag(MAINSCENE_OPTION));
	option->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto help = dynamic_cast<Button*>(layer->getChildByTag(MAINSCENE_HELP));
	help->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto exit = dynamic_cast<Button*>(layer->getChildByTag(MAINSCENE_EXIT));
	exit->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	return true;
}

void MainScene::onEnterTransitionDidFinish()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bg.mp3");
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
		else if(tag == MAINSCENE_OPTION)
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


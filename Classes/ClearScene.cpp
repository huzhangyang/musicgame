#include "ClearScene.h"
#include "MainScene.h"
#include "GameScene.h"

Scene* ClearScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainScene::create();
	scene->addChild(layer);
	return scene;
}

bool ClearScene::init()
{
	//super init
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////////////////////////////
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/clear.mp3");
	auto sceneNode = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("clearScene.json");
	addChild(sceneNode);
	auto UINode = sceneNode->getChildByTag(10004);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("GUIComponent");
	auto UIlayer = UIComponent->getNode();
	return true;
}

void ClearScene::onEnterTransitionDidFinish()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/clear.mp3");
}

void ClearScene::onExit()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
}

void ClearScene::menuCloseCallback(Object* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void ClearScene::touchEvent(Object* obj, gui::TouchEventType eventType)
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
		}
		break;
	}
}


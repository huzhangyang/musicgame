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
	auto mainScene = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("draftm.json");
	addChild(mainScene);

	auto shelf = dynamic_cast<Button*>(mainScene->getChildByTag(MAINSCENE_SHELF));
	shelf->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	return true;
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
			Director::getInstance()->replaceScene(TransitionCrossFade::create(2, scene));
		}
	}
}


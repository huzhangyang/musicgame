#include "MainScene.h"
#include "cocostudio/CocoStudio.h"  

USING_NS_CC;
USING_NS_CC_EXT;

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
	auto node =cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("draftm.json");
	//auto node = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("mainscene/practice1_1/practice1_1.json");
	addChild(node);
	return true;
}

void MainScene::menuCloseCallback(Object* pSender)
{
	Director::getInstance()->end();

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
	#endif
}
#include "SelectScene.h"
#include "MainScene.h"
#include "GameScene.h"

Scene* SelectScene::createScene()
{
	auto scene = Scene::create();
	auto layer = SelectScene::create();
	scene->addChild(layer);
	return scene;
}

bool SelectScene::init()
{
	//super init
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////////////////////////////
	auto sceneNode = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("selectScene.json");
	addChild(sceneNode);
	auto UINode = sceneNode->getChildByTag(10003);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("selectScene");
	auto UILayer = UIComponent->getNode();
	auto list = dynamic_cast<ListView*>(UILayer->getChildByTag(SELECTSCENE_LIST));
	auto info = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_LIST)->getChildByTag(SELECTSCENE_INFO));
	auto level = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_LEVEL));
	auto difficulty = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_DIFFICULTY));
	auto score = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_SCORE));
	//////////
	list->setItemModel(info);
	list->addEventListenerListView(this, listvieweventselector(SelectScene::listViewEvent));
	((Text*)list->getItem(0))->setText(FILENAME);
	list->pushBackDefaultItem();
	((Text*)list->getItem(1))->setText("YOYOYO");
	list->pushBackDefaultItem();
	((Text*)list->getItem(2))->setText("HAHAHA");
	list->pushBackDefaultItem();
	((Text*)list->getItem(3))->setText("HEYHEY");
	list->pushBackDefaultItem();
	((Text*)list->getItem(4))->setText("LYC");
	return true;
}

void SelectScene::onExitTransitionDidStart()
{
	Layer::onExitTransitionDidStart();
	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->stop();
}

void SelectScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void SelectScene::touchEvent(Ref* obj, TouchEventType eventType)
{
	auto widget = dynamic_cast<Widget*>(obj);
	int tag = widget->getTag();
	switch (eventType)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		break;
	}
}

void SelectScene::listViewEvent(Ref* obj, ListViewEventType eventType)
{
	auto list = dynamic_cast<ListView*>(obj);
	auto index = list->getCurSelectedIndex();
	auto info = dynamic_cast<Text*>(list->getItem(index));
	std::string musicname = "music/" + FILENAME + ".mp3";
	std::string mapname = FileUtils::getInstance()->getWritablePath() + FILENAME + ".gnm";
	Scene* scene;
	switch (eventType)
	{
	case ListViewEventType::LISTVIEW_ONSELECTEDITEM_START:
		break;
	case ListViewEventType::LISTVIEW_ONSELECTEDITEM_END:
		scene = GameScene::createScene(info->getStringValue());
		Director::getInstance()->replaceScene(TransitionPageTurn::create(2, scene, true));
		break;
	}
}
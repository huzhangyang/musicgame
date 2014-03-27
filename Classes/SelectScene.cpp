#include "SelectScene.h"
#include "MainScene.h"
#include "GameScene.h"

int selectMode;

Scene* SelectScene::createScene(int mode)
{
	auto scene = Scene::create();
	auto layer = SelectScene::create();
	scene->addChild(layer);
	selectMode = mode;
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
	list = dynamic_cast<ListView*>(UILayer->getChildByTag(SELECTSCENE_LIST));
	auto info = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_LIST)->getChildByTag(SELECTSCENE_INFO));
	auto level = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_LEVEL));
	auto difficulty = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_DIFFICULTY));
	auto score = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_SCORE));
	auto buttonReturn = dynamic_cast<Button*>(UILayer->getChildByTag(SELECTSCENE_RETURN));
	//////////
	buttonReturn->addTouchEventListener(this, toucheventselector(SelectScene::touchEvent));
	list->addEventListenerListView(this, listvieweventselector(SelectScene::listViewEvent));
	list->setItemModel(info);
	return true;
}

void SelectScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	/////////////////////////////////////////////////////
	auto files = FileUtils::getInstance()->getValueVectorFromFile("files.xml");
	int index = 0;
	for (auto& file : files)
	{
		auto fileInfo = file.asValueMap();
		auto fileName = fileInfo.at("name").asString();
		auto fileBPM = fileInfo.at("BPM").asFloat();
		auto fileScore = fileInfo.at("score").asFloat();
		if (selectMode == 0)
		{
			std::string musicname = "music/" + fileName + ".mp3";
			if (FileUtils::getInstance()->isFileExist(musicname))
			{
				if (index > 0)
					list->pushBackDefaultItem();
				((Text*)list->getItem(index++))->setText(fileName);
			}
		}
		else if (selectMode == 1)
		{
			std::string musicname = "music/" + fileName + ".mp3";
			std::string mapname = FileUtils::getInstance()->getWritablePath() + fileName + ".gnm";
			if (FileUtils::getInstance()->isFileExist(mapname) && FileUtils::getInstance()->isFileExist(musicname))
			{
				if (index > 0)
					list->pushBackDefaultItem();
				((Text*)list->getItem(index++))->setText(fileName);
			}
		}
	}
	//AudioEngine::getInstance()->play();
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
	Scene* scene;
	switch (eventType)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		scene = MainScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
		break;
	}
}

void SelectScene::listViewEvent(Ref* obj, ListViewEventType eventType)
{
	auto list = dynamic_cast<ListView*>(obj);
	auto index = list->getCurSelectedIndex();
	auto info = dynamic_cast<Text*>(list->getItem(index));

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
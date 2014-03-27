#include "SelectScene.h"
#include "MainScene.h"
#include "GameScene.h"
#include "MapUtils.h"

int selectMode;
Node *LoadingNode;

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
	LoadingNode = sceneNode->getChildByTag(10007);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("selectScene");
	auto LoadingComponent = (cocostudio::ComRender*) LoadingNode->getComponent("loadingUI");
	auto UILayer = UIComponent->getNode();
	auto LoadingLayer = (Layer*)LoadingComponent->getNode();
	list = dynamic_cast<ListView*>(UILayer->getChildByTag(SELECTSCENE_LIST));
	auto bg = dynamic_cast<ImageView*>(UILayer->getChildByTag(SELECTSCENE_LIST)->getChildByTag(SELECTSCENE_BG));
	auto info = dynamic_cast<Text*>(bg->getChildByTag(SELECTSCENE_INFO));
	auto level = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_LEVEL));
	auto difficulty = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_DIFFICULTY));
	auto score = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_SCORE));
	auto buttonReturn = dynamic_cast<Button*>(UILayer->getChildByTag(SELECTSCENE_RETURN));
	//////////
	buttonReturn->addTouchEventListener(this, toucheventselector(SelectScene::touchEvent));
	list->addEventListenerListView(this, listvieweventselector(SelectScene::listViewEvent));
	list->setItemModel(info);
	//////////
	auto bgLoading = dynamic_cast<ImageView*>(LoadingLayer->getChildByTag(SELECTSCENE_LOADING_BG));
	auto imageWords = dynamic_cast<ImageView*>(LoadingLayer->getChildByTag(SELECTSCENE_LOADING_WORDS));
	auto imageLight = dynamic_cast<ImageView*>(LoadingLayer->getChildByTag(SELECTSCENE_LOADING_LIGHT));
	imageLight->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1), FadeOut::create(1), NULL)));
	imageWords->runAction(RepeatForever::create(RotateBy::create(5, 360)));
	return true;
}

void SelectScene::loadingEnd()
{
	LoadingNode->setVisible(false);
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

	case ListViewEventType::LISTVIEW_ONSELECTEDITEM_END:
		if (selectMode == 0)
		{
			LoadingNode->setVisible(true);
			MapUtils::generateMap(info->getStringValue().c_str());
		}
		else if (selectMode == 1)
		{
			scene = GameScene::createScene(info->getStringValue());
			Director::getInstance()->replaceScene(TransitionPageTurn::create(2, scene, true));
		}
		break;
	}
}
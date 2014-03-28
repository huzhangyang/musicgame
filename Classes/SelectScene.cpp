#include "SelectScene.h"
#include "MainScene.h"
#include "GameScene.h"
#include "MapUtils.h"

int selectMode;
Node *LoadingNode;
std::vector<FileInfo> fileinfo;
float BPM;

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
	UINode = sceneNode->getChildByTag(10003);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("selectScene");
	auto UILayer = UIComponent->getNode();
	LoadingNode = sceneNode->getChildByTag(10007);
	auto LoadingComponent = (cocostudio::ComRender*) LoadingNode->getComponent("loadingUI");
	auto LoadingLayer = (Layer*)LoadingComponent->getNode();
	list = dynamic_cast<ListView*>(UILayer->getChildByTag(SELECTSCENE_LIST));
	auto bg = dynamic_cast<ImageView*>(UILayer->getChildByTag(SELECTSCENE_LIST)->getChildByTag(SELECTSCENE_BG));
	auto info = dynamic_cast<Text*>(bg->getChildByTag(SELECTSCENE_INFO));
	auto labelDifficulty = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_DIFFICULTY));
	auto buttonReturn = dynamic_cast<Button*>(UILayer->getChildByTag(SELECTSCENE_RETURN));
	//////////
	buttonReturn->addTouchEventListener(this, toucheventselector(SelectScene::touchEvent));
	list->addEventListenerListView(this, listvieweventselector(SelectScene::listViewEvent));
	list->setItemModel(bg);
	//////////
	auto bgLoading = dynamic_cast<ImageView*>(LoadingLayer->getChildByTag(SELECTSCENE_LOADING_BG));
	auto imageWords = dynamic_cast<ImageView*>(LoadingLayer->getChildByTag(SELECTSCENE_LOADING_WORDS));
	auto imageLight = dynamic_cast<ImageView*>(LoadingLayer->getChildByTag(SELECTSCENE_LOADING_LIGHT));
	bgLoading->setEnabled(false);
	imageLight->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1), FadeOut::create(1), NULL)));
	imageWords->runAction(RepeatForever::create(RotateBy::create(5, 360)));
	return true;
}

void SelectScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("selectScene");
	auto UILayer = UIComponent->getNode();
	auto labelDifficulty = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_DIFFICULTY));
	auto labelLevel = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_LEVEL));
	auto labelScore = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_SCORE));
	/////////////////////////////////////////////////////
	auto files = FileUtils::getInstance()->getValueVectorFromFile("files.xml");

	int index = 0;
	for (auto& file : files)
	{
		auto fileMap = file.asValueMap();
		FileInfo currinfo;
		currinfo.name = fileMap.at("name").asString();
		currinfo.BPM = fileMap.at("BPM").asFloat();
		currinfo.score = fileMap.at("score").asFloat();
		fileinfo.push_back(currinfo);
		if (selectMode == 0)
		{
			std::string musicname = "music/" + currinfo.name + ".mp3";
			if (FileUtils::getInstance()->isFileExist(musicname))
			{
				if (index > 0)
					list->pushBackDefaultItem();
				auto x = list->getItem(index);
				((Text*)(list->getItem(index++)->getChildByTag(SELECTSCENE_INFO)))->setText(currinfo.name);
			}
		}
		else if (selectMode == 1)
		{
			std::string musicname = "music/" + currinfo.name + ".mp3";
			std::string mapname = FileUtils::getInstance()->getWritablePath() + currinfo.name + ".gnm";
			if (FileUtils::getInstance()->isFileExist(mapname) && FileUtils::getInstance()->isFileExist(musicname))
			{
				if (index > 0)
					list->pushBackDefaultItem();
				((Text*)(list->getItem(index++)->getChildByTag(SELECTSCENE_INFO)))->setText(currinfo.name);
			}
		}
	}
	auto FileName = ((Text*)(list->getItem(0)->getChildByTag(SELECTSCENE_INFO)))->getStringValue();
	if (selectMode == 1)
	{
		auto difficulty = UserDefault::getInstance()->getIntegerForKey("difficulty");//获取当前难度
		MusicInfo musicinfo = MapUtils::loadMap(FileName.c_str());
		if (difficulty == 0)
		{
			labelDifficulty->setText("Easy");
			labelLevel->setText("LV." + musicinfo.Level_Easy);
		}
		else if (difficulty == 1)
		{
			labelDifficulty->setText("Hard");
			labelLevel->setText("LV." + musicinfo.Level_Hard);
		}
	}
	std::string musicname = "music/" + FileName + ".mp3";
	AudioEngine::getInstance()->createLoop(musicname.c_str());
	AudioEngine::getInstance()->play();
}

void SelectScene::onExitTransitionDidStart()
{
	Layer::onExitTransitionDidStart();
	/////////////////////////////////////////////////////
	MapUtils::closeMap();
	AudioEngine::getInstance()->stop();
}

void SelectScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void SelectScene::loadingEnd()
{
	auto LoadingComponent = (cocostudio::ComRender*) LoadingNode->getComponent("loadingUI");
	auto LoadingLayer = (Layer*)LoadingComponent->getNode();
	auto bgLoading = dynamic_cast<ImageView*>(LoadingLayer->getChildByTag(SELECTSCENE_LOADING_BG));
	LoadingNode->setVisible(false);
	bgLoading->setEnabled(false);
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
	auto bg = list->getItem(index);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("selectScene");
	auto UILayer = UIComponent->getNode();
	auto LoadingComponent = (cocostudio::ComRender*) LoadingNode->getComponent("loadingUI");
	auto LoadingLayer = (Layer*)LoadingComponent->getNode();
	auto info = dynamic_cast<Text*>(bg->getChildByTag(SELECTSCENE_INFO));
	auto labelLevel = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_LEVEL));
	auto labelScore = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_SCORE));
	auto bgLoading = dynamic_cast<ImageView*>(LoadingLayer->getChildByTag(SELECTSCENE_LOADING_BG));
	auto difficulty = UserDefault::getInstance()->getIntegerForKey("difficulty");//获取当前难度
	Scene* scene;
	switch (eventType)
	{
	case ListViewEventType::LISTVIEW_ONSELECTEDITEM_END:
		if (selectMode == 0)
		{
			for (auto& currinfo : fileinfo)
			{
				if (currinfo.name == info->getStringValue())
				{
					LoadingNode->setVisible(true);
					bgLoading->setEnabled(true);
					BPM = currinfo.BPM;
					MapUtils::generateMap(currinfo.name);
					break;
				}
			}
		}
		else if (selectMode == 1)
		{
			for (auto& currinfo : fileinfo)
			{
				if (currinfo.name == info->getStringValue())
				{
					BPM = currinfo.BPM;
					scene = GameScene::createScene(currinfo.name);
					Director::getInstance()->replaceScene(TransitionPageTurn::create(2, scene, true));
					break;
				}
			}

		}
		break;
	}
}
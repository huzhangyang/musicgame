#include "MainScene.h"
#include "GameScene.h"
#include "MapUtils.h"

Node *LoadingNode, *DialogNode, *ExitNode;
extern Layer *OptionLayer;

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
	OptionLayer = (Layer*)cocostudio::GUIReader::getInstance()->widgetFromJsonFile("optionUI/optionUI.json");
	addChild(OptionLayer);
	auto UINode = sceneNode->getChildByTag(10005);
	ExitNode = sceneNode->getChildByTag(10004);
	DialogNode = sceneNode->getChildByTag(10006);
	LoadingNode = sceneNode->getChildByTag(10007);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("mainSceneUI");
	auto ExitComponent = (cocostudio::ComRender*) ExitNode->getComponent("exitSelectUI");
	auto DialogComponent = (cocostudio::ComRender*) DialogNode->getComponent("dialogBoxUI");
	auto LoadingComponent = (cocostudio::ComRender*) LoadingNode->getComponent("loadingUI");
	auto UILayer = (Layer*)UIComponent->getNode();
	auto ExitLayer = (Layer*)ExitComponent->getNode();
	auto DialogLayer = (Layer*)DialogComponent->getNode();
	auto LoadingLayer = (Layer*)LoadingComponent->getNode();
	//////////
	auto imageTable = dynamic_cast<ImageView*>(UILayer->getChildByTag(MAINSCENE_IMAGE_TABLE));
	auto imagePaper = dynamic_cast<Button*>(UILayer->getChildByTag(MAINSCENE_IMAGE_PAPER));
	auto imageShelf = dynamic_cast<Button*>(UILayer->getChildByTag(MAINSCENE_IMAGE_SHELF));
	auto imageClock = dynamic_cast<ImageView*>(UILayer->getChildByTag(MAINSCENE_IMAGE_CLOCK));
	auto imageCat = dynamic_cast<ImageView*>(UILayer->getChildByTag(MAINSCENE_IMAGE_CAT));
	auto imageCharacter = dynamic_cast<ImageView*>(UILayer->getChildByTag(MAINSCENE_IMAGE_CHARACTER));
	auto buttonOption = dynamic_cast<Button*>(UILayer->getChildByTag(MAINSCENE_BUTTON_OPTION));
	auto buttonHelp = dynamic_cast<Button*>(UILayer->getChildByTag(MAINSCENE_BUTTON_HELP));
	auto buttonExit = dynamic_cast<Button*>(UILayer->getChildByTag(MAINSCENE_BUTTON_EXIT));
	imageTable->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	imagePaper->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	imageShelf->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	imageClock->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	imageCat->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	imageCharacter->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	buttonOption->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	buttonHelp->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	buttonExit->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	//////////
	auto bgExit = dynamic_cast<ImageView*>(ExitLayer->getChildByTag(MAINSCENE_EXIT_BG));
	auto buttonYes= dynamic_cast<Button*>(ExitLayer->getChildByTag(MAINSCENE_EXIT_BUTTON_YES));
	auto buttonNo = dynamic_cast<Button*>(ExitLayer->getChildByTag(MAINSCENE_EXIT_BUTTON_NO));
	bgExit->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	buttonYes->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	buttonNo->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	bgExit->setEnabled(false);
	buttonYes->setEnabled(false);
	buttonNo->setEnabled(false);
	//////////
	auto bgDialog = dynamic_cast<ImageView*>(DialogLayer->getChildByTag(MAINSCENE_DIALOG_BG));
	bgDialog->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	bgDialog->setEnabled(false);
	//////////
	auto bgLoading = dynamic_cast<ImageView*>(LoadingLayer->getChildByTag(MAINSCENE_LOADING_BG));
	auto imageWords = dynamic_cast<ImageView*>(LoadingLayer->getChildByTag(MAINSCENE_LOADING_WORDS));
	auto imageLight = dynamic_cast<ImageView*>(LoadingLayer->getChildByTag(MAINSCENE_LOADING_LIGHT));
	bgLoading->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	imageLight->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1), FadeOut::create(1), NULL)));
	imageWords->runAction(RepeatForever::create(RotateBy::create(5, 360)));
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

void MainScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void MainScene::createDialog(std::string key)
{
	auto DialogComponent = (cocostudio::ComRender*) DialogNode->getComponent("dialogBoxUI");
	auto DialogLayer = (Layer*)DialogComponent->getNode();
	auto labelWord = dynamic_cast<Text*>(DialogLayer->getChildByTag(MAINSCENE_DIALOG_WORDS));
	auto bgDialog = dynamic_cast<ImageView*>(DialogLayer->getChildByTag(MAINSCENE_DIALOG_BG));
	ValueMap strings = FileUtils::getInstance()->getValueMapFromFile("strings.xml");
	DialogNode->setVisible(true);
	bgDialog->setEnabled(true);
	labelWord->setText(strings.at(key).asString());
}

void MainScene::loadingEnd()
{
	LoadingNode->setVisible(false);
}

void MainScene::touchEvent(Ref* obj, TouchEventType eventType)
{
	auto widget = dynamic_cast<Widget*>(obj);
	int tag = widget->getTag();
	std::string musicname = "music/" + FILENAME + ".mp3";
	std::string mapname = FileUtils::getInstance()->getWritablePath() + FILENAME + ".gnm";
	auto ExitComponent = (cocostudio::ComRender*) ExitNode->getComponent("exitSelectUI");
	auto ExitLayer = (Layer*)ExitComponent->getNode();
	auto DialogComponent = (cocostudio::ComRender*) DialogNode->getComponent("dialogBoxUI");
	auto DialogLayer = (Layer*)DialogComponent->getNode();
	auto bgExit = dynamic_cast<ImageView*>(ExitLayer->getChildByTag(MAINSCENE_EXIT_BG));
	auto buttonYes = dynamic_cast<Button*>(ExitLayer->getChildByTag(MAINSCENE_EXIT_BUTTON_YES));
	auto buttonNo = dynamic_cast<Button*>(ExitLayer->getChildByTag(MAINSCENE_EXIT_BUTTON_NO));
	if (eventType == TouchEventType::TOUCH_EVENT_ENDED)
	{
		switch (tag)
		{
		case MAINSCENE_IMAGE_TABLE:
			this->createDialog("dialogTable");
			break;
		case MAINSCENE_IMAGE_PAPER:
			if (!FileUtils::getInstance()->isFileExist(musicname))
			{
				this->createDialog("dialogNoSong");
			}
			else
			{
				LoadingNode->setVisible(true);
				LoadingNode->runAction(FadeIn::create(1));
				MapUtils::generateMap(musicname.c_str());
				this->createDialog("dialogMapCreated");
			}
			break;
		case MAINSCENE_IMAGE_SHELF:
			if (!FileUtils::getInstance()->isFileExist(mapname))
			{
				this->createDialog("dialogNoMap");

			}
			else if (!FileUtils::getInstance()->isFileExist(musicname))
			{
				this->createDialog("dialogNoSong");
			}
			else
			{
				auto scene = GameScene::createScene(FILENAME);
				Director::getInstance()->replaceScene(TransitionPageTurn::create(2, scene, true));
			}
			break;
		case MAINSCENE_IMAGE_CLOCK:
			this->createDialog("dialogClock");
			break;
		case MAINSCENE_IMAGE_CAT:
			this->createDialog("dialogCat");
			break;
		case MAINSCENE_BUTTON_OPTION:
			//TODO
			if (UserDefault::getInstance()->getIntegerForKey("difficulty") == 0)
				UserDefault::getInstance()->setIntegerForKey("difficulty", 1);
			else
				UserDefault::getInstance()->setIntegerForKey("difficulty", 0);
			break;
		case MAINSCENE_BUTTON_HELP:
			break;
		case MAINSCENE_BUTTON_EXIT:
			ExitNode->setVisible(true);
			bgExit->setEnabled(true);
			buttonYes->setEnabled(true);
			buttonNo->setEnabled(true);
			break;
		case  MAINSCENE_EXIT_BUTTON_YES:
			Director::getInstance()->popScene();
			break;
		case  MAINSCENE_EXIT_BUTTON_NO:
			ExitNode->setVisible(false);
			bgExit->setEnabled(false);
			buttonYes->setEnabled(false);
			buttonNo->setEnabled(false);
			break;
		case MAINSCENE_IMAGE_CHARACTER:
			this->createDialog("dialogCharacter");
			break;
		case MAINSCENE_DIALOG_BG:
			DialogNode->setVisible(false);
			auto bgDialog = dynamic_cast<ImageView*>(DialogLayer->getChildByTag(MAINSCENE_DIALOG_BG));
			bgDialog->setEnabled(false);
			break;
		}
	}
}
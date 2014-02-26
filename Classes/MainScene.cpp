#include "MainScene.h"
#include "GameScene.h"
#include "MapGenerator.h"
#include <iostream>

const std::string FILENAME = "test";//曲子文件名
Node *ExitNode, *DialogNode;
TextBMFont *labelWord;

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

	auto UINode = sceneNode->getChildByTag(10005);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("mainSceneUI");
	auto UIlayer = UIComponent->getNode();
	auto objectTable = dynamic_cast<Button*>(UIlayer->getChildByTag(MAINSCENE_TABLE));
	objectTable->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto objectPaper = dynamic_cast<Button*>(UIlayer->getChildByTag(MAINSCENE_PAPER));
	objectPaper->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto objectShelf = dynamic_cast<Button*>(UIlayer->getChildByTag(MAINSCENE_SHELF));
	objectShelf->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto objectClock = dynamic_cast<Button*>(UIlayer->getChildByTag(MAINSCENE_CLOCK));
	objectClock->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto objectCat = dynamic_cast<Button*>(UIlayer->getChildByTag(MAINSCENE_CAT));
	objectCat->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto buttonOption = dynamic_cast<Button*>(UIlayer->getChildByTag(MAINSCENE_OPTION));
	buttonOption->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto buttonHelp = dynamic_cast<Button*>(UIlayer->getChildByTag(MAINSCENE_HELP));
	buttonHelp->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto buttonExit = dynamic_cast<Button*>(UIlayer->getChildByTag(MAINSCENE_EXIT));
	buttonExit->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto objectCharacter = dynamic_cast<ImageView*>(UIlayer->getChildByTag(MAINSCENE_CHARACTER));
	objectCharacter->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));

	ExitNode = sceneNode->getChildByTag(10004);
	auto ExitComponent = (cocostudio::ComRender*) ExitNode->getComponent("exitSelectUI");
	auto Exitlayer = ExitComponent->getNode();
	auto buttonCheck = dynamic_cast<Button*>(Exitlayer->getChildByTag(MAINSCENE_CHECKMARK));
	buttonCheck->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	auto buttonCross = dynamic_cast<Button*>(Exitlayer->getChildByTag(MAINSCENE_CROSSMARK));
	buttonCross->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));

	DialogNode = sceneNode->getChildByTag(10006);
	auto dialogComponent = (cocostudio::ComRender*) DialogNode->getComponent("dialogBoxUI");
	auto dialoglayer = dialogComponent->getNode();
	auto objectBox = dynamic_cast<ImageView*>(dialoglayer->getChildByTag(MAINSCENE_BOX));
	labelWord = dynamic_cast<TextBMFont*>(dialoglayer->getChildByTag(MAINSCENE_WORD));
	objectBox->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));

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

void MainScene::menuCloseCallback(Object* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void MainScene::speak(std::string content)
{
	DialogNode->setVisible(true);
	labelWord->setText(content.c_str());
}

void MainScene::touchEvent(Object* obj, gui::TouchEventType eventType)
{
	auto widget = dynamic_cast<Widget*>(obj);
	int tag = widget->getTag();
	switch (eventType)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		if (tag == MAINSCENE_TABLE)
		{
			this->speak("我只是桌子不要碰我啊尼玛");
		}
		else if (tag == MAINSCENE_PAPER)
		{
			std::string musicname = "music/" + FILENAME + ".mp3";
			MapGenerator::generateMap(musicname.c_str());
			this->speak("呼，完成了！");
		}
		else if (tag == MAINSCENE_SHELF)
		{
			std::string mapname = FileUtils::getInstance()->getWritablePath() + FILENAME + ".gnm";
			if (FileUtils::getInstance()->isFileExist(mapname))
			{
				auto scene = GameScene::createScene(FILENAME);
				Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
			}
			else
				this->speak("好像还没有作曲吧…");

		}
		else if (tag == MAINSCENE_CLOCK)
		{
			this->speak("时间紧迫啊…");
		}
		else if (tag == MAINSCENE_CAT)
		{
			this->speak("喵");
		}
		else if (tag == MAINSCENE_OPTION)
		{
			UserDefault::getInstance()->setIntegerForKey("difficulty", 0);
		}
		else if (tag == MAINSCENE_HELP)
		{
		}
		else if (tag == MAINSCENE_EXIT)
		{
			ExitNode->setVisible(true);
		}
		else if (tag == MAINSCENE_CHECKMARK && ExitNode->isVisible())
		{
			Director::getInstance()->popScene();
		}
		else if (tag == MAINSCENE_CROSSMARK && ExitNode->isVisible())
		{
			ExitNode->setVisible(false);
		}
		else if (tag == MAINSCENE_CHARACTER)
		{
			this->speak("道姐威武~");
		}
		else if (tag == MAINSCENE_BOX && DialogNode->isVisible())
		{
			DialogNode->setVisible(false);
		}
		break;
	}
}


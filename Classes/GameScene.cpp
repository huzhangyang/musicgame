#include "GameScene.h"
#include "ClearScene.h"
#include "Note.h"

int framecounter;
int counterAll, counterPerfect, counterGood, counterMiss, counterCombo, counterMaxcombo;
TextBMFont *labelInfo, *labelCombo, *labelJudge;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);

	framecounter;//帧数计数器
	counterAll = 0;//音符总数
	counterPerfect = 0;//完美数
	counterGood = 0;//普通击中数
	counterMiss = 0;//错过数
	counterCombo = 0;//连击数
	counterMaxcombo = 0;//最大连击数

	return scene;
}

bool GameScene::init()
{
	//super init
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////////////////////////////
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/game.mp3");
	auto sceneNode = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("gameScene.json");
	sceneNode->setLocalZOrder(-1);
	addChild(sceneNode);
	auto UINode = sceneNode->getChildByTag(10004);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("GUIComponent");
	auto UIlayer = UIComponent->getNode();
	auto buttonPause = dynamic_cast<Button*>(UIlayer->getChildByTag(GAMESCENE_PAUSE));
	buttonPause->addTouchEventListener(this, toucheventselector(GameScene::touchEvent));
	labelInfo = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(GAMESCENE_INFO));
	labelCombo = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(GAMESCENE_COMBO));
	labelJudge = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(GAMESCENE_JUDGE));
	labelJudge->setScale(0);
	labelCombo->setOpacity(0);
	return true;
}

void GameScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	/////////////////////////////////////////////////////
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/game.mp3");
	this->scheduleUpdate();
}

void GameScene::menuCloseCallback(Object* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void GameScene::update(float dt)
{
	framecounter++;
	switch (framecounter % 180)
	{
	case 0:addRandomNote(2); break;
	case 60:addRandomNote(1); break;
	case 120:addRandomNote(0); break;
	}
	if (!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		this->unscheduleUpdate();
		if (counterMaxcombo == 0)
			counterMaxcombo = counterAll;//全程无miss
		double completePercent = (double)counterPerfect / (double)counterAll * 0.7;
		completePercent += (double)counterGood / (double)counterAll * 0.6;
		completePercent += (double)counterMaxcombo / (double)counterAll * 0.3;
		log("%d %d %d %d %d", counterAll, counterPerfect, counterGood, counterMiss, counterMaxcombo);
		log("%.2f%s", completePercent, "%");//简易结算

		auto scene = ClearScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
	}
}

void GameScene::touchEvent(Object* obj, gui::TouchEventType eventType)
{
	auto button = dynamic_cast<Button*>(obj);
	int tag = button->getTag();
	switch (eventType)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		if (tag == GAMESCENE_PAUSE)
		{
			if (!Director::getInstance()->isPaused())
			{
				Director::getInstance()->pause();
				CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
			}
			else
			{
				Director::getInstance()->resume();
				CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
			}
		}
		break;
	}
}

void GameScene::addNewNote(int posX, int posY, int type)
{
	counterAll++;
	auto note = Note::createNote(posX, posY, type);
	auto noteListener = EventListenerTouchOneByOne::create();
	noteListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	noteListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	noteListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(noteListener, note);
	addChild(note);
}

void GameScene::addRandomNote(int type)
{
	int randomX = CCRANDOM_0_1() * 6 + 1;
	int randomY = CCRANDOM_0_1() * 5 + 1;
	addNewNote(randomX, randomY, type);
}

bool GameScene::onTouchBegan(Touch *touch, Event  *event)
{
	auto target = static_cast<Note*>(event->getCurrentTarget());
	Point locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode) && !Director::getInstance()->isPaused() && !target->isTouched())
	{
		target->setTouched();
		if (target->getType() == 0)
		{
			float lifePercent = (float)target->getLife() / (float)target->getLifeSpan();
			if (lifePercent >= 0.8 || lifePercent <= 0.4)
				judgeNote(1);
			else
				judgeNote(2);
			target->runAction(FadeOut::create(0.2));
		}
		else if (target->getType() == 1 || target->getType() == 2)
		{
			target->setScale(1.25);
		}
	}
	return true;
}
void GameScene::onTouchMoved(Touch *touch, Event  *event)
{
	auto target = static_cast<Note*>(event->getCurrentTarget());
	if (!Director::getInstance()->isPaused())
	{
		if (target->getType() == 2)
		{
			target->setPosition(touch->getLocation());
		}
	}
}
void GameScene::onTouchEnded(Touch *touch, Event  *event)
{
	auto target = static_cast<Note*>(event->getCurrentTarget());
	Point locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode) && !Director::getInstance()->isPaused())
	{
		target->setTouchEnded();
		target->setScale(1);
		target->runAction(FadeOut::create(0.2));
		if (target->getType() == 1)
		{
			float lifePercent = (float)target->getLifeTouched() / (float)target->getLifeSpan();
			if (lifePercent >= 0.8 || lifePercent <= 0.4)
				judgeNote(1);
			else
				judgeNote(2);
		}
		else if (target->getType() == 2)
		{
		}
	}
}
void GameScene::judgeNote(int judge)
{
	char buffer[20];
	if (judge == 0)
	{
		if (counterMaxcombo < counterCombo)
			counterMaxcombo = counterCombo;
		counterCombo = 0;
		counterMiss++;
		labelJudge->setText("Miss!");
		labelCombo->setText("");
	}
	else if (judge == 1)
	{
		counterCombo++;
		counterGood++;
		labelJudge->setText("Good!");
		labelCombo->setText(_itoa(counterCombo, buffer, 10));
	}
	else if (judge == 2)
	{
		counterCombo++;
		counterPerfect++;
		labelJudge->setText("Perfect!");
		labelCombo->setText(_itoa(counterCombo, buffer, 10));
	}
	labelJudge->setScale(1.25);
	labelJudge->runAction(ScaleTo::create(0.25, 1));
	labelCombo->runAction(FadeOut::create(1));
}


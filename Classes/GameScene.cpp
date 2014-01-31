#include "GameScene.h"
#include "MainScene.h"
#include "Note.h"

const float BPM = 91.97;

int counterAll, counterPerfect, counterGood, counterMiss, counterCombo, counterMaxcombo;
TextBMFont* labelInfo;
TextBMFont* labelCombo;
TextBMFont* labelJudge;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);

	counterAll = 0;//音符总数
	counterPerfect = 0;//完美数
	counterGood = 0;//普通击中数
	counterMiss = 0;//错过数
	counterCombo = 0;//连击数
	counterMaxcombo = 0;//最大连击数

	return scene;
}

void GameScene::addNewNote(int posX, int posY, int type)
{
	counterAll++;
	auto note = Note::createNote(posX, posY, type);
	auto noteListener = EventListenerTouchOneByOne::create();
	noteListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(noteListener, note);
	addChild(note);
}

void GameScene::addRandomNote(float dt)
{
	if (!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		this->unschedule(schedule_selector(GameScene::addRandomNote));

		if (counterMaxcombo == 0)
			counterMaxcombo = counterAll;//全程无miss
		double completePercent = (double)counterPerfect / counterAll * 0.7;
		completePercent += (double)counterGood / counterAll * 0.6;
		completePercent += (double)counterMaxcombo / counterAll * 0.3;
		log("%d %d %d %d", counterAll, counterPerfect, counterGood, counterMiss);
		log("%.2f%s", completePercent, "%");//简易结算

		auto scene = MainScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
	}
	int randomX = CCRANDOM_0_1() * 6 + 1;
	int randomY = CCRANDOM_0_1() * 5 + 1;
	addNewNote(randomX, randomY, 0);
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
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/test.mp3");
	auto sceneNode = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("gameScene.json");
	sceneNode->setZOrder(0);
	addChild(sceneNode);
	auto UINode = sceneNode->getChildByTag(10004);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("GUIComponent");
	auto UIlayer = UIComponent->getNode();
	auto buttonPause = dynamic_cast<Button*>(UIlayer->getChildByTag(GAMESCENE_PAUSE));
	buttonPause->addTouchEventListener(this, toucheventselector(GameScene::touchEvent));
	auto x = UIlayer->getChildByTag(GAMESCENE_INFO);
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
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/test.mp3");
	this->schedule(schedule_selector(GameScene::addRandomNote), 60 / BPM);
}

void GameScene::menuCloseCallback(Object* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
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

bool GameScene::onTouchBegan(Touch *touch, Event  *event)
{
	auto target = static_cast<Note*>(event->getCurrentTarget());
	Point locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode) && !Director::getInstance()->isPaused() && !target->isTouched())
	{
		target->setTouched();
		float life = target->getLife() / 60.0;
		if (life >= 2 / 2.5 || life <= 1 / 2.5)
			setCondition(1);
		else
			setCondition(2);
		target->stopAllActions();
		target->setLife(20);
		target->runAction(RotateBy::create(1 / 3.0, 360));
		return true;
	}
	else
	{
		return false;
	}

}

void GameScene::setCondition(int cond)
{
	char buffer[20];
	if (cond == 0)
	{
		if (counterMaxcombo < counterCombo)
			counterMaxcombo = counterCombo;
		counterCombo = 0;
		counterMiss++;
		labelJudge->setText("MISS!");
		labelCombo->setText("");
	}
	else if (cond == 1)
	{
		counterCombo++;
		counterGood++;
		labelJudge->setText("GOOD!!");
		labelCombo->setText(_itoa(counterCombo, buffer, 10));
	}
	else
	{
		counterCombo++;
		counterPerfect++;
		labelJudge->setText("PERFECT!!!");
		labelCombo->setText(_itoa(counterCombo, buffer, 10));
	}
	labelJudge->setScale(1.25);
	labelJudge->runAction(ScaleTo::create(0.25, 1));
	labelCombo->runAction(FadeOut::create(1));
}


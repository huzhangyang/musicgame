#include "Global.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "Note.h"

const int POS_X1 = 270;
const int POS_X2 = 420;
const int POS_X3 = 570;
const int POS_X4 = 720;
const int POS_X5 = 870;
const int POS_X6 = 1020;
const int POS_Y1 = 470;
const int POS_Y2 = 380;
const int POS_Y3 = 290;
const int POS_Y4 = 200;
const int POS_Y5 = 110;

int framecounter;
int counterTotal, counterPerfect, counterGood, counterMiss, counterCombo, counterMaxcombo;
TextBMFont *labelInfo, *labelCombo, *labelJudge;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);

	framecounter;//帧数计数器
	counterTotal = 0;//音符总数
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
	switch (framecounter % 720)
	{
	case 180:addRandomNote(2); break;
	case 360:addRandomNote(1); break;
	case 540:addRandomNote(0); break;
	}
	if (!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		this->unscheduleUpdate();
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
			auto scene = ClearScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
			/*if (!Director::getInstance()->isPaused())
			{
			Director::getInstance()->pause();
			CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
			}
			else
			{
			Director::getInstance()->resume();
			CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
			}*/
		}
		break;
	}
}

void GameScene::addNewNote(int type, int pos, int des)
{
	counterTotal++;
	auto note = Note::createNote(type, pos, des);
	auto noteListener = EventListenerTouchOneByOne::create();
	noteListener->setSwallowTouches(true);
	noteListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	noteListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	noteListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(noteListener, note);
	addChild(note);
	if (note->getType() == 2)
		addArrow(pos, des);
}

void GameScene::addArrow(int pos, int des)
{
	auto arrow = Sprite::create("gameSceneUI/arrow.png");
	auto dest = Sprite::create("gameSceneUI/note.png");
	int X, Y;
	switch (pos / 10)
	{
	case 1:X = POS_X1; break;
	case 2:X = POS_X2; break;
	case 3:X = POS_X3; break;
	case 4:X = POS_X4; break;
	case 5:X = POS_X5; break;
	case 6:X = POS_X6; break;
	default: break;
	}
	switch (des / 10)
	{
	case 1:X += POS_X1; dest->setPositionX(POS_X1); break;
	case 2:X += POS_X2; dest->setPositionX(POS_X2); break;
	case 3:X += POS_X3; dest->setPositionX(POS_X3); break;
	case 4:X += POS_X4; dest->setPositionX(POS_X4); break;
	case 5:X += POS_X5; dest->setPositionX(POS_X5); break;
	case 6:X += POS_X6; dest->setPositionX(POS_X6); break;
	default: break;
	}
	switch (pos % 10)
	{
	case 1:Y = POS_Y1; break;
	case 2:Y = POS_Y2; break;
	case 3:Y = POS_Y3; break;
	case 4:Y = POS_Y4; break;
	case 5:Y = POS_Y5; break;
	default: break;
	}
	switch (des % 10)
	{
	case 1:Y += POS_Y1; dest->setPositionY(POS_Y1); break;
	case 2:Y += POS_Y2; dest->setPositionY(POS_Y2); break;
	case 3:Y += POS_Y3; dest->setPositionY(POS_Y3); break;
	case 4:Y += POS_Y4; dest->setPositionY(POS_Y4); break;
	case 5:Y += POS_Y5; dest->setPositionY(POS_Y5); break;
	default: break;
	}
	arrow->setPosition(X / 2, Y / 2);
	if (des / 10 > pos / 10 && des % 10 == pos % 10);
	else if (des / 10 > pos / 10 && des % 10 > pos % 10)arrow->setRotation(45);
	else if (des / 10 == pos / 10 && des % 10 > pos % 10)arrow->setRotation(90);
	else if (des / 10 < pos / 10 && des % 10 > pos % 10)arrow->setRotation(135);
	else if (des / 10 < pos / 10 && des % 10 == pos % 10)arrow->setRotation(180);
	else if (des / 10 < pos / 10 && des % 10 < pos % 10)arrow->setRotation(225);
	else if (des / 10 == pos / 10 && des % 10 < pos % 10)arrow->setRotation(270);
	else if (des / 10 > pos / 10 && des % 10 < pos % 10)arrow->setRotation(315);
	dest->runAction(FadeOut::create(3));
	arrow->runAction(FadeOut::create(3));
	this->addChild(arrow);
	this->addChild(dest);
}

void GameScene::addRandomNote(int type)
{
	int randomX = CCRANDOM_0_1() * 6 + 1;
	int randomY = CCRANDOM_0_1() * 5 + 1;
	int randomA = CCRANDOM_0_1() * 6 + 1;
	int randomB = CCRANDOM_0_1() * 5 + 1;
	addNewNote(type, randomX * 10 + randomY, randomA * 10 + randomB);
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
			target->setNotMissed();
			target->stopAllActions();
			target->runAction(FadeOut::create(0.2));
			float lifePercent = (float)target->getLife() / (float)target->getLifeSpan();
			if (lifePercent >= 0.8 || lifePercent <= 0.4)
				judgeNote(1);
			else
				judgeNote(2);
		}
		else
			target->setScale(1.25);
	}
	return true;
}
void GameScene::onTouchMoved(Touch *touch, Event  *event)
{
	auto target = static_cast<Note*>(event->getCurrentTarget());
	int X, Y;
	switch (target->getDest() / 10)
	{
	case 1:X = POS_X1; break;
	case 2:X = POS_X2; break;
	case 3:X = POS_X3; break;
	case 4:X = POS_X4; break;
	case 5:X = POS_X5; break;
	case 6:X = POS_X6; break;
	default: break;
	}
	switch (target->getDest() % 10)
	{
	case 1:Y = POS_X1; break;
	case 2:Y = POS_X2; break;
	case 3:Y = POS_X3; break;
	case 4:Y = POS_X4; break;
	case 5:Y = POS_X5; break;
	default: break;
	}

	if (!Director::getInstance()->isPaused() && target->getType() == 2)
	{
		target->setPosition(touch->getLocation());
		//要制作出轨迹呀！
	}
}
void GameScene::onTouchEnded(Touch *touch, Event  *event)
{
	auto target = static_cast<Note*>(event->getCurrentTarget());
	Point locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	target->setScale(1);
	if (rect.containsPoint(locationInNode) && !Director::getInstance()->isPaused() && target->getType() == 1)
	{
		target->setNotMissed();
		target->stopAllActions();
		target->runAction(FadeOut::create(0.2));
		float lifePercent = (float)target->getLifeTouched() / (float)target->getLifeSpan();
		if (lifePercent >= 0.8 || lifePercent <= 0.4)
			judgeNote(1);
		else
			judgeNote(2);
	}
}
void GameScene::judgeNote(int judge)
{
	char buffer[64];
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
	labelJudge->setVisible(true);
	labelCombo->setVisible(true);
	labelJudge->setScale(1.25);
	labelJudge->runAction(ScaleTo::create(0.25, 1));
	labelCombo->runAction(FadeOut::create(1));
}


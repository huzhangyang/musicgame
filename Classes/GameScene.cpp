#include "Global.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "Note.h"

const float TIME_DISAPPEAR = 0.2;//消失特效时间

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
		addArrow(note->getPositionX(), note->getPositionY(), note->getDestX(), note->getDestY());
}

void GameScene::addArrow(int posX, int posY, int desX, int desY)
{
	auto arrow = Sprite::create("gameSceneUI/arrow.png");
	auto dest = Sprite::create("gameSceneUI/note.png");
	arrow->setPosition(posX / 2 + desX / 2, posY / 2 + desY / 2);
	dest->setPosition(desX, desY);
	if (desX > posX && desY == posY);
	else if (desX > posX && desY < posY)arrow->setRotation(45);
	else if (desX == posX && desY < posY)arrow->setRotation(90);
	else if (desX < posX && desY < posY)arrow->setRotation(135);
	else if (desX < posX && desY == posY)arrow->setRotation(180);
	else if (desX < posX && desY > posY)arrow->setRotation(225);
	else if (desX == posX && desY > posY)arrow->setRotation(270);
	else if (desX > posX && desY > posY)arrow->setRotation(315);
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
			target->stopAllActions();
			target->unscheduleUpdate();
			target->scheduleOnce(schedule_selector(Note::removeNote), TIME_DISAPPEAR);
			target->runAction(FadeOut::create(TIME_DISAPPEAR));//点击后消失特效
			target->judge();
		}
		else
		{
			target->setScale(1.25);
			target->runAction(RotateBy::create(target->getLifeSpan() / 60.0, 360));
		}
	}
	return true;
}
void GameScene::onTouchMoved(Touch *touch, Event  *event)
{
	auto target = static_cast<Note*>(event->getCurrentTarget());
	if (!Director::getInstance()->isPaused() && target->getType() == 2)
	{
		target->setPosition(touch->getLocation());
		//要制作出轨迹呀！使用基本图形绘制？
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
		target->setScale(1);
		target->stopAllActions();
		target->unscheduleUpdate();
		target->scheduleOnce(schedule_selector(Note::removeNote), TIME_DISAPPEAR);
		target->runAction(FadeOut::create(TIME_DISAPPEAR));//点击后消失特效
		target->judge();
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


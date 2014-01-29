#include "GameScene.h"
#include "MainScene.h"
#include "Note.h"

const int POS_X1 = 260;
const int POS_X2 = 410;
const int POS_X3 = 560;
const int POS_X4 = 710;
const int POS_X5 = 860;
const int POS_X6 = 1010;
const int POS_Y1 = 470;
const int POS_Y2 = 380;
const int POS_Y3 = 290;
const int POS_Y4 = 200;
const int POS_Y5 = 110;

int allnotes, combo, hit, perfect, maxcombo;
Array* notearray;
LabelTTF* labelCond;
LabelTTF* labelCombo;

const float BPM = 91.97;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);

	notearray = Array::create();
	notearray->retain();
	allnotes = 0;//音符总数
	combo = 0;//连击数
	hit = 0;//击中数
	perfect = 0;//完美数
	maxcombo = 0;

	return scene;
}

void GameScene::addNewNote(Point p)
{
	auto note = Note::createAtPoint(p, allnotes++);
	notearray->addObject(note);
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

		if (maxcombo == 0)
			maxcombo = allnotes;
		log("%d %d %d", allnotes, hit, perfect);
		log("%.2f%s", (double)perfect / allnotes*0.1 + (double)(hit - perfect) / allnotes*0.6 + (double)maxcombo / allnotes*0.3, "%");//简易结算

		auto scene = MainScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
	}
	int randomX = CCRANDOM_0_1() * 6 + 1;
	int randomY = CCRANDOM_0_1() * 5 + 1;
	switch (10 * randomX + randomY)
	{
	case 11:addNewNote(Point(POS_X1, POS_Y1)); break;
	case 12:addNewNote(Point(POS_X1, POS_Y2)); break;
	case 13:addNewNote(Point(POS_X1, POS_Y3)); break;
	case 14:addNewNote(Point(POS_X1, POS_Y4)); break;
	case 15:addNewNote(Point(POS_X1, POS_Y5)); break;
	case 21:addNewNote(Point(POS_X2, POS_Y1)); break;
	case 22:addNewNote(Point(POS_X2, POS_Y2)); break;
	case 23:addNewNote(Point(POS_X2, POS_Y3)); break;
	case 24:addNewNote(Point(POS_X2, POS_Y4)); break;
	case 25:addNewNote(Point(POS_X2, POS_Y5)); break;
	case 31:addNewNote(Point(POS_X3, POS_Y1)); break;
	case 32:addNewNote(Point(POS_X3, POS_Y2)); break;
	case 33:addNewNote(Point(POS_X3, POS_Y3)); break;
	case 34:addNewNote(Point(POS_X3, POS_Y4)); break;
	case 35:addNewNote(Point(POS_X3, POS_Y5)); break;
	case 41:addNewNote(Point(POS_X4, POS_Y1)); break;
	case 42:addNewNote(Point(POS_X4, POS_Y2)); break;
	case 43:addNewNote(Point(POS_X4, POS_Y3)); break;
	case 44:addNewNote(Point(POS_X4, POS_Y4)); break;
	case 45:addNewNote(Point(POS_X4, POS_Y5)); break;
	case 51:addNewNote(Point(POS_X5, POS_Y1)); break;
	case 52:addNewNote(Point(POS_X5, POS_Y2)); break;
	case 53:addNewNote(Point(POS_X5, POS_Y3)); break;
	case 54:addNewNote(Point(POS_X5, POS_Y4)); break;
	case 55:addNewNote(Point(POS_X5, POS_Y5)); break;
	case 61:addNewNote(Point(POS_X6, POS_Y1)); break;
	case 62:addNewNote(Point(POS_X6, POS_Y2)); break;
	case 63:addNewNote(Point(POS_X6, POS_Y3)); break;
	case 64:addNewNote(Point(POS_X6, POS_Y4)); break;
	case 65:addNewNote(Point(POS_X6, POS_Y5)); break;
	}
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
	auto child = sceneNode->getChildByTag(10004);
	auto reader = (cocostudio::ComRender*)child->getComponent("GUIComponent");
	auto layer = (Layer*)reader->getNode();

	auto pause = dynamic_cast<Button*>(layer->getChildByTag(GAMESCENE_PAUSE));
	pause->addTouchEventListener(this, toucheventselector(GameScene::touchEvent));
	labelCond = LabelTTF::create("", "微软雅黑", 36);
	labelCond->setColor(Color3B(75, 92, 196));
	labelCond->setPosition(1180, 300);
	addChild(labelCond);
	labelCombo = LabelTTF::create("", "微软雅黑", 36);
	labelCombo->setColor(Color3B(75, 92, 196));
	labelCombo->setPosition(1180, 250);
	addChild(labelCombo);
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
			else//否则  
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
		if (life >= 2 / 3.0 || life <= 1 / 3.0)
			setCondition(1);
		else
			setCondition(2);
		ActionInterval* action = RotateBy::create(life, 360);
		target->stopAllActions();
		target->runAction(action);
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
		if (maxcombo < combo)
			maxcombo = combo;
		combo = 0;
		labelCond->setString("MISS!");
		labelCombo->setString("");
	}
	else if (cond == 1)
	{
		combo++;
		hit++;
		labelCond->setString("GOOD!!");
		labelCombo->setString(_itoa(combo, buffer, 10));
	}
	else
	{
		combo++;
		hit++;
		perfect++;
		labelCond->setString("PERFECT!!!");
		labelCombo->setString(_itoa(combo, buffer, 10));
	}
	ActionInterval* action = FadeOut::create(1);
	labelCond->runAction(action);
	//labelCombo->runAction(action);
}


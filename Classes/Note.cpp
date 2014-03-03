#include "Note.h"
#include "GameScene.h"

Note::Note()
{
}

Note::~Note()
{
}

Note* Note::createNote(int type, int length, int pos, int des)
{
	Note *note = new Note();
	if (note)
	{
		note->initNote(type, length, pos, des);
		note->autorelease();
		return note;
	}
	CC_SAFE_DELETE(note);
	return NULL;
}

void Note::initNote(int type, int length, int pos, int des)
{
	this->type = (NoteType)type;
	this->life = TIME_PRELOAD;
	this->lifeTouchBegan = 0;
	this->isActivated = false;
	this->isTouched = false;
	this->setPositionX(120 * (pos / 10) + 80);
	this->setPositionY(60 * (10 - pos % 10) + 5);
	this->desX = 120 * (des / 10) + 80;
	this->desY = 60 * (10 - des % 10) + 5;
	judgePic = Sprite::create("gameSceneUI/judge.png");
	auto noteListener = EventListenerTouchOneByOne::create();
	noteListener->setSwallowTouches(true);//一次触摸只对一个有效
	switch (type)
	{
	case CLICK:
		this->initWithFile("gameSceneUI/note0.png");
		this->life = TIME_PRELOAD;
		noteListener->onTouchBegan = CC_CALLBACK_2(Note::onTouchBegan, this);
		break;
	case LONGPRESS:
		this->initWithFile("gameSceneUI/note1.png");
		this->length = length;
		noteListener->onTouchBegan = CC_CALLBACK_2(Note::onTouchBegan, this);
		noteListener->onTouchEnded = CC_CALLBACK_2(Note::onTouchEnded, this);
		break;
	case SLIDE:
		this->initWithFile("gameSceneUI/note2.png");
		this->length = length;
		//this->setRotation(atan2(desX - getPositionX(), desY - getPositionY()) * 180 / M_PI);
		//judgePic->setRotation(-atan2(desX - getPositionX(), desY - getPositionY()) * 180 / M_PI);
		noteListener->onTouchMoved = CC_CALLBACK_2(Note::onTouchMoved, this);
		break;
	}
	judgePic->setOpacity(128);
	judgePic->setScale(2);
	judgePic->runAction(Sequence::create(ScaleTo::create(TIME_PRELOAD / 60.0, 1), CallFunc::create(CC_CALLBACK_0(Note::onPreloadTimeOver, this)), NULL));
	judgePic->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
	this->addChild(judgePic);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(noteListener, this);
}

void Note::removeNote()
{
	this->removeFromParent();
}

void Note::update(float dt)
{
	life--;//减少生命
	if (life <= 0)
	{
		this->judge();
		this->removeNote();
	}
}

void Note::onPreloadTimeOver()
{
	judgePic->runAction(ScaleTo::create(life / 60.0, 0));
	isActivated = true;
	if (isTouched)
		this->lifeTouchBegan = life;
	this->scheduleUpdate();
}

void Note::judge()
{
	float lifePercent;
	int judgeResult;

	this->stopAllActions();//停止所有动作
	this->unscheduleAllSelectors();//停止所有计算
	this->runAction(Sequence::create(FadeOut::create(0.2f), CallFunc::create(CC_CALLBACK_0(Note::removeNote, this)), NULL));//消失特效

	switch (type)
	{
	case CLICK:
		lifePercent = (float)life / TIME_PRELOAD;
		if (lifePercent <= 0.1 || lifePercent >= 0.9)//太早或太晚都是miss
			judgeResult = 0;
		else if (lifePercent <= 0.3 || lifePercent >= 0.7)//正中点前后40%开外只能是good
			judgeResult = 1;
		else
			judgeResult = 2;
		break;
	case LONGPRESS:
		lifePercent = (float)(lifeTouchBegan - life) / length;
		if (lifePercent <= 0.4)//触摸时间太短则判定为miss
			judgeResult = 0;
		else if (lifePercent <= 0.8)//触摸时间不够长是good
			judgeResult = 1;
		else
			judgeResult = 2;
		break;
	case SLIDE:
		lifePercent = (float)life / TIME_PRELOAD;
		if (lifePercent <= 0.1 || lifePercent >= 0.9)//太早或太晚都是miss
			judgeResult = 0;
		else if (lifePercent <= 0.3 || lifePercent >= 0.7)//正中点前后40%开外只能是good
			judgeResult = 1;
		else
			judgeResult = 2;
		break;
	}
	if (judgeResult == 0)
		judgePic->setTexture("clearSceneUI/miss.png");
	else if (judgeResult == 1)
		judgePic->setTexture("clearSceneUI/good.png");
	else
		judgePic->setTexture("clearSceneUI/perfect.png");
	judgePic->setScale(0.5);
	judgePic->runAction(FadeOut::create(0.2f));
	GameScene::judgeNote(judgeResult);
}

bool Note::onTouchBegan(Touch *touch, Event  *event)
{
	auto note = static_cast<Note*>(event->getCurrentTarget());
	Point pos = note->getPosition();
	Size s = note->getContentSize();
	Rect rect = Rect(pos.x - s.width / 2, pos.y - s.height / 2, s.width, s.height);
	if (rect.containsPoint(touch->getLocation()) && !Director::getInstance()->isPaused() && isTouched == false)
	{
		isTouched == true;
		if (note->type == CLICK)//对普通note，直接进行判定
			note->judge();
		else if (isActivated)//对已开始生命周期的长按note,记录此时生命值
			note->lifeTouchBegan = note->life;
		return true;
	}
	return false;
}
void Note::onTouchMoved(Touch *touch, Event  *event)
{
	auto note = static_cast<Note*>(event->getCurrentTarget());
	Point pos = note->getPosition();
	Size s = note->getContentSize();
	Rect rect = Rect(pos.x - s.width / 2, pos.y - s.height / 2, s.width, s.height);
	if (rect.containsPoint(touch->getLocation()) && !Director::getInstance()->isPaused() && note->life != 0)//滑动经过音符内时进行滑动音符的判定
	{
		note->judge();
	}
}
void Note::onTouchEnded(Touch *touch, Event  *event)
{
	auto note = static_cast<Note*>(event->getCurrentTarget());
	if (!Director::getInstance()->isPaused() && note->life != 0)//提前松手时进行长按音符的判定
		note->judge();
}
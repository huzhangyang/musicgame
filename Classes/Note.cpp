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
		note->scheduleUpdate();
		note->autorelease();
		return note;
	}
	CC_SAFE_DELETE(note);
	return NULL;
}

void Note::initNote(int type, int length, int pos, int des)
{
	this->type = (NoteType)type;
	this->setTag(++counter.total);
	this->life = TIME_PRELOAD;
	this->lifeTouchBegan = 0;
	this->isActivated = false;
	this->isTouched = false;
	this->isSlided = false;
	this->setPositionX(120 * (pos / 10) + 80);
	this->setPositionY(60 * (10 - pos % 10) + 5);
	this->desX = 120 * (des / 10) + 80;
	this->desY = 60 * (10 - des % 10) + 5;
	noteListener = EventListenerTouchOneByOne::create();
	noteListener->setSwallowTouches(true);//一次触摸只对一个有效
	switch (type)
	{
	case CLICK:
		this->initWithFile("gameSceneUI/note0.png");
		this->length = TIME_PRELOAD;
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
		this->length = TIME_PRELOAD;
		this->setRotation(atan2(desX - getPositionX(), desY - getPositionY()) * 180 / M_PI);
		noteListener->onTouchBegan = CC_CALLBACK_2(Note::onTouchBegan, this);
		noteListener->onTouchMoved = CC_CALLBACK_2(Note::onTouchMoved, this);
		noteListener->onTouchEnded = CC_CALLBACK_2(Note::onTouchEnded, this);
		break;
	}
	this->setOpacity(200);
	this->runAction(FadeTo::create(1, 255));
	judgePic = Sprite::create("gameSceneUI/judge.png");
	judgePic->setScale(2);
	judgePic->runAction(ScaleTo::create(life / 60.0, 1));
	judgePic->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
	this->addChild(judgePic);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(noteListener, this->getTag());
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
		if (!isActivated)
		{
			isActivated = true;
			life = this->length;
			judgePic->runAction(ScaleTo::create(life / 60.0, 0));
			if (type == LONGPRESS)
				this->runAction(RotateBy::create(life / 60.0, 360));
			if (isTouched)
				this->lifeTouchBegan = life;
		}
		else
			this->judge();
	}
}

void Note::judge(float slideAngle)
{
	float lifePercent;
	int judgeResult;

	this->stopAllActions();//停止所有动作
	this->unscheduleAllSelectors();//停止所有计算
	Director::getInstance()->getEventDispatcher()->removeEventListener(this->noteListener);//取消动作监听
	this->runAction(Sequence::create(FadeOut::create(0.2f), DelayTime::create(0.2f), CallFunc::create(CC_CALLBACK_0(Note::removeNote, this)), NULL));//消失特效

	switch (type)
	{
	case CLICK:
		if (isActivated)
			lifePercent = (float)(TIME_PRELOAD - life) / TIME_PRELOAD;
		else
			lifePercent = (float)life / TIME_PRELOAD;
		if (lifePercent >= 0.8)//太早或太晚都是miss
			judgeResult = 0;
		else if (lifePercent >= 0.2)//正中点前后40%开外只能是good
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
		lifePercent = abs(slideAngle - this->getRotation());
		if (!isSlided || lifePercent > 36)
			judgeResult = 0;
		else if (lifePercent > 18)
			judgeResult = 1;
		else
			judgeResult = 2;
		break;
	}
	judgePic->stopAllActions();
	judgePic->setScale(1);
	if (judgeResult == 0)
		judgePic->setTexture("gameSceneUI/halo0.png");
	else if (judgeResult == 1)
		judgePic->setTexture("gameSceneUI/halo1.png");
	else
		judgePic->setTexture("gameSceneUI/halo2.png");
	judgePic->runAction(FadeOut::create(0.4f));
	GameScene::judgeNote(judgeResult);
}

bool Note::onTouchBegan(Touch *touch, Event  *event)
{
	Point locationInNode = this->convertToNodeSpace(touch->getLocation());
	Size s = this->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode) && !Director::getInstance()->isPaused() && isTouched == false)
	{
		isTouched = true;
		judgePic->setOpacity(0);
		if (type == CLICK)
			this->judge();
		else if (type == LONGPRESS)
		{
			this->setScale(1.1f);
			if (isActivated)
				this->lifeTouchBegan = this->life;
		}
		return true;
	}
	else if (type == SLIDE)
		return true;
	return false;
}
void Note::onTouchMoved(Touch *touch, Event  *event)
{
	Point locationInNode = this->convertToNodeSpace(touch->getLocation());
	Size s = this->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode) && !Director::getInstance()->isPaused() && isSlided == false)//滑动经过音符内时标记为划过
	{
		isSlided = true;
	}
}
void Note::onTouchEnded(Touch *touch, Event  *event)
{
	Point locationInNode = this->convertToNodeSpace(touch->getLocation());
	Size s = this->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	float slideAngle = atan2(touch->getLocation().x - touch->getStartLocation().x, touch->getLocation().y - touch->getStartLocation().y) * 180 / M_PI;
	if (rect.containsPoint(locationInNode) && !Director::getInstance()->isPaused())//提前松手时进行长按音符的判定
	{
		if (type == LONGPRESS)
			this->judge();
		else if (touch->getLocation().getDistance(this->getPosition()) >= s.width)
			this->judge(slideAngle);
	}
	else if (type == SLIDE&&isSlided&& touch->getLocation().getDistance(this->getPosition()) >= s.width)
	{
		this->judge(slideAngle);
	}
}
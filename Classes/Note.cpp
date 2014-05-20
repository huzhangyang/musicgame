#include "Note.h"
#include "MapUtils.h"
#include "GameScene.h"

EventListenerTouchOneByOne *noteListener;
int notenumber;
int preloadTime;


Note::Note()
{
}

Note::~Note()
{
}

Note* Note::createNote(int type, int length, int posX, int posY)
{
	Note *note = new Note();
	if (note)
	{
		note->initNote(type, length, posX, posY);
		note->scheduleUpdate();
		note->autorelease();
		return note;
	}
	CC_SAFE_DELETE(note);
	return NULL;
}

void Note::initNote(int type, int length, int posX, int posY)
{
	this->type = (NoteType)type;
	this->life = preloadTime;
	this->lifeTouchBegan = 0;
	this->isActivated = false;
	this->isTouched = false;
	this->isSlided = false;
	this->slideAngle = 720;
	this->slideDistance = 0;
	this->setPositionX(posX);
	this->setPositionY(posY);
	switch (type)
	{
	case CLICK:
		this->initWithFile("game/note0.png");
		this->length = preloadTime;
		break;
	case LONGPRESS:
		this->initWithFile("game/note1.png");
		this->length = length;
		break;
	case SLIDE:
		this->initWithFile("game/note2.png");
		this->length = preloadTime;
		this->setRotation(atan2(MapUtils::getNextPos().x - getPositionX(), MapUtils::getNextPos().y - getPositionY()) * 180 / M_PI);
		break;
	}
	this->setOpacity(0);
	this->runAction(FadeTo::create(life / 60.0, 200));
	this->setLocalZOrder(notenumber - (++counter.total));//调整显示和响应顺序
	if (!noteListener)
		createNoteListener();
	else addToNoteListener();
	judgePic = Sprite::create("game/judge.png");
	judgePic->setScale(2);
	judgePic->runAction(EaseSineIn::create(ScaleTo::create(life / 60.0, 1)));
	judgePic->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
	this->addChild(judgePic);
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
			judgePic->runAction(EaseSineOut::create(ScaleTo::create(life / 60.0, 0)));
			if (type == LONGPRESS)
				this->runAction(EaseSineInOut::create(RotateBy::create(life / 60.0, 360)));
			if (isTouched)
				this->lifeTouchBegan = life;
		}
		else
			this->judge();
	}
}

void Note::judge()
{
	float lifePercent;
	int judgeResult = 0;

	this->stopAllActions();//停止所有动作
	this->isTouched = true;//不再接受新的触摸事件
	this->unscheduleAllSelectors();//停止所有计算
	this->runAction(Sequence::create(FadeOut::create(0.2f), DelayTime::create(0.2f), CallFunc::create(CC_CALLBACK_0(Note::removeNote, this)), NULL));//消失特效

	switch (type)
	{
	case CLICK:
		if (isActivated)
			lifePercent = (float)(preloadTime - life) / preloadTime;
		else
			lifePercent = (float)life / preloadTime;
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
		auto rot = this->getRotation();
		if (rot >= 0 && slideAngle >= 0 || rot<0 && slideAngle<0)
			lifePercent = abs(rot - slideAngle);
		else
			lifePercent = abs(slideAngle) + abs(rot);
		if (lifePercent > 180)
			lifePercent = 360 - lifePercent;
		if (!isSlided || lifePercent > 36 || slideDistance<50)//没划过、方向太偏、距离太短为miss
			judgeResult = 0;
		else if (lifePercent > 18 || slideDistance < 100)
			judgeResult = 1;
		else
			judgeResult = 2;
		break;
	}
	judgePic->stopAllActions();
	judgePic->setScale(1);
	if (judgeResult == 0)
		judgePic->setTexture("game/halo0.png");
	else if (judgeResult == 1)
		judgePic->setTexture("game/halo1.png");
	else
		judgePic->setTexture("game/halo2.png");
	judgePic->runAction(FadeOut::create(0.4f));
	GameScene::judgeNote(judgeResult);
	//log("%d %d %d %d", notenumber - this->getLocalZOrder(), this->type, this->life, judgeResult);
}

void Note::createNoteListener()
{
	auto noteListener = EventListenerTouchOneByOne::create();
	noteListener->setSwallowTouches(true);//一次触摸只对一个有效
	noteListener->onTouchBegan = [](Touch *touch, Event  *event)
	{
		auto target = dynamic_cast<Note*>(event->getCurrentTarget());//获取的当前触摸的目标 
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode) && !target->isTouched)
		{
			target->isTouched = true;
			target->judgePic->stopAllActions();
			target->judgePic->setScale(0);
			if (target->type == CLICK)
				target->judge();
			else if (target->type == LONGPRESS)
			{
				target->setScale(1.1f);
				if (target->isActivated)
					target->lifeTouchBegan = target->life;
			}
			return true;
		}
		return false;
	};
	noteListener->onTouchMoved = [](Touch *touch, Event  *event)
	{
		auto target = dynamic_cast<Note*>(event->getCurrentTarget());//获取的当前触摸的目标 
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode) && target->type == SLIDE)//滑动经过音符内时标记为划过
		{
			target->isSlided = true;
		}
		if (target->isSlided)
		{
			target->slideAngle = atan2(touch->getLocation().x - touch->getStartLocation().x, touch->getLocation().y - touch->getStartLocation().y) * 180 / M_PI;
			target->slideDistance = touch->getLocation().getDistance(touch->getStartLocation());
		}
	};
	noteListener->onTouchEnded = [](Touch *touch, Event  *event)
	{
		auto target = dynamic_cast<Note*>(event->getCurrentTarget());//获取的当前触摸的目标 
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode) && target->type == LONGPRESS &&target->life > 0)//提前松手时进行长按音符的判定
		{
			if (!target->isActivated)
			{
				target->isSlided = false;
				target->isTouched = false;
				target->setScale(1);
				target->lifeTouchBegan = 0;
			}
			else
				target->judge();
		}
		else if (target->type == SLIDE&&target->life > 0)
		{
			target->judge();
		}

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(noteListener, this);
}

void Note::addToNoteListener()
{
	_eventDispatcher->addEventListenerWithSceneGraphPriority(noteListener->clone(), this);
}

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
	this->life = TIME_PRELOAD;
	this->lifeTouchBegan = 0;
	this->length = length;
	this->status = UNTOUCHED_UNACTIVATED;
	this->setPositionX(120 * (pos / 10) + 80);
	this->setPositionY(60 * (10 - pos % 10) + 5);
	this->desX = 120 * (des / 10) + 80;
	this->desY = 60 * (10 - des % 10) + 5;
	judgePic = Sprite::create();
	auto noteListener = EventListenerTouchOneByOne::create();
	noteListener->setSwallowTouches(true);//一次触摸只对一个有效
	switch (type)
	{
	case CLICK:
		this->initWithFile("gameSceneUI/note0.png");
		this->status = UNTOUCHED_ACTIVATED;
		noteListener->onTouchBegan = CC_CALLBACK_2(Note::onTouchBegan, this);
		this->setScale(2);
		this->runAction(ScaleTo::create(TIME_PRELOAD / 60.0, 0));//出现特效
		break;
	case LONGPRESS:
		this->initWithFile("gameSceneUI/note1.png");
		noteListener->onTouchBegan = CC_CALLBACK_2(Note::onTouchBegan, this);
		noteListener->onTouchEnded = CC_CALLBACK_2(Note::onTouchEnded, this);
		this->setScale(0.5);
		this->runAction(ScaleTo::create(TIME_PRELOAD / 60.0, 1));//出现特效
		break;
	case SLIDE:
		this->initWithFile("gameSceneUI/note2.png");
		this->setRotation(atan2(desX - getPositionX(), desY - getPositionY()) * 180 / M_PI);
		judgePic->setRotation(-atan2(desX - getPositionX(), desY - getPositionY()) * 180 / M_PI);
		noteListener->onTouchBegan = CC_CALLBACK_2(Note::onTouchBegan, this);
		noteListener->onTouchMoved = CC_CALLBACK_2(Note::onTouchMoved, this);
		this->runAction(RotateBy::create(TIME_PRELOAD / 60.0, 360));//出现特效
		break;
	}
	judgePic->setScale(0.5f);
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
	if (this->type == SLIDE && (this->life % (this->length / 3) == 0) && (this->status == UNTOUCHED_ACTIVATED || this->status == TOUCHED_ACTIVATED))
		this->judge();
	life--;//减少生命
	if (life <= 0)
	{
		switch (status)
		{
		case UNTOUCHED_UNACTIVATED://预判时间过了，长按和滑动开始，普通音符消失
			life = length;
			status = UNTOUCHED_ACTIVATED;
			if (type == LONGPRESS)
				this->runAction(RotateBy::create(length / 60.0, 360));//生命周期特效
			else
				this->runAction(Sequence::create(MoveTo::create(length / 60.0, Point(desX, desY)), CallFunc::create(CC_CALLBACK_0(Note::removeNote, this)), NULL));//生命周期特效
			break;
		case TOUCHED_UNACTIVATED://预判时间过后已触摸，则开始生命周期
			life = length;
			lifeTouchBegan = length;
			status = TOUCHED_ACTIVATED;
			if (type == LONGPRESS)
				this->runAction(RotateBy::create(length / 60.0, 360));//生命周期特效
			else
				this->runAction(Sequence::create(MoveTo::create(length / 60.0, Point(desX, desY)), CallFunc::create(CC_CALLBACK_0(Note::removeNote, this)), NULL));//生命周期特效
			break;
		case UNTOUCHED_ACTIVATED://生命周期结束后仍未开始触摸，直接去结算
			this->judge();
			this->removeNote();
			break;
		case TOUCHED_ACTIVATED://生命周期结束后仍未停止触摸，直接去结算
			this->judge();
			this->removeNote();
			break;
		}
	}
}

void Note::judge()
{
	float lifePercent;
	int judgeResult;
	switch (this->getType())
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
		if (status == TOUCHED_UNACTIVATED || lifePercent <= 0.4)//触摸时间太短则判定为miss
			judgeResult = 0;
		else if (lifePercent <= 0.8)//触摸时间不够长是good
			judgeResult = 1;
		else
			judgeResult = 2;
		break;
	case SLIDE:
		Point pos = this->touchpoint;
		Size s = this->getContentSize();
		Rect rect = Rect(pos.x - s.width / 2, pos.y - s.height / 2, s.width, s.height);
		Rect rect2 = Rect(pos.x - s.width, pos.y - s.height, s.width * 2, s.height * 2);
		if (rect.containsPoint(this->getPosition()))//触摸点在音符内为perfect
			judgeResult = 2;
		else if (rect2.containsPoint(this->getPosition()))//稍稍偏离为good
			judgeResult = 1;
		else
			judgeResult = 0;
		break;
	}
	if (this->getType() != SLIDE)
	{
		this->stopAllActions();//停止所有动作
		this->unscheduleAllSelectors();//停止所有计算
		this->runAction(Sequence::create(FadeOut::create(0.2f), CallFunc::create(CC_CALLBACK_0(Note::removeNote, this)), NULL));//消失特效
	}
	if (judgeResult == 0)
		judgePic->setTexture("clearSceneUI/miss.png");
	else if (judgeResult == 1)
		judgePic->setTexture("clearSceneUI/good.png");
	else
		judgePic->setTexture("clearSceneUI/perfect.png");
	judgePic->runAction(FadeOut::create(0.2f));
	GameScene::judgeNote(judgeResult);
}

bool Note::onTouchBegan(Touch *touch, Event  *event)
{
	auto note = static_cast<Note*>(event->getCurrentTarget());
	Point pos = note->getPosition();
	Size s = note->getContentSize();
	Rect rect = Rect(pos.x - s.width / 2, pos.y - s.height / 2, s.width, s.height);
	if (rect.containsPoint(touch->getLocation()) && !Director::getInstance()->isPaused())
	{
		if (note->getStatus() == UNTOUCHED_UNACTIVATED)//预判时按下，状态变为按下_未激活
		{
			note->setStatus(TOUCHED_UNACTIVATED);
			if (note->getType() == SLIDE)//对滑动note，保存此时触摸位置
				note->setTouchPoint(touch->getLocation());
			return true;
		}
		else if (note->getStatus() == UNTOUCHED_ACTIVATED)//已经开始生命周期时按下，状态变为按下_激活
		{
			note->setStatus(TOUCHED_ACTIVATED);
			if (note->getType() == CLICK)//对普通note，直接进行判定
				note->judge();
			else if (note->getType() == LONGPRESS)//对长按note,记录此时生命值
				note->setLifeTouchBegan(note->getLife());
			return true;
		}
	}
	return false;
}
void Note::onTouchMoved(Touch *touch, Event  *event)
{
	auto note = static_cast<Note*>(event->getCurrentTarget());
	if (!Director::getInstance()->isPaused() && note->getStatus() == TOUCHED_ACTIVATED)
	{
		note->setTouchPoint(touch->getLocation());//保存触摸位置
	}
}
void Note::onTouchEnded(Touch *touch, Event  *event)
{
	auto note = static_cast<Note*>(event->getCurrentTarget());
	if (!Director::getInstance()->isPaused() && note->getLife() != 0)//提前松手时进行长按与滑动音符的判定
		note->judge();
}

int Note::getDesX(){ return this->desX; }
int Note::getDesY(){ return this->desY; }
int Note::getLife(){ return this->life; }
int Note::getLength(){ return this->length; }
NoteType Note::getType(){ return this->type; }
NoteStatus Note::getStatus(){ return this->status; }
void Note::setLifeTouchBegan(int lifeTouchBegan){ this->lifeTouchBegan = lifeTouchBegan; }
void Note::setStatus(NoteStatus status){ this->status = status; }
void Note::setTouchPoint(Point point){ this->touchpoint = point; }
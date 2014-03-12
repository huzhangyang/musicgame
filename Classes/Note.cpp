#include "Note.h"
#include "MapUtils.h"
#include "GameScene.h"

EventListenerTouchOneByOne *noteListener;
int notenumber;
int TIME_PRELOAD = 7200 / BPM;

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
	this->life = TIME_PRELOAD;
	this->lifeTouchBegan = 0;
	this->isActivated = false;
	this->isTouched = false;
	this->isSlided = false;
	this->setScale(1.25);
	this->setPositionX(posX);
	this->setPositionY(posY);
	switch (type)
	{
	case CLICK:
		this->initWithFile("game/note0.png");
		this->length = TIME_PRELOAD;
		break;
	case LONGPRESS:
		this->initWithFile("game/note1.png");
		this->length = length;
		break;
	case SLIDE:
		this->initWithFile("game/note2.png");
		this->length = TIME_PRELOAD;
		this->setRotation(atan2(MapUtils::getNextPos().x - getPositionX(), MapUtils::getNextPos().y - getPositionY()) * 180 / M_PI);
		break;
	}
	this->setOpacity(0);
	this->runAction(FadeTo::create(life / 60.0, 200));
	this->setLocalZOrder(notenumber - (++counter.total));//������ʾ����Ӧ˳��
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
	life--;//��������
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

void Note::judge(float slideAngle, float slideDistance)
{
	float lifePercent;
	int judgeResult;

	this->stopAllActions();//ֹͣ���ж���
	this->isTouched = true;//���ٽ����µĴ����¼�
	this->unscheduleAllSelectors();//ֹͣ���м���
	this->runAction(Sequence::create(FadeOut::create(0.2f), DelayTime::create(0.2f), CallFunc::create(CC_CALLBACK_0(Note::removeNote, this)), NULL));//��ʧ��Ч

	switch (type)
	{
	case CLICK:
		if (isActivated)
			lifePercent = (float)(TIME_PRELOAD - life) / TIME_PRELOAD;
		else
			lifePercent = (float)life / TIME_PRELOAD;
		if (lifePercent >= 0.8)//̫���̫����miss
			judgeResult = 0;
		else if (lifePercent >= 0.2)//���е�ǰ��40%����ֻ����good
			judgeResult = 1;
		else
			judgeResult = 2;
		break;
	case LONGPRESS:
		lifePercent = (float)(lifeTouchBegan - life) / length;
		if (lifePercent <= 0.4)//����ʱ��̫�����ж�Ϊmiss
			judgeResult = 0;
		else if (lifePercent <= 0.8)//����ʱ�䲻������good
			judgeResult = 1;
		else
			judgeResult = 2;
		break;
	case SLIDE:
		lifePercent = abs(slideAngle - this->getRotation());
		if (!isSlided || lifePercent > 36 || slideDistance<50)//û����������̫ƫ������̫��Ϊmiss
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
	noteListener->setSwallowTouches(true);//һ�δ���ֻ��һ����Ч
	noteListener->onTouchBegan = [](Touch *touch, Event  *event)
	{
		auto target = dynamic_cast<Note*>(event->getCurrentTarget());//��ȡ�ĵ�ǰ������Ŀ�� 
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode) && !target->isTouched)
		{
			target->isTouched = true;
			target->judgePic->setOpacity(0);
			if (target->type == CLICK)
				target->judge();
			else if (target->type == LONGPRESS)
			{
				target->setScale(1.5f);
				if (target->isActivated)
					target->lifeTouchBegan = target->life;
			}
			return true;
		}
		//else if (target->type == SLIDE&& !target->isTouched)
		//return true;
		return false;
	};
	noteListener->onTouchMoved = [](Touch *touch, Event  *event)
	{
		auto target = dynamic_cast<Note*>(event->getCurrentTarget());//��ȡ�ĵ�ǰ������Ŀ�� 
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode) && !target->isSlided)//��������������ʱ���Ϊ����
		{
			target->isSlided = true;
		}
	};
	noteListener->onTouchEnded = [](Touch *touch, Event  *event)
	{
		auto target = dynamic_cast<Note*>(event->getCurrentTarget());//��ȡ�ĵ�ǰ������Ŀ�� 
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode) && target->type == LONGPRESS &&target->life > 0)//��ǰ����ʱ���г����������ж�
		{
			if (!target->isActivated)
			{
				target->isSlided = false;
				target->isTouched = false;
				target->setScale(1.25f);
				target->lifeTouchBegan = 0;
				target->judgePic->setOpacity(255);
			}
			else
				target->judge();
		}
		else if (target->type == SLIDE&&target->life > 0)
		{
			float slideAngle = atan2(touch->getLocation().x - touch->getStartLocation().x, touch->getLocation().y - touch->getStartLocation().y) * 180 / M_PI;
			float slideDistance = touch->getLocation().getDistance(target->getPosition());
			target->judge(slideAngle, slideDistance);
		}

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(noteListener, this);
}

void Note::addToNoteListener()
{
	_eventDispatcher->addEventListenerWithSceneGraphPriority(noteListener->clone(), this);
}

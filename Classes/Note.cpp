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
	noteListener->setSwallowTouches(true);//һ�δ���ֻ��һ����Ч
	switch (type)
	{
	case CLICK:
		this->initWithFile("gameSceneUI/note0.png");
		this->status = UNTOUCHED_ACTIVATED;
		noteListener->onTouchBegan = CC_CALLBACK_2(Note::onTouchBegan, this);
		this->setScale(2);
		this->runAction(ScaleTo::create(TIME_PRELOAD / 60.0, 0));//������Ч
		break;
	case LONGPRESS:
		this->initWithFile("gameSceneUI/note1.png");
		noteListener->onTouchBegan = CC_CALLBACK_2(Note::onTouchBegan, this);
		noteListener->onTouchEnded = CC_CALLBACK_2(Note::onTouchEnded, this);
		this->setScale(0.5);
		this->runAction(ScaleTo::create(TIME_PRELOAD / 60.0, 1));//������Ч
		break;
	case SLIDE:
		this->initWithFile("gameSceneUI/note2.png");
		this->setRotation(atan2(desX - getPositionX(), desY - getPositionY()) * 180 / M_PI);
		judgePic->setRotation(-atan2(desX - getPositionX(), desY - getPositionY()) * 180 / M_PI);
		noteListener->onTouchBegan = CC_CALLBACK_2(Note::onTouchBegan, this);
		noteListener->onTouchMoved = CC_CALLBACK_2(Note::onTouchMoved, this);
		this->runAction(RotateBy::create(TIME_PRELOAD / 60.0, 360));//������Ч
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
	life--;//��������
	if (life <= 0)
	{
		switch (status)
		{
		case UNTOUCHED_UNACTIVATED://Ԥ��ʱ����ˣ������ͻ�����ʼ����ͨ������ʧ
			life = length;
			status = UNTOUCHED_ACTIVATED;
			if (type == LONGPRESS)
				this->runAction(RotateBy::create(length / 60.0, 360));//����������Ч
			else
				this->runAction(Sequence::create(MoveTo::create(length / 60.0, Point(desX, desY)), CallFunc::create(CC_CALLBACK_0(Note::removeNote, this)), NULL));//����������Ч
			break;
		case TOUCHED_UNACTIVATED://Ԥ��ʱ������Ѵ�������ʼ��������
			life = length;
			lifeTouchBegan = length;
			status = TOUCHED_ACTIVATED;
			if (type == LONGPRESS)
				this->runAction(RotateBy::create(length / 60.0, 360));//����������Ч
			else
				this->runAction(Sequence::create(MoveTo::create(length / 60.0, Point(desX, desY)), CallFunc::create(CC_CALLBACK_0(Note::removeNote, this)), NULL));//����������Ч
			break;
		case UNTOUCHED_ACTIVATED://�������ڽ�������δ��ʼ������ֱ��ȥ����
			this->judge();
			this->removeNote();
			break;
		case TOUCHED_ACTIVATED://�������ڽ�������δֹͣ������ֱ��ȥ����
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
		if (lifePercent <= 0.1 || lifePercent >= 0.9)//̫���̫����miss
			judgeResult = 0;
		else if (lifePercent <= 0.3 || lifePercent >= 0.7)//���е�ǰ��40%����ֻ����good
			judgeResult = 1;
		else
			judgeResult = 2;
		break;
	case LONGPRESS:
		lifePercent = (float)(lifeTouchBegan - life) / length;
		if (status == TOUCHED_UNACTIVATED || lifePercent <= 0.4)//����ʱ��̫�����ж�Ϊmiss
			judgeResult = 0;
		else if (lifePercent <= 0.8)//����ʱ�䲻������good
			judgeResult = 1;
		else
			judgeResult = 2;
		break;
	case SLIDE:
		Point pos = this->touchpoint;
		Size s = this->getContentSize();
		Rect rect = Rect(pos.x - s.width / 2, pos.y - s.height / 2, s.width, s.height);
		Rect rect2 = Rect(pos.x - s.width, pos.y - s.height, s.width * 2, s.height * 2);
		if (rect.containsPoint(this->getPosition()))//��������������Ϊperfect
			judgeResult = 2;
		else if (rect2.containsPoint(this->getPosition()))//����ƫ��Ϊgood
			judgeResult = 1;
		else
			judgeResult = 0;
		break;
	}
	if (this->getType() != SLIDE)
	{
		this->stopAllActions();//ֹͣ���ж���
		this->unscheduleAllSelectors();//ֹͣ���м���
		this->runAction(Sequence::create(FadeOut::create(0.2f), CallFunc::create(CC_CALLBACK_0(Note::removeNote, this)), NULL));//��ʧ��Ч
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
		if (note->getStatus() == UNTOUCHED_UNACTIVATED)//Ԥ��ʱ���£�״̬��Ϊ����_δ����
		{
			note->setStatus(TOUCHED_UNACTIVATED);
			if (note->getType() == SLIDE)//�Ի���note�������ʱ����λ��
				note->setTouchPoint(touch->getLocation());
			return true;
		}
		else if (note->getStatus() == UNTOUCHED_ACTIVATED)//�Ѿ���ʼ��������ʱ���£�״̬��Ϊ����_����
		{
			note->setStatus(TOUCHED_ACTIVATED);
			if (note->getType() == CLICK)//����ͨnote��ֱ�ӽ����ж�
				note->judge();
			else if (note->getType() == LONGPRESS)//�Գ���note,��¼��ʱ����ֵ
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
		note->setTouchPoint(touch->getLocation());//���津��λ��
	}
}
void Note::onTouchEnded(Touch *touch, Event  *event)
{
	auto note = static_cast<Note*>(event->getCurrentTarget());
	if (!Director::getInstance()->isPaused() && note->getLife() != 0)//��ǰ����ʱ���г����뻬���������ж�
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
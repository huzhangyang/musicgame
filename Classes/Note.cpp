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
	this->setPositionX(120 * (pos / 10) + 80);
	this->setPositionY(60 * (10 - pos % 10) + 5);
	this->desX = 120 * (des / 10) + 80;
	this->desY = 60 * (10 - des % 10) + 5;
	noteListener = EventListenerTouchOneByOne::create();
	noteListener->setSwallowTouches(true);//һ�δ���ֻ��һ����Ч
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
		this->length = length;
		this->setRotation(atan2(desX - getPositionX(), desY - getPositionY()) * 180 / M_PI);
		noteListener->onTouchBegan = CC_CALLBACK_2(Note::onTouchBegan, this);
		noteListener->onTouchMoved = CC_CALLBACK_2(Note::onTouchMoved, this);
		break;
	}
	this->setOpacity(200);
	this->runAction(FadeTo::create(1, 255));
	judgePic = Sprite::create("gameSceneUI/judge.png");
	judgePic->setScale(2);
	judgePic->setLocalZOrder(5);
	judgePic->runAction(ScaleTo::create(TIME_PRELOAD / 60.0, 1));
	judgePic->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
	this->addChild(judgePic);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(noteListener, this->getTag());
}

void Note::removeNote()
{
	this->removeFromParent();
	Director::getInstance()->getEventDispatcher()->removeEventListener(this->noteListener);
}

void Note::update(float dt)
{
	life--;//��������
	if (life <= 0)
	{
		if (!isActivated)
		{
			judgePic->runAction(ScaleTo::create(life / 60.0, 0));
			isActivated = true;
			this->life = this->length;
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
	int judgeResult;

	this->stopAllActions();//ֹͣ���ж���
	this->unscheduleAllSelectors();//ֹͣ���м���
	this->runAction(Sequence::create(FadeOut::create(0.2f), CallFunc::create(CC_CALLBACK_0(Note::removeNote, this)), NULL));//��ʧ��Ч

	switch (type)
	{
	case CLICK:
		lifePercent = (float)life / (TIME_PRELOAD * 2);
		if (lifePercent <= 0.1 || lifePercent >= 0.9)//̫���̫����miss
			judgeResult = 0;
		else if (lifePercent <= 0.3 || lifePercent >= 0.7)//���е�ǰ��40%����ֻ����good
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
		lifePercent = (float)life / length * 2;
		if (lifePercent <= 0.1 || lifePercent >= 0.9)//̫���̫����miss
			judgeResult = 0;
		else if (lifePercent <= 0.3 || lifePercent >= 0.7)//���е�ǰ��40%����ֻ����good
			judgeResult = 1;
		else
			judgeResult = 2;
		break;
	}
	judgePic->setScale(1);
	if (judgeResult == 0)
		judgePic->setTexture("gameSceneUI/halo0.png");
	else if (judgeResult == 1)
		judgePic->setTexture("gameSceneUI/halo1.png");
	else
		judgePic->setTexture("gameSceneUI/halo2.png");
	judgePic->runAction(FadeOut::create(0.2f));
	GameScene::judgeNote(judgeResult);
}

bool Note::onTouchBegan(Touch *touch, Event  *event)
{
	Point pos = this->getPosition();
	Size s = this->getContentSize();
	Rect rect = Rect(pos.x - s.width / 2, pos.y - s.height / 2, s.width, s.height);
	if (rect.containsPoint(touch->getLocation()) && !Director::getInstance()->isPaused() && isTouched == false)
	{
		isTouched = true;
		if (this->type == CLICK)//����ͨnote��ֱ�ӽ����ж�
			this->judge();
		else if (isActivated)//���ѿ�ʼ�������ڵĳ���note,��¼��ʱ����ֵ
			this->lifeTouchBegan = this->life;
		return true;
	}
	return false;
}
void Note::onTouchMoved(Touch *touch, Event  *event)
{
	Point pos = this->getPosition();
	Size s = this->getContentSize();
	Rect rect = Rect(pos.x - s.width / 2, pos.y - s.height / 2, s.width, s.height);
	if (rect.containsPoint(touch->getLocation()) && !Director::getInstance()->isPaused() && this->life != 0)//��������������ʱ���л����������ж�
	{
		this->judge();
	}
}
void Note::onTouchEnded(Touch *touch, Event  *event)
{
	if (!Director::getInstance()->isPaused() && this->life != 0)//��ǰ����ʱ���г����������ж�
		this->judge();
}
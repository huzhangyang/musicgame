#include "Note.h"
#include "GameScene.h"

const int TIME_PRELOAD = 60;//用于反应的时间

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

Note::Note()
{
}

Note::~Note()
{
}

Note* Note::createNote(int type, int pos, int des)
{
	Note *note = new Note();
	if (note)
	{
		note->initNote(type, pos, des);
		note->scheduleUpdate();
		note->autorelease();
		return note;
	}
	CC_SAFE_DELETE(note);
	return NULL;
}

void Note::initNote(int type, int pos, int des)
{
	this->type = type;
	this->life = TIME_PRELOAD;
	this->lifeTouched = 0;
	this->touched = false;
	this->setScale(0.5);
	this->runAction(EaseBackOut::create(ScaleTo::create(life / 180.0, 1)));//出现特效
	switch (type)
	{
	case 0:
		this->initWithFile("gameSceneUI/note0.png");
		this->lifeSpan = 0;
		break;
	case 1:
		this->initWithFile("gameSceneUI/note1.png");
		this->lifeSpan = 120;
		break;
	case 2:
		this->initWithFile("gameSceneUI/note2.png");
		this->lifeSpan = 60;
		break;
	}
	switch (pos / 10)
	{
	case 1:this->setPositionX(POS_X1); break;
	case 2:this->setPositionX(POS_X2); break;
	case 3:this->setPositionX(POS_X3); break;
	case 4:this->setPositionX(POS_X4); break;
	case 5:this->setPositionX(POS_X5); break;
	case 6:this->setPositionX(POS_X6); break;
	default: break;
	}
	switch (pos % 10)
	{
	case 1:this->setPositionY(POS_Y1); break;
	case 2:this->setPositionY(POS_Y2); break;
	case 3:this->setPositionY(POS_Y3); break;
	case 4:this->setPositionY(POS_Y4); break;
	case 5:this->setPositionY(POS_Y5); break;
	default: break;
	}
	switch (des / 10)
	{
	case 1:this->destX = POS_X1; break;
	case 2:this->destX = POS_X2; break;
	case 3:this->destX = POS_X3; break;
	case 4:this->destX = POS_X4; break;
	case 5:this->destX = POS_X5; break;
	case 6:this->destX = POS_X6; break;
	default: break;
	}
	switch (des % 10)
	{
	case 1:this->destY = POS_Y1; break;
	case 2:this->destY = POS_Y2; break;
	case 3:this->destY = POS_Y3; break;
	case 4:this->destY = POS_Y4; break;
	case 5:this->destY = POS_Y5; break;
	default: break;
	}
}

void Note::removeNote(float dt)
{
	this->removeFromParentAndCleanup(true);
}

void Note::update(float dt)
{
	if (isTouched())//如果被触摸过，则增加触摸时间
	{
		if (lifeTouched == 0)//第一次先把生命设为lifespan
			life = lifeSpan;
		lifeTouched++;
	}
	life--;//减少生命
	if (life <= 0)
	{
		if (!isTouched())//如果到死都没被触摸过则肯定是miss，否则就是按到头没有松手，送去judge
			GameScene::judgeNote(0);
		else
			this->judge();
		this->removeFromParentAndCleanup(true);//删除该note
	}
}

void Note::judge()
{
	float lifePercent;
	switch (this->getType())
	{
	case 0:
		lifePercent = (float)this->getLife() / TIME_PRELOAD;
		if (lifePercent >= 0.8 || lifePercent <= 0.4)//太快或太慢触摸都只能是good
			GameScene::judgeNote(1);
		else
			GameScene::judgeNote(2);
		break;
	case 1:
		lifePercent = (float)this->getLifeTouched() / (float)this->getLifeSpan();
		if (lifePercent <= 0.8)//长按的触摸时间不够长是good
			GameScene::judgeNote(1);
		else
			GameScene::judgeNote(2);
		break;
	case 2:
		Size s = getContentSize();
		Point dest = Point(destX, destY);//目标点
		Rect rect = Rect(getPositionX() - s.width / 2, getPositionY() - s.height / 2, s.width, s.height);//当前note矩形
		if (rect.containsPoint(dest))//检测目标点是否在note内
		{
			lifePercent = (float)this->getLifeTouched() / (float)this->getLifeSpan();
			if (lifePercent <= 0.8)//如果滑动用时过短也只能是good
				GameScene::judgeNote(1);
			else
				GameScene::judgeNote(2);
		}
		else
			GameScene::judgeNote(0);//没包含住目标点则为miss
		break;
	}
}

int Note::getType(){ return this->type; }
int Note::getDestX(){ return this->destX; }
int Note::getDestY(){ return this->destY; }
int Note::getLife(){ return this->life; }
int Note::getLifeSpan(){ return this->lifeSpan; }
int Note::getLifeTouched(){ return this->lifeTouched; }
void Note::setTouched(){ this->touched = true; }
bool Note::isTouched(){ return this->touched; }
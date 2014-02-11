#include "Note.h"
#include "GameScene.h"

const int TIME_PRELOAD = 60;//用于反应的时间


const int POS_X1 = 200;
const int POS_X2 = 320;
const int POS_X3 = 440;
const int POS_X4 = 560;
const int POS_X5 = 680;
const int POS_X6 = 800;
const int POS_X7 = 920;
const int POS_X8 = 1040;
const int POS_X9 = 1160;

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

Note* Note::createNote(int type, int length,int pos, int des)
{
	Note *note = new Note();
	if (note)
	{
		note->initNote(type,length, pos, des);
		note->scheduleUpdate();
		note->autorelease();
		return note;
	}
	CC_SAFE_DELETE(note);
	return NULL;
}

void Note::initNote(int type,int length, int pos, int des)
{
	this->type = type;
	this->life = TIME_PRELOAD;
	this->lifeSpan = length;
	this->touched = false;
	this->inLifeSpan = false;
	switch (type)
	{
	case 0:
		this->initWithFile("gameSceneUI/note0.png");
		//this->setScale(2.5);
		//this->runAction(ScaleTo::create(life / 60.0, 0));//出现特效
		break;
	case 1:
		this->initWithFile("gameSceneUI/note1.png");
		this->runAction(ScaleTo::create(life / 60.0, 1.25));//出现特效
		break;
	case 2:
		this->initWithFile("gameSceneUI/note2.png");
		this->runAction(RotateBy::create(life / 60.0, 360));//出现特效
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
	case 7:this->setPositionX(POS_X7); break;
	case 8:this->setPositionX(POS_X8); break;
	case 9:this->setPositionX(POS_X9); break;
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
	case 1:this->destX=POS_X1; break;
	case 2:this->destX=POS_X2; break;
	case 3:this->destX=POS_X3; break;
	case 4:this->destX=POS_X4; break;
	case 5:this->destX=POS_X5; break;
	case 6:this->destX=POS_X6; break;
	case 7:this->destX=POS_X7; break;
	case 8:this->destX=POS_X8; break;
	case 9:this->destX=POS_X9; break;
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
	life--;//减少生命
	if (life <= 0)
	{
		if (!touched)//如果到死都没被触摸过则肯定是miss
		{
			GameScene::judgeNote(0);
			this->removeFromParentAndCleanup(true);//删除该note
		}
		else if (!inLifeSpan)//否则开始生命周期
		{
			life = lifeSpan;
			inLifeSpan = true;
			if (type == 1)
				this->runAction(RotateBy::create(lifeSpan / 60.0, 360));//生命周期特效
			else
				this->runAction(MoveTo::create(lifeSpan / 60.0, Point(destX, destY)));//生命周期特效
		}
		else//生命周期结束还未松手就去结算
		{
			this->judge();
			this->removeFromParentAndCleanup(true);//删除该note
		}
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
		lifePercent = 1 - (float)life / lifeSpan;
		if (!inLifeSpan || lifePercent <= 0.4)//长按的触摸时间太短则判定为miss
			GameScene::judgeNote(0);
		else if (lifePercent <= 0.8)//长按的触摸时间不够长是good
			GameScene::judgeNote(1);
		else
			GameScene::judgeNote(2);
		break;
	case 2:
		break;
	}
}

int Note::getType(){ return this->type; }
int Note::getDestX(){ return this->destX; }
int Note::getDestY(){ return this->destY; }
int Note::getLife(){ return this->life; }
int Note::getLifeSpan(){ return this->lifeSpan; }
void Note::setTouched(){ this->touched = true; }
bool Note::isTouched(){ return this->touched; }
bool Note::isInLifeSpan(){ return this->inLifeSpan; }
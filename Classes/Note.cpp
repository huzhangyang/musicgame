#include "Note.h"
#include "GameScene.h"

const int TIME_PRELOAD = 30;//音符提前出现的时间
const int TIME_DEADLINE = 30;//音符最大允许延迟按下的时间

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
	this->length = length;
	this->status = UNTOUCHED_PRELOAD;
	switch (type)
	{
	case CLICK:
		this->initWithFile("gameSceneUI/note0.png");
		this->setScale(2);
		this->runAction(ScaleTo::create(TIME_PRELOAD / 60.0, 1));//出现特效
		break;
	case LONGPRESS:
		this->initWithFile("gameSceneUI/note1.png");
		this->setScale(0.5);
		this->runAction(ScaleTo::create(TIME_PRELOAD / 60.0, 1));//出现特效
		break;
	case SLIDE:
		this->initWithFile("gameSceneUI/note2.png");
		this->runAction(RotateBy::create(TIME_PRELOAD / 60.0, 360));//出现特效
		break;
	}
	this->setPositionX(120 * (pos / 10) + 80);
	this->setPositionY(60 * (10 - pos % 10) + 5);
	this->destX = 120 * (des / 10) + 80;
	this->destY = 60 * (10 - des % 10) + 5;
}

void Note::removeNote()
{
	this->removeFromParent();
}

void Note::update(float dt)
{
	life--;//减少生命
	if (this->type == SLIDE && this->status == TOUCHED_ACTIVATED&&this->life % (this->length / 3) == 0)
		this->judge();
	if (life <= 0)
	{
		switch (status)
		{
		case UNTOUCHED_PRELOAD://预判时间过了还没有触摸，再等待延迟时间
			life = TIME_DEADLINE;
			status = UNTOUCHED_DEADLINE;
			if (type == CLICK)
				this->runAction(ScaleTo::create(TIME_DEADLINE / 60.0, 0));//生命周期特效
			else if (type == LONGPRESS)
				this->runAction(RotateBy::create(length / 60.0, 360));//生命周期特效
			else
				this->runAction(MoveTo::create(length / 60.0, Point(destX, destY)));//生命周期特效
			break;
		case UNTOUCHED_DEADLINE://等待后仍未触摸，则计为miss
			GameScene::judgeNote(0);
			this->removeNote();
			break;
		case TOUCHED_UNACTIVATED://预判时间过后已触摸，则开始生命周期
			life = length;
			status = TOUCHED_ACTIVATED;
			if (type == LONGPRESS)
				this->runAction(RotateBy::create(length / 60.0, 360));//生命周期特效
			else
				this->runAction(MoveTo::create(length / 60.0, Point(destX, destY)));//生命周期特效
			break;
		case TOUCHED_ACTIVATED://生命周期结束后仍未停止触摸，直接去结算
			this->judge();
			break;
		}
	}
}

void Note::judge()
{
	float lifePercent;

	this->stopAllActions();//停止所有动作
	this->unscheduleAllSelectors();//停止所有计算
	this->runAction(Sequence::create(FadeOut::create(0.2f), CallFunc::create(CC_CALLBACK_0(Note::removeNote, this)), NULL));//消失特效

	switch (this->getType())
	{
	case CLICK:
		if (status == TOUCHED_UNACTIVATED)
			lifePercent = (float)life / TIME_PRELOAD;
		else
			lifePercent = (float)(TIME_DEADLINE - life) / TIME_DEADLINE;
		if (lifePercent >= 0.4)//正中点前后40%开外都只能是GOOD
			GameScene::judgeNote(1);
		else
			GameScene::judgeNote(2);
		break;
	case LONGPRESS:
		lifePercent = 1 - (float)life / length;
		if (status != TOUCHED_ACTIVATED || lifePercent <= 0.4)//长按的触摸时间太短则判定为miss
			GameScene::judgeNote(0);
		else if (lifePercent <= 0.8)//长按的触摸时间不够长是good
			GameScene::judgeNote(1);
		else
			GameScene::judgeNote(2);
		break;
	case SLIDE:
		Point pos = this->touchpoint;
		Size s = this->getContentSize();
		Rect rect = Rect(pos.x - s.width / 2, pos.y - s.height / 2, s.width, s.height);
		Rect rect2 = Rect(pos.x - s.width, pos.y - s.height, s.width * 2, s.height * 2);
		if (rect.containsPoint(pos))//触摸点在音符内为perfect
			GameScene::judgeNote(2);
		else if (rect2.containsPoint(pos))//稍稍偏离为good
			GameScene::judgeNote(1);
		else//偏离太多为miss
			GameScene::judgeNote(0);
		break;
	}
}


int Note::getDestX(){ return this->destX; }
int Note::getDestY(){ return this->destY; }
int Note::getLife(){ return this->life; }
int Note::getLength(){ return this->length; }
NoteType Note::getType(){ return this->type; }
NoteStatus Note::getStatus(){ return this->status; }
void Note::setLife(int life){ this->life = life; }
void Note::setStatus(NoteStatus status){ this->status = status; }
void Note::setTouchPoint(Point point){ this->touchpoint = point; }
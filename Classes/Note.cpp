#include "Note.h"
#include "GameScene.h"

const int LIFESPAN = 60;//×ÜÉúÃüÖ¡Êý

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
	switch (type)
	{
	case 0:
		this->initWithFile("gameSceneUI/note0.png");
		this->lifeSpan = LIFESPAN;
		this->setScale(2.5);
		this->runAction(ScaleTo::create(lifeSpan / 60.0, 0));
		break;
	case 1:
		this->initWithFile("gameSceneUI/note1.png");
		this->lifeSpan = 180;
		this->runAction(RotateBy::create(lifeSpan / 60.0, 360));
		break;
	case 2:
		this->initWithFile("gameSceneUI/note2.png");
		this->lifeSpan = 180;
		this->runAction(RotateBy::create(lifeSpan / 60.0, 360));
		break;
	}
	this->life = lifeSpan;
	this->lifeTouched = 0;
	this->touched = false;
	this->missed = true;
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
	life--;
	if (life <= 0)
	{
		if (missed)
			GameScene::judgeNote(0);
		this->removeFromParentAndCleanup(true);
	}
	else if (isTouched())
	{
		lifeTouched++;
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
void Note::setNotMissed(){ this->missed = false; }
bool Note::isMissed(){ return this->missed; }
#include "Note.h"
#include "GameScene.h"

const int LIFESPAN = 60;//×ÜÉúÃüÖ¡Êý

const int POS_X1 = 200;
const int POS_X2 = 350;
const int POS_X3 = 500;
const int POS_X4 = 650;
const int POS_X5 = 800;
const int POS_X6 = 950;
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

Note* Note::createNote(int posX, int posY, int type)
{
	Note *note = new Note();
	if (note && note->initWithFile("gameSceneUI/note.png"))
	{
		note->initNote(posX, posY, type);
		note->scheduleUpdate();
		note->runAction(ScaleTo::create(1, 0));
		note->autorelease();
		return note;
	}
	CC_SAFE_DELETE(note);
	return NULL;
}

void Note::initNote(int posX, int posY, int type)
{
	this->type = type;
	this->life = LIFESPAN;
	this->touched = false;
	this->setScale(2.5);
	switch (posX)
	{
	case 1:this->setPositionX(POS_X1); break;
	case 2:this->setPositionX(POS_X2); break;
	case 3:this->setPositionX(POS_X3); break;
	case 4:this->setPositionX(POS_X4); break;
	case 5:this->setPositionX(POS_X5); break;
	case 6:this->setPositionX(POS_X6); break;
	default: break;
	}
	switch (posY)
	{
	case 1:this->setPositionY(POS_Y1); break;
	case 2:this->setPositionY(POS_Y2); break;
	case 3:this->setPositionY(POS_Y3); break;
	case 4:this->setPositionY(POS_Y4); break;
	case 5:this->setPositionY(POS_Y5); break;
	default: break;
	}
}

void Note::update(float dt)
{
	life--;
	if (life <= 0)
	{
		if (touched == false)
			GameScene::judgeNote(0);
		this->removeFromParentAndCleanup(true);
	}
}

void Note::setLife(int life)
{
	this->life = life;
}

int Note::getLife()
{
	return this->life;
}

int Note::getType()
{
	return this->type;
}

void Note::setTouched()
{
	this->touched = true;
}

bool Note::isTouched()
{
	return this->touched;
}
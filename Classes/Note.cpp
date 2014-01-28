#include "Note.h"

const float LIFESPAN = 1.0;
const int POS_X1 = 260;
const int POS_X2 = 410;
const int POS_X3 = 560;
const int POS_X4 = 710;
const int POS_X5 = 860;
const int POS_X6 = 1010;
const int POS_Y1 = 470;
const int POS_Y2 = 380;
const int POS_Y3 = 290;
const int POS_Y4 = 200;
const int POS_Y5 = 110;

Note* Note::createAtPoint(Point p,int tag)
{
	Note *note = new Note();
	if (note && note->initWithFile("gameSceneUI/note.png"))
	{
		note->setZOrder(1);
		note->setTag(tag);
		note->setPosition(p);
		note->scheduleOnce(schedule_selector(Note::removeSelf), LIFESPAN);
		note->autorelease();
		return note;
	}
	CC_SAFE_DELETE(note);
	return NULL;
}

void Note::removeSelf(float dt)
{
	this->removeFromParentAndCleanup(true);
}
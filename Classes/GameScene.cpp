#include "GameScene.h"
#include "ClearScene.h"
#include "Note.h"
#include "MainScene.h"
#include <fstream>

const int TIME_PRELOAD = 60;//音符提前出现的时间
Noteline noteline;
Counter counter;

int difficulty;//当前难度
std::string FileName;//音乐文件名称
std::ifstream fin;//输入流
TextBMFont *labelInfo, *labelCombo, *labelJudge, *labelDifficulty;
LoadingBar *loadingBar;
Node* pauseNode;

Scene* GameScene::createScene(std::string filename)
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);

	FileName = filename;
	counter.frame = 0;
	counter.total = 0;
	counter.perfect = 0;
	counter.good = 0;
	counter.miss = 0;
	counter.combo = 0;
	counter.maxcombo = 0;
	return scene;
}

bool GameScene::init()
{
	//super init
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////////////////////////////
	auto sceneNode = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("gameScene.json");
	sceneNode->setLocalZOrder(-1);
	addChild(sceneNode);

	auto UINode = sceneNode->getChildByTag(10004);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("gameSceneUI");
	auto UIlayer = UIComponent->getNode();
	auto buttonPause = dynamic_cast<Button*>(UIlayer->getChildByTag(GAMESCENE_PAUSE));
	buttonPause->addTouchEventListener(this, toucheventselector(GameScene::touchEvent));
	labelInfo = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(GAMESCENE_INFO));
	labelCombo = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(GAMESCENE_COMBO));
	labelJudge = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(GAMESCENE_JUDGE));
	labelDifficulty = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(GAMESCENE_DIFFICULTY));
	loadingBar = dynamic_cast<LoadingBar*>(UIlayer->getChildByTag(GAMESCENE_LOADINGBAR));
	labelInfo->setText(FileName.c_str());
	difficulty = UserDefault::getInstance()->getIntegerForKey("difficulty");//获取当前难度
	if (difficulty == 0)
	{
		labelDifficulty->setText("EASY");
	}
	else if (difficulty == 1)
	{
		labelDifficulty->setText("HARD");
	}

	pauseNode = sceneNode->getChildByTag(10005);
	auto pauseComponent = (cocostudio::ComRender*) pauseNode->getComponent("pauseSelectUI");
	auto pauselayer = pauseComponent->getNode();
	auto buttonRetry = dynamic_cast<Button*>(pauselayer->getChildByTag(GAMESCENE_RETRY));
	buttonRetry->addTouchEventListener(this, toucheventselector(GameScene::touchEvent));
	auto buttonReturn = dynamic_cast<Button*>(pauselayer->getChildByTag(GAMESCENE_RETURN));
	buttonReturn->addTouchEventListener(this, toucheventselector(GameScene::touchEvent));
	auto buttonSet = dynamic_cast<Button*>(pauselayer->getChildByTag(GAMESCENE_SET));
	buttonSet->addTouchEventListener(this, toucheventselector(GameScene::touchEvent));
	auto buttonStart = dynamic_cast<Button*>(pauselayer->getChildByTag(GAMESCENE_START));
	buttonStart->addTouchEventListener(this, toucheventselector(GameScene::touchEvent));

	return true;
}

void GameScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->create("music/test.mp3");
	fin.open(FileUtils::getInstance()->fullPathForFilename(FileName + ".gnm"));//打开测试谱面
	getNoteline();//读取第一行

	labelJudge->setText("Get Ready");
	labelJudge->runAction(FadeOut::create(3));
	loadingBar->setPercent(0);
	this->schedule(schedule_selector(GameScene::startGame), 0.02f);
}

void GameScene::onExitTransitionDidStart()
{
	Layer::onExitTransitionDidStart();
	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->stop();
}

void GameScene::menuCloseCallback(Object* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void GameScene::startGame(float dt)
{
	loadingBar->setPercent(loadingBar->getPercent() + 1);
	if (loadingBar->getPercent() == 100)
	{
		this->unscheduleAllSelectors();
		AudioEngine::getInstance()->play();
		this->scheduleUpdate();
	}
	//this->schedule(schedule_selector(GameScene::addRandomNote), 120 / 115.65f);
}

void GameScene::update(float dt)
{
	counter.frame++;
	if (AudioEngine::getInstance()->hasBeat())
		labelInfo->setText("BEAT");
		else
		labelInfo->setText("");
	int percent = AudioEngine::getInstance()->getPosition() * 100 / AudioEngine::getInstance()->getLength();
	loadingBar->setPercent(percent);
	while ((counter.frame + TIME_PRELOAD >= noteline.time&&noteline.type != 0)
		|| (counter.frame + TIME_PRELOAD / 2 >= noteline.time&&noteline.type == 0))//提前一些生成
	{
		if (noteline.time == 0)break;//读到最后跳出
		if (difficulty >= noteline.difficulty)//当前难度符合则生成否则跳过
			addNewNote(noteline.type, noteline.length, noteline.pos, noteline.des);
		getNoteline();//读取下个音符
	}
	if (!AudioEngine::getInstance()->isPlaying())//一首歌结束则切换到结算界面
	{
		this->unscheduleUpdate();
		fin.close();
		if (counter.maxcombo == 0 && counter.miss != counter.total)
			counter.maxcombo = counter.total;//全程无miss
		auto scene = ClearScene::createScene(FileName);
		Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
	}
}

void GameScene::getNoteline()
{
	std::string notestring;
	if (getline(fin, notestring))
	{
		noteline.time = atoi(notestring.substr(0, 5).c_str());
		noteline.difficulty = atoi(notestring.substr(6, 7).c_str());
		noteline.type = atoi(notestring.substr(8, 9).c_str());
		noteline.length = atoi(notestring.substr(10, 13).c_str());
		noteline.pos = atoi(notestring.substr(14, 16).c_str());
		noteline.des = atoi(notestring.substr(17, 19).c_str());
	}
	else
		noteline.time = 0;//结束标识符
}

void GameScene::addNewNote(int type, int length, int pos, int des)
{
	auto note = Note::createNote(type, length, pos, des);
	addChild(note);
	if (note->getType() == 2)
		addArrow(note->getPositionX(), note->getPositionY(), note->getDestX(), note->getDestY());
}

void GameScene::addArrow(int posX, int posY, int desX, int desY)
{
	auto arrow = Sprite::create("gameSceneUI/arrow.png");
	auto dest = Sprite::create("gameSceneUI/note.png");
	arrow->setPosition(posX / 2 + desX / 2, posY / 2 + desY / 2);
	dest->setPosition(desX, desY);
	arrow->setRotation(atan2(desX - posX, desY - posY) * 180 / M_PI);
	dest->runAction(FadeOut::create(2));
	arrow->runAction(FadeOut::create(2));//消失特效
	addChild(arrow);
	addChild(dest);
}

void GameScene::addRandomNote(float dt)
{
	int randomT = CCRANDOM_0_1() * 3;
	int randomX = CCRANDOM_0_1() * 8 + 1;
	int randomY = CCRANDOM_0_1() * 8 + 1;
	int randomA = CCRANDOM_0_1() * 8 + 1;
	int randomB = CCRANDOM_0_1() * 8 + 1;
	addNewNote(randomT, 60, randomX * 10 + randomY, randomA * 10 + randomB);
}

void GameScene::judgeNote(int judge)
{
	counter.total++;
	char temp[64];
	switch (judge)
	{
	case 0:
		if (counter.maxcombo < counter.combo)
			counter.maxcombo = counter.combo;
		counter.combo = 0;
		counter.miss++;
		labelJudge->setText("Miss!");
		labelCombo->setText("");
		break;
	case 1:
		counter.combo++;
		counter.good++;
		labelJudge->setText("Good!");
		sprintf(temp, "%d", counter.combo);
		labelCombo->setText(temp);
		break;
	case 2:
		counter.combo++;
		counter.perfect++;
		labelJudge->setText("Perfect!");
		sprintf(temp, "%d", counter.combo);
		labelCombo->setText(temp);
		break;
	}
	labelJudge->runAction(FadeOut::create(1));
	labelCombo->runAction(FadeOut::create(1));//消失特效
}

void GameScene::touchEvent(Object* obj, gui::TouchEventType eventType)
{
	auto button = dynamic_cast<Button*>(obj);
	int tag = button->getTag();
	switch (eventType)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		if (tag == GAMESCENE_PAUSE)
		{
			Director::getInstance()->pause();
			AudioEngine::getInstance()->pause();
			pauseNode->setVisible(true);
		}
		else if (tag == GAMESCENE_START)
		{
			pauseNode->setVisible(false);
			Director::getInstance()->resume();
			AudioEngine::getInstance()->resume();
		}
		else if (tag == GAMESCENE_RETRY)
		{
			pauseNode->setVisible(false);
			auto scene = GameScene::createScene(FileName);
			Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
		}
		else if (tag == GAMESCENE_SET)
		{
		}
		else if (tag == GAMESCENE_RETURN)
		{
			pauseNode->setVisible(false);
			auto scene = MainScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
		}
		break;
	}
}


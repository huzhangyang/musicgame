#include "GameScene.h"
#include "ClearScene.h"
#include "SelectScene.h"
#include "Note.h"
#include "MapUtils.h"

extern int notenumber;//总note数
extern std::string FileName;//音乐文件名称
extern int TIME_PRELOAD;

Counter counter;
Node *MenuNode, *UINode;
int setting_difficulty;
float setting_lag;
bool setting_scanline;

Scene* GameScene::createScene(std::string filename)
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);

	FileName = filename;
	TIME_PRELOAD = 7200 / BPM;
	setting_difficulty = UserDefault::getInstance()->getIntegerForKey("difficulty");//获取当前难度
	setting_scanline = UserDefault::getInstance()->getBoolForKey("scanline");
	setting_lag = UserDefault::getInstance()->getIntegerForKey("lag") * 60 / 100;
	counter.total = 0;//音符总数
	counter.perfect = 0;//完美数
	counter.good = 0;//普通击中数
	counter.miss = 0;//错过数
	counter.combo = 0;//连击数
	counter.percent = 0;//总分
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
	addChild(sceneNode);
	UINode = sceneNode->getChildByTag(10004);
	MenuNode = sceneNode->getChildByTag(10005);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("gameSceneUI");
	auto PauseComponent = (cocostudio::ComRender*)MenuNode->getComponent("pauseSelectUI");
	auto UILayer = (Layer*)UIComponent->getNode();
	auto MenuLayer = (Layer*)PauseComponent->getNode();
	//////////
	auto buttonPause = dynamic_cast<Button*>(UILayer->getChildByTag(GAMESCENE_BUTTON_PAUSE));
	buttonPause->addTouchEventListener(this, toucheventselector(GameScene::touchEvent));
	buttonPause->setTouchEnabled(false);
	//////////
	auto bgPause = dynamic_cast<ImageView*>(MenuLayer->getChildByTag(GAMESCENE_MENU_BG));
	auto buttonRetry = dynamic_cast<Button*>(MenuLayer->getChildByTag(GAMESCENE_MENU_RETRY));
	auto buttonReturn = dynamic_cast<Button*>(MenuLayer->getChildByTag(GAMESCENE_MENU_RETURN));
	auto buttonResume = dynamic_cast<Button*>(MenuLayer->getChildByTag(GAMESCENE_MENU_RESUME));
	bgPause->addTouchEventListener(this, toucheventselector(GameScene::touchEvent));
	buttonRetry->addTouchEventListener(this, toucheventselector(GameScene::touchEvent));
	buttonReturn->addTouchEventListener(this, toucheventselector(GameScene::touchEvent));
	buttonResume->addTouchEventListener(this, toucheventselector(GameScene::touchEvent));
	bgPause->setEnabled(false);
	buttonRetry->setEnabled(false);
	buttonReturn->setEnabled(false);
	buttonResume->setEnabled(false);
	//////////
	return true;
}

void GameScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("gameSceneUI");
	auto UILayer = (Layer*)UIComponent->getNode();
	auto labelCombo = dynamic_cast<Text*>(UILayer->getChildByTag(GAMESCENE_LABEL_COMBO));
	auto labelInfo = dynamic_cast<Text*>(UILayer->getChildByTag(GAMESCENE_LABEL_INFO));
	auto labelLevel = dynamic_cast<Text*>(UILayer->getChildByTag(GAMESCENE_LABEL_LEVEL));
	auto labelDifficulty = dynamic_cast<Text*>(UILayer->getChildByTag(GAMESCENE_LABEL_DIFFICULTY));
	/////////////////////////////////////////////////////	
	auto title = AudioEngine::getInstance()->getName();
	if (title != "")
		labelInfo->setText(title);//显示ID3 TITLE
	else
		labelInfo->setText(FileName);//没获取到则显示文件名
	MusicInfo info = MapUtils::loadMap(FileName.c_str());
	if (setting_difficulty == 0)
	{
		notenumber = info.NoteNumber_Easy;
		labelLevel->setText(info.Level_Easy);
		labelDifficulty->setText("Easy");
	}
	else if (setting_difficulty == 1)
	{
		notenumber = info.NoteNumber_Hard;
		labelLevel->setText(info.Level_Hard);
		labelDifficulty->setText("Hard");
	}
	labelCombo->setText("READY");
	this->schedule(schedule_selector(GameScene::startGame), 0.02f);
}

void GameScene::onExitTransitionDidStart()
{
	Layer::onExitTransitionDidStart();
	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->stop();
	MapUtils::closeMap();
}

void GameScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void GameScene::startGame(float dt)
{
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("gameSceneUI");
	auto UILayer = (Layer*)UIComponent->getNode();
	auto loadingBar = dynamic_cast<LoadingBar*>(UILayer->getChildByTag(GAMESCENE_LOADINGBAR));
	auto labelCombo = dynamic_cast<Text*>(UILayer->getChildByTag(GAMESCENE_LABEL_COMBO));
	auto buttonPause = dynamic_cast<Button*>(UILayer->getChildByTag(GAMESCENE_BUTTON_PAUSE));
	loadingBar->setPercent(loadingBar->getPercent() + 1);
	if (loadingBar->getPercent() == 50)
		labelCombo->runAction(FadeOut::create(1));
	if (loadingBar->getPercent() == 100)
	{
		this->unscheduleAllSelectors();
		labelCombo->setText("");
		labelCombo->setOpacity(100);
		AudioEngine::getInstance()->play();
		this->scheduleUpdate();
		addScanline();
		buttonPause->setTouchEnabled(true);
	}
}

void GameScene::resumeGame(float dt)
{
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("gameSceneUI");
	auto UILayer = (Layer*)UIComponent->getNode();
	auto loadingBar = dynamic_cast<LoadingBar*>(UILayer->getChildByTag(GAMESCENE_LOADINGBAR));
	auto labelCombo = dynamic_cast<Text*>(UILayer->getChildByTag(GAMESCENE_LABEL_COMBO));
	static int calledtime = 0;
	loadingBar->setPercent(calledtime++);
	if (loadingBar->getPercent() == 1)
	{
		labelCombo->setText("READY");
		labelCombo->setOpacity(255);
	}
	if (loadingBar->getPercent() == 50)
		labelCombo->runAction(FadeOut::create(1));
	if (loadingBar->getPercent() == 100)
	{
		this->unscheduleAllSelectors();
		calledtime = 0;
		labelCombo->setText("");
		labelCombo->setOpacity(100);
		for (auto&i : UINode->getChildren())
		{
			i->resume();
			for (auto&j : i->getChildren())
				j->resume();
		}
		AudioEngine::getInstance()->resume();
		this->scheduleUpdate();
	}
}

void GameScene::update(float dt)
{
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("gameSceneUI");
	auto UILayer = (Layer*)UIComponent->getNode();
	auto loadingBar = dynamic_cast<LoadingBar*>(UILayer->getChildByTag(GAMESCENE_LOADINGBAR));
	auto labelLevel = dynamic_cast<Text*>(UILayer->getChildByTag(GAMESCENE_LABEL_LEVEL));
	auto labelInfo = dynamic_cast<Text*>(UILayer->getChildByTag(GAMESCENE_LABEL_INFO));
	int currPos = AudioEngine::getInstance()->getPosition();
	int percent = currPos * 100 / AudioEngine::getInstance()->getLength();
	loadingBar->setPercent(percent);
	setScanline(currPos);
	while ((currPos + TIME_PRELOAD + setting_lag >= noteline.time))//提前一些生成
	{
		if (noteline.time == 0)break;//读到最后跳出
		int arg1 = noteline.type;
		int arg2 = noteline.length;
		int arg3 = noteline.posX;
		int arg4 = noteline.posY;
		MapUtils::getNoteline();//读取下个音符
		addNewNote(arg1, arg2, arg3, arg4);
	}
	if (!AudioEngine::getInstance()->isPlaying())//一首歌结束则切换到结算界面
	{
		this->unscheduleUpdate();
		auto scene = ClearScene::createScene(labelInfo->getStringValue(), labelLevel->getStringValue());
		Director::getInstance()->replaceScene(TransitionCrossFade::create(2, scene));
	}
}

void GameScene::addNewNote(int type, int length, int posX, int posY)
{
	auto note = Note::createNote(type, length, posX, posY);
	UINode->addChild(note);
}

void GameScene::addScanline()
{
	if (setting_scanline)
	{
		scanline = Sprite::create("judge/scanline.png");
		scanline->setOpacity(200);
		scanline->setPosition(655, 305);
		UINode->addChild(scanline);
	}
}

void GameScene::setScanline(int time)
{
	if (setting_scanline)
	{
		float FramePerBeat = 3600 / BPM;
		int y = time % (int)(FramePerBeat * 4);
		if (y < FramePerBeat)
		{
			y = 305 - 240 * y / FramePerBeat;
		}
		else if (y < FramePerBeat * 2)
		{
			y = 65 + 240 * (y - FramePerBeat) / FramePerBeat;
		}
		else if (y < FramePerBeat * 3)
		{
			y = 305 + 240 * (y - FramePerBeat * 2) / FramePerBeat;
		}
		else if (y < FramePerBeat * 4)
		{
			y = 545 - 240 * (y - FramePerBeat * 3) / FramePerBeat;
		}
		scanline->setPositionY(y);
	}
}

void GameScene::judgeNote(int judgeResult)
{
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("gameSceneUI");
	auto UILayer = (Layer*)UIComponent->getNode();
	auto labelCombo = dynamic_cast<Text*>(UILayer->getChildByTag(GAMESCENE_LABEL_COMBO));
	auto labelScore = dynamic_cast<Text*>(UILayer->getChildByTag(GAMESCENE_LABEL_PERCENT));
	char temp[64];
	switch (judgeResult)
	{
	case 0:
		counter.combo = 0;
		counter.miss++;
		labelCombo->setText("");
		break;
	case 1:
		counter.combo++;
		counter.good++;
		counter.percent += 20 * counter.combo / counter.total / (float)notenumber;
		counter.percent += 40 / (float)notenumber;
		sprintf(temp, "%d", counter.combo);
		labelCombo->setText(temp);
		break;
	case 2:
		counter.combo++;
		counter.perfect++;
		counter.percent += 20 * counter.combo / counter.total / (float)notenumber;
		counter.percent += 80 / (float)notenumber;
		sprintf(temp, "%d", counter.combo);
		labelCombo->setText(temp);
		break;
	}
	if (counter.combo == (int)(notenumber *0.5))
		labelCombo->setText("Sensational!!!!!");
	else if (counter.combo == (int)(notenumber *0.4))
		labelCombo->setText("Awesome!!!!");
	else if (counter.combo == (int)(notenumber *0.3))
		labelCombo->setText("Beautiful!!!");
	else if (counter.combo == (int)(notenumber *0.2))
		labelCombo->setText("Charming!!");
	else if (counter.combo == (int)(notenumber *0.1))
		labelCombo->setText("Decent!");
	sprintf(temp, "%.2f", counter.percent);
	labelScore->setText(strcat(temp, "%"));
	labelCombo->runAction(Sequence::create(ScaleTo::create(0.2f, 1.25), ScaleTo::create(0.2f, 1), NULL));//Combo特效
}

void GameScene::touchEvent(Ref* obj, TouchEventType eventType)
{
	auto widget = dynamic_cast<Widget*>(obj);
	int tag = widget->getTag();
	auto MenuComponent = (cocostudio::ComRender*)MenuNode->getComponent("pauseSelectUI");
	auto MenuLayer = (Layer*)MenuComponent->getNode();
	auto bgPause = dynamic_cast<ImageView*>(MenuLayer->getChildByTag(GAMESCENE_MENU_BG));
	auto buttonRetry = dynamic_cast<Button*>(MenuLayer->getChildByTag(GAMESCENE_MENU_RETRY));
	auto buttonReturn = dynamic_cast<Button*>(MenuLayer->getChildByTag(GAMESCENE_MENU_RETURN));
	auto buttonResume = dynamic_cast<Button*>(MenuLayer->getChildByTag(GAMESCENE_MENU_RESUME));
	Scene* scene;
	if (eventType == TouchEventType::TOUCH_EVENT_ENDED)
	{
		switch (tag)
		{
		case GAMESCENE_BUTTON_PAUSE:
			for (auto&i : UINode->getChildren())
			{
				i->pause();
				for (auto&j : i->getChildren())
					j->pause();
			}
			AudioEngine::getInstance()->pause();
			this->unscheduleUpdate();
			MenuNode->setVisible(true);
			bgPause->setEnabled(true);
			buttonRetry->setEnabled(true);
			buttonReturn->setEnabled(true);
			buttonResume->setEnabled(true);
			break;
		case GAMESCENE_MENU_RESUME:
			MenuNode->setVisible(false);
			bgPause->setEnabled(false);
			buttonRetry->setEnabled(false);
			buttonReturn->setEnabled(false);
			buttonResume->setEnabled(false);
			this->schedule(schedule_selector(GameScene::resumeGame), 0.01f);
			break;
		case GAMESCENE_MENU_RETRY:
			MenuNode->setVisible(false);
			bgPause->setEnabled(false);
			buttonRetry->setEnabled(false);
			buttonReturn->setEnabled(false);
			buttonResume->setEnabled(false);
			AudioEngine::getInstance()->stop();
			scene = GameScene::createScene(FileName);
			Director::getInstance()->replaceScene(TransitionPageTurn::create(2, scene, true));
			break;
		case GAMESCENE_MENU_RETURN:
			MenuNode->setVisible(false);
			bgPause->setEnabled(false);
			buttonRetry->setEnabled(false);
			buttonReturn->setEnabled(false);
			buttonResume->setEnabled(false);
			AudioEngine::getInstance()->stop();
			scene = SelectScene::createScene(1);
			Director::getInstance()->replaceScene(TransitionPageTurn::create(2, scene, false));
			break;
		}
	}
}


#include "GameScene.h"
#include "ClearScene.h"
#include "SelectScene.h"
#include "Note.h"
#include "MapUtils.h"

extern int notenumber;//总note数
extern std::string FileName;//音乐文件名称
extern int preloadTime;//预加载时间

Counter counter;
Node *MenuNode, *UINode;
int setting_difficulty;
int setting_lag;

Scene* GameScene::createScene(std::string filename)
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	FileName = filename;
	setting_difficulty = UserDefault::getInstance()->getIntegerForKey("difficulty");//获取当前难度
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
	auto loadingBar = dynamic_cast<LoadingBar*>(UILayer->getChildByTag(GAMESCENE_LOADINGBAR));
	auto labelCombo = dynamic_cast<Text*>(UILayer->getChildByTag(GAMESCENE_LABEL_COMBO));
	auto labelInfo = dynamic_cast<Text*>(UILayer->getChildByTag(GAMESCENE_LABEL_INFO));
	auto labelLevel = dynamic_cast<Text*>(UILayer->getChildByTag(GAMESCENE_LABEL_LEVEL));
	auto labelDifficulty = dynamic_cast<Text*>(UILayer->getChildByTag(GAMESCENE_LABEL_DIFFICULTY));
	auto labelScore = dynamic_cast<Text*>(UILayer->getChildByTag(GAMESCENE_LABEL_PERCENT));
	/////////////////////////////////////////////////////	
	std::string musicname = FileUtils::getInstance()->fullPathForFilename("music/" + FileName + ".mp3");
	AudioEngine::getInstance()->create(musicname.c_str());
	auto title = AudioEngine::getInstance()->getName();
	if (title != "")
		labelInfo->setString(title);//显示ID3 TITLE
	else
		labelInfo->setString(FileName);//没获取到则显示文件名
	labelInfo->setColor(Color3B(64, 68, 72));
	MusicInfo info = MapUtils::loadMap(FileName.c_str());
	if (setting_difficulty == 0)
	{
		notenumber = info.NoteNumber_Easy;
		labelLevel->setString(info.Level_Easy);
		labelDifficulty->setString("Easy");
		labelLevel->setColor(Color3B(45, 65, 30));
		labelDifficulty->setColor(Color3B(45, 65, 30));
		labelScore->setColor(Color3B(45, 65, 30));
		preloadTime = 60;
	}
	else if (setting_difficulty == 1)
	{
		notenumber = info.NoteNumber_Hard;
		labelLevel->setString(info.Level_Hard);
		labelDifficulty->setString("Hard");
		labelLevel->setColor(Color3B(150, 15, 15));
		labelDifficulty->setColor(Color3B(150, 15, 15));
		labelScore->setColor(Color3B(150, 15, 15));
		preloadTime = 40;
	}
	labelCombo->setString("READY");
	loadingBar->setPercent(0);
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
		labelCombo->setString("");
		labelCombo->setOpacity(100);
		AudioEngine::getInstance()->play();
		this->scheduleUpdate();
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
		labelCombo->setString("READY");
		labelCombo->setOpacity(255);
	}
	if (loadingBar->getPercent() == 50)
		labelCombo->runAction(FadeOut::create(1));
	if (loadingBar->getPercent() == 100)
	{
		this->unscheduleAllSelectors();
		calledtime = 0;
		labelCombo->setString("");
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
	while ((currPos + preloadTime + setting_lag >= noteline.time))//提前一些生成
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
		auto scene = ClearScene::createScene(labelInfo->getString(), labelLevel->getString());
		Director::getInstance()->replaceScene(TransitionCrossFade::create(2, scene));
	}
}

void GameScene::addNewNote(int type, int length, int posX, int posY)
{
	auto note = Note::createNote(type, length, posX, posY);
	UINode->addChild(note);
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
		labelCombo->setString("");
		break;
	case 1:
		counter.combo++;
		counter.good++;
		counter.percent += 20 * counter.combo / counter.total / (float)notenumber;
		counter.percent += 40 / (float)notenumber;
		sprintf(temp, "%d", counter.combo);
		labelCombo->setString(temp);
		break;
	case 2:
		counter.combo++;
		counter.perfect++;
		counter.percent += 20 * counter.combo / counter.total / (float)notenumber;
		counter.percent += 80 / (float)notenumber;
		sprintf(temp, "%d", counter.combo);
		labelCombo->setString(temp);
		break;
	}
	if (counter.combo == (int)(notenumber *0.5))
		labelCombo->setString("Sensational!!!!!");
	else if (counter.combo == (int)(notenumber *0.4))
		labelCombo->setString("Awesome!!!!");
	else if (counter.combo == (int)(notenumber *0.3))
		labelCombo->setString("Beautiful!!!");
	else if (counter.combo == (int)(notenumber *0.2))
		labelCombo->setString("Charming!!");
	else if (counter.combo == (int)(notenumber *0.1))
		labelCombo->setString("Decent!");
	sprintf(temp, "%.2f", counter.percent);
	labelScore->setString(strcat(temp, "%"));
	labelCombo->runAction(Sequence::create(ScaleTo::create(0.1f, 1.25), ScaleTo::create(0.1f, 1), NULL));//Combo特效
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


#include "GameScene.h"
#include "ClearScene.h"
#include "MainScene.h"
#include "Note.h"
#include "MapUtils.h"

extern int notenumber;//总note数
int difficulty;//当前难度
std::string FileName;//音乐文件名称

Counter counter;
Text *labelCombo, *labelScore;

Scene* GameScene::createScene(std::string filename)
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);

	FileName = filename;
	counter.total = 0;
	counter.perfect = 0;
	counter.good = 0;
	counter.miss = 0;
	counter.combo = 0;
	counter.percent = 0;
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
	PauseNode = sceneNode->getChildByTag(10005);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("gameSceneUI");
	auto PauseComponent = (cocostudio::ComRender*) PauseNode->getComponent("pauseSelectUI");
	auto UIlayer = UIComponent->getNode();
	auto Pauselayer = PauseComponent->getNode();
	auto buttonPause = dynamic_cast<Button*>(UIlayer->getChildByTag(GAMESCENE_PAUSE));
	bgPause = dynamic_cast<ImageView*>(Pauselayer->getChildByTag(GAMESCENE_PAUSEBG));
	buttonRetry = dynamic_cast<Button*>(Pauselayer->getChildByTag(GAMESCENE_RETRY));
	buttonReturn = dynamic_cast<Button*>(Pauselayer->getChildByTag(GAMESCENE_RETURN));
	buttonOption = dynamic_cast<Button*>(Pauselayer->getChildByTag(GAMESCENE_OPTION));
	buttonResume = dynamic_cast<Button*>(Pauselayer->getChildByTag(GAMESCENE_RESUME));
	labelInfo = dynamic_cast<Text*>(UIlayer->getChildByTag(GAMESCENE_INFO));
	labelCombo = dynamic_cast<Text*>(UIlayer->getChildByTag(GAMESCENE_COMBO));
	labelScore = dynamic_cast<Text*>(UIlayer->getChildByTag(GAMESCENE_PERCENT));
	labelDifficulty = dynamic_cast<ImageView*>(UIlayer->getChildByTag(GAMESCENE_DIFFICULTY));
	loadingBar = dynamic_cast<LoadingBar*>(UIlayer->getChildByTag(GAMESCENE_LOADINGBAR));
	bgPause->addTouchEventListener(this, toucheventselector(GameScene::touchEvent));
	buttonPause->addTouchEventListener(this, toucheventselector(GameScene::touchEvent));
	buttonRetry->addTouchEventListener(this, toucheventselector(GameScene::touchEvent));
	buttonReturn->addTouchEventListener(this, toucheventselector(GameScene::touchEvent));
	buttonOption->addTouchEventListener(this, toucheventselector(GameScene::touchEvent));
	buttonResume->addTouchEventListener(this, toucheventselector(GameScene::touchEvent));
	bgPause->setEnabled(false);
	buttonRetry->setEnabled(false);
	buttonReturn->setEnabled(false);
	buttonOption->setEnabled(false);
	buttonResume->setEnabled(false);
	labelScore->setFontName("trends.ttf");
	difficulty = UserDefault::getInstance()->getIntegerForKey("difficulty");//获取当前难度
	if (difficulty == 0)
	{
		labelDifficulty->loadTexture("gameSceneUI/easy.png");
	}
	else if (difficulty == 1)
	{
		labelDifficulty->loadTexture("gameSceneUI/hard.png");
	}
	return true;
}

void GameScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	/////////////////////////////////////////////////////	
	std::string musicname = "music/" + FileName + ".mp3";
	AudioEngine::getInstance()->create(musicname.c_str());
	auto title = AudioEngine::getInstance()->getName();
	if (title != "")
		labelInfo->setText(title);//显示ID3 TITLE
	else
		labelInfo->setText(FileName);//没获取到则显示文件名
	MapUtils::loadMap(FileName.c_str());
	notenumber = MapUtils::getNoteNumber();
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
	loadingBar->setPercent(loadingBar->getPercent() + 1);
	if (loadingBar->getPercent() == 50)
		labelCombo->runAction(FadeOut::create(1));
	if (loadingBar->getPercent() == 100)
	{
		this->unscheduleAllSelectors();
		labelCombo->setText("");
		labelCombo->setOpacity(100);
		AudioEngine::getInstance()->play();
		auto x = AudioEngine::getInstance()->isPlaying();
		this->scheduleUpdate();
		addScanline();
	}
}

void GameScene::update(float dt)
{
	int currPos = AudioEngine::getInstance()->getPosition();
	int percent = currPos * 100 / AudioEngine::getInstance()->getLength();
	loadingBar->setPercent(percent);
	while ((currPos + 7200 / BPM >= noteline.time))//提前一些生成
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
		auto scene = ClearScene::createScene(FileName);
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
	auto bar = Sprite::create("game/bar.png");
	bar->setOpacity(200);
	bar->setPosition(655, 305);
	UINode->addChild(bar);
	auto action1 = MoveTo::create(60.0 / BPM, Point(655, 65));
	auto action2 = MoveTo::create(60.0 / BPM, Point(655, 305));
	auto action3 = MoveTo::create(60.0 / BPM, Point(655, 545));
	auto action4 = MoveTo::create(60.0 / BPM, Point(655, 305));
	bar->runAction(RepeatForever::create(Sequence::create(action1, action2, action3, action4, NULL)));
}

void GameScene::judgeNote(int judgeResult)
{
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
	Scene* scene;
	if (eventType == TouchEventType::TOUCH_EVENT_ENDED)
	{
		switch (tag)
		{
		case  GAMESCENE_PAUSE:
			Director::getInstance()->pause();
			AudioEngine::getInstance()->pause();
			PauseNode->setVisible(true);
			bgPause->setEnabled(true);
			buttonRetry->setEnabled(true);
			buttonReturn->setEnabled(true);
			buttonOption->setEnabled(true);
			buttonResume->setEnabled(true);
			break;
		case GAMESCENE_RESUME:
			PauseNode->setVisible(false);
			bgPause->setEnabled(false);
			buttonRetry->setEnabled(false);
			buttonReturn->setEnabled(false);
			buttonOption->setEnabled(false);
			buttonResume->setEnabled(false);
			Director::getInstance()->resume();
			AudioEngine::getInstance()->resume();
			break;
		case GAMESCENE_RETRY:
			PauseNode->setVisible(false);
			bgPause->setEnabled(false);
			buttonRetry->setEnabled(false);
			buttonReturn->setEnabled(false);
			buttonOption->setEnabled(false);
			buttonResume->setEnabled(false);
			Director::getInstance()->resume();
			AudioEngine::getInstance()->stop();
			this->unscheduleUpdate();
			scene = GameScene::createScene(FileName);
			Director::getInstance()->replaceScene(TransitionPageTurn::create(2, scene, true));
			break;
		case GAMESCENE_OPTION:
			if (UserDefault::getInstance()->getIntegerForKey("difficulty") == 0)
				UserDefault::getInstance()->setIntegerForKey("difficulty", 1);
			else
				UserDefault::getInstance()->setIntegerForKey("difficulty", 0);
			break;
		case GAMESCENE_RETURN:
			PauseNode->setVisible(false);
			bgPause->setEnabled(false);
			buttonRetry->setEnabled(false);
			buttonReturn->setEnabled(false);
			buttonOption->setEnabled(false);
			buttonResume->setEnabled(false);
			Director::getInstance()->resume();
			AudioEngine::getInstance()->stop();
			this->unscheduleUpdate();
			scene = MainScene::createScene();
			Director::getInstance()->replaceScene(TransitionPageTurn::create(2, scene, false));
			break;
		}
	}
}


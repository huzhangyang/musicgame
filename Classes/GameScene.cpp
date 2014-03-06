#include "GameScene.h"
#include "ClearScene.h"
#include "MainScene.h"
#include "Note.h"
#include "MapUtils.h"

int difficulty;//当前难度
std::string FileName;//音乐文件名称

Counter counter;
TextBMFont *labelCombo;

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
	labelCombo = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(GAMESCENE_COMBO));
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
	labelInfo->setText(FileName);//显示文件名
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
	MapUtils::loadMap(FileName.c_str());
	this->schedule(schedule_selector(GameScene::startGame), 0.02f);
}

void GameScene::onExitTransitionDidStart()
{
	Layer::onExitTransitionDidStart();
	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->stop();
	MapUtils::closeMap();
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
		auto x = AudioEngine::getInstance()->isPlaying();
		this->scheduleUpdate();
	}
}

void GameScene::update(float dt)
{
	int currPos = AudioEngine::getInstance()->getPosition();
	int percent = currPos * 100 / AudioEngine::getInstance()->getLength();
	loadingBar->setPercent(percent);
	while ((currPos + TIME_PRELOAD >= noteline.time))//提前一些生成
	{
		if (noteline.time == 0)break;//读到最后跳出
		if (difficulty >= noteline.difficulty)//当前难度符合则生成否则跳过
		{
			int arg1 = noteline.type;
			int arg2 = noteline.length;
			int arg3 = noteline.pos;
			MapUtils::getNoteline();//读取下个音符
			addNewNote(arg1, arg2, arg3);
		}
	}
	if (!AudioEngine::getInstance()->isPlaying())//一首歌结束则切换到结算界面
	{
		this->unscheduleUpdate();
		auto scene = ClearScene::createScene(FileName);
		Director::getInstance()->replaceScene(TransitionCrossFade::create(2, scene));
	}
}

void GameScene::addNewNote(int type, int length, int pos)
{
	auto note = Note::createNote(type, length, pos);
	UINode->addChild(note);
}

void GameScene::judgeNote(int judgeResult)
{
	char temp[64];
	float all = MapUtils::getLineNumber();
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
		counter.percent += 20 * counter.combo / (float)counter.total / all;
		counter.percent += 40 / all;
		sprintf(temp, "%d", counter.combo);
		labelCombo->setText(temp);
		break;
	case 2:
		counter.combo++;
		counter.perfect++;
		counter.percent += 20 * counter.combo / (float)counter.total / all;
		counter.percent += 80 / all;
		sprintf(temp, "%d", counter.combo);
		labelCombo->setText(temp);
		break;
	}
	labelCombo->runAction(FadeOut::create(1));//消失特效
}

void GameScene::touchEvent(Object* obj, gui::TouchEventType eventType)
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


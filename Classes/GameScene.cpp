#include "GameScene.h"
#include "ClearScene.h"
#include "Note.h"
#include "MainScene.h"
#include <fstream>

int difficulty;//当前难度
std::string FileName;//音乐文件名称
std::ifstream fin;//输入流
Noteline noteline;
Counter counter;
Text *labelCombo;
Sprite *judgePic;

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
	judgePic = Sprite::create();
	UINode->addChild(judgePic);
	judgePic->setOpacity(0);
	return true;
}

void GameScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	/////////////////////////////////////////////////////
	std::string musicname = "music/" + FileName + ".mp3";
	AudioEngine::getInstance()->create(musicname.c_str());
	//fin.open(FileUtils::getInstance()->fullPathForFilename(FileName + ".gnm"));//打开手动生成测试谱面
	std::string mapname = FileUtils::getInstance()->getWritablePath() + FileName + ".gnm";
	fin.open(mapname);//打开自动生成测试谱面
	getNoteline();//读取第一行

	loadingBar->setPercent(0);
	this->schedule(schedule_selector(GameScene::startGame), 0.02f);
}

void GameScene::onExitTransitionDidStart()
{
	Layer::onExitTransitionDidStart();
	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->stop();
	fin.close();
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
	int currPos = AudioEngine::getInstance()->getPosition();
	int percent = currPos * 100 / AudioEngine::getInstance()->getLength();
	loadingBar->setPercent(percent);
	while ((currPos + TIME_PRELOAD >= noteline.time&&noteline.type != 0)
		|| (currPos >= noteline.time&&noteline.type == 0))//提前一些生成
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
		auto scene = ClearScene::createScene(FileName);
		Director::getInstance()->replaceScene(TransitionCrossFade::create(2, scene));
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
	UINode->addChild(note);
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
	dest->runAction(FadeOut::create(2));//消失特效
	arrow->runAction(FadeOut::create(2));//消失特效
	UINode->addChild(arrow);
	UINode->addChild(dest);
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

void GameScene::judgeNote(int judge, Point pos)
{
	counter.total++;
	char temp[64];
	switch (judge)
	{
	case 0:
		counter.combo = 0;
		counter.miss++;
		judgePic->setTexture("clearSceneUI/miss.png");
		labelCombo->setText("");
		break;
	case 1:
		counter.combo++;
		counter.good++;
		counter.percent += (float)counter.combo / (float)counter.total;
		judgePic->setTexture("clearSceneUI/good.png");
		sprintf(temp, "%d", counter.combo);
		labelCombo->setText(temp);
		break;
	case 2:
		counter.combo++;
		counter.perfect++;
		counter.percent += (float)counter.combo / (float)counter.total;
		judgePic->setTexture("clearSceneUI/perfect.png");
		sprintf(temp, "%d", counter.combo);
		labelCombo->setText(temp);
		break;
	}
	labelCombo->runAction(FadeOut::create(1));//消失特效
	judgePic->setPosition(pos);
	judgePic->setScale(0.6f);
	judgePic->runAction(FadeOut::create(0.2f));
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


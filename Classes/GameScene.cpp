#include "GameScene.h"
#include "ClearScene.h"
#include "Note.h"
#include "MainScene.h"
#include <fstream>

int difficulty;//��ǰ�Ѷ�
std::string FileName;//�����ļ�����
std::ifstream fin;//������
Noteline noteline;
Counter counter;
Text *labelCombo;

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
	std::string musicname = "music/" + FileName + ".mp3";
	AudioEngine::getInstance()->create(musicname.c_str());

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
	labelInfo->setText(FileName);//��ʾ�ļ���
	difficulty = UserDefault::getInstance()->getIntegerForKey("difficulty");//��ȡ��ǰ�Ѷ�
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
	//fin.open(FileUtils::getInstance()->fullPathForFilename(FileName + ".gnm"));//���ֶ����ɲ�������
	std::string mapname = FileUtils::getInstance()->getWritablePath() + FileName + ".gnm";
	fin.open(mapname);//���Զ����ɲ�������
	getNoteline();//��ȡ��һ��
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
}

void GameScene::update(float dt)
{
	int currPos = AudioEngine::getInstance()->getPosition();
	int percent = currPos * 100 / AudioEngine::getInstance()->getLength();
	loadingBar->setPercent(percent);
	while ((currPos + TIME_PRELOAD >= noteline.time))//��ǰһЩ����
	{
		if (noteline.time == 0)break;//�����������
		if (difficulty >= noteline.difficulty)//��ǰ�Ѷȷ��������ɷ�������
			addNewNote(noteline.type, noteline.length, noteline.pos, noteline.des);
		getNoteline();//��ȡ�¸�����
	}
	if (!AudioEngine::getInstance()->isPlaying())//һ�׸�������л����������
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
		noteline.time = 0;//������ʶ��
}

void GameScene::addNewNote(int type, int length, int pos, int des)
{
	auto note = Note::createNote(type, length, pos, des);
	UINode->addChild(note);
}

void GameScene::judgeNote(int judgeResult)
{
	counter.total++;
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
		counter.percent += (float)counter.combo / (float)counter.total;
		sprintf(temp, "%d", counter.combo);
		labelCombo->setText(temp);
		break;
	case 2:
		counter.combo++;
		counter.perfect++;
		counter.percent += (float)counter.combo / (float)counter.total;
		sprintf(temp, "%d", counter.combo);
		labelCombo->setText(temp);
		break;
	}
	labelCombo->runAction(FadeOut::create(1));//��ʧ��Ч
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


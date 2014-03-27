#include "ClearScene.h"
#include "MainScene.h"
#include "GameScene.h"

std::string FileName;//音乐文件名称
std::string Level;

Scene* ClearScene::createScene(std::string filename, std::string level)
{
	auto scene = Scene::create();
	auto layer = ClearScene::create();
	FileName = filename;
	Level = level;
	scene->addChild(layer);
	return scene;
}

bool ClearScene::init()
{
	//super init
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////////////////////////////
	auto sceneNode = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("clearScene.json");
	addChild(sceneNode);
	UINode = sceneNode->getChildByTag(10003);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("GUIComponent");
	auto UIlayer = UIComponent->getNode();
	auto background = dynamic_cast<ImageView*>(UIlayer->getChildByTag(CLEARSCENE_BG));
	auto buttonRetry = dynamic_cast<Button*>(UIlayer->getChildByTag(CLEARSCENE_RETRY));
	auto buttonReturn = dynamic_cast<Button*>(UIlayer->getChildByTag(CLEARSCENE_RETURN));
	auto labelPerfect = dynamic_cast<Text*>(UIlayer->getChildByTag(CLEARSCENE_PNO));
	auto labelGood = dynamic_cast<Text*>(UIlayer->getChildByTag(CLEARSCENE_GNO));
	auto labelMiss = dynamic_cast<Text*>(UIlayer->getChildByTag(CLEARSCENE_MNO));
	auto labelTotal = dynamic_cast<Text*>(UIlayer->getChildByTag(CLEARSCENE_TNO));
	auto labelComplete = dynamic_cast<Text*>(UIlayer->getChildByTag(CLEARSCENE_CNO));
	auto labelJudge = dynamic_cast<ImageView*>(UIlayer->getChildByTag(CLEARSCENE_JUDGE));
	auto labelDifficulty = dynamic_cast<Text*>(UIlayer->getChildByTag(CLEARSCENE_DIFFICULTY));
	buttonRetry->addTouchEventListener(this, toucheventselector(ClearScene::touchEvent));
	buttonReturn->addTouchEventListener(this, toucheventselector(ClearScene::touchEvent));
	auto difficulty = UserDefault::getInstance()->getIntegerForKey("difficulty");//获取当前难度
	if (difficulty == 0)
	{
		labelDifficulty->setText("Easy");
	}
	else if (difficulty == 1)
	{
		labelDifficulty->setText("Hard");
	}
	AudioEngine::getInstance()->createLoop("music/clear.mp3");
	char temp[64];
	sprintf(temp, "%d", counter.perfect);
	labelPerfect->setText(temp);
	sprintf(temp, "%d", counter.good);
	labelGood->setText(temp);
	sprintf(temp, "%d", counter.miss);
	labelMiss->setText(temp);
	sprintf(temp, "%d", counter.total);
	labelTotal->setText(temp);
	sprintf(temp, "%.2f", counter.percent);
	labelComplete->setText(strcat(temp, "%"));
	if (counter.combo = counter.total&&counter.percent > 95)
		labelJudge->loadTexture("clearSceneUI/S.png");
	else if (counter.percent >= 90)
		labelJudge->loadTexture("clearSceneUI/A.png");
	else if (counter.percent >= 80)
		labelJudge->loadTexture("clearSceneUI/B.png");
	else if (counter.percent >= 70)
		labelJudge->loadTexture("clearSceneUI/C.png");
	else if (counter.percent >= 60)
		labelJudge->loadTexture("clearSceneUI/D.png");
	else
	{
		background->loadTexture("clearSceneUI/QAQ.png");
		labelJudge->loadTexture("clearSceneUI/F.png");
	}
	auto emitter = ParticleRain::createWithTotalParticles(1000);
	emitter->setGravity(Point(0, -20));
	emitter->setAutoRemoveOnFinish(true);
	addChild(emitter, 10);
	return true;
}

void ClearScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->play();
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("GUIComponent");
	auto UIlayer = UIComponent->getNode();
	auto labelInfo = dynamic_cast<Text*>(UIlayer->getChildByTag(CLEARSCENE_INFO));
	auto labelLevel = dynamic_cast<Text*>(UIlayer->getChildByTag(CLEARSCENE_LEVEL));
	labelLevel->setText("LV."+Level);
	labelInfo->setText(FileName);//没获取到则显示文件名
}

void ClearScene::onExitTransitionDidStart()
{
	Layer::onExitTransitionDidStart();
	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->stop();
}

void ClearScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void ClearScene::touchEvent(Ref* obj, TouchEventType eventType)
{
	auto button = dynamic_cast<Button*>(obj);
	int tag = button->getTag();
	switch (eventType)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		if (tag == CLEARSCENE_RETRY)
		{
			auto scene = GameScene::createScene(FileName);
			Director::getInstance()->replaceScene(TransitionPageTurn::create(2, scene, true));
		}
		else if (tag == CLEARSCENE_RETURN)
		{
			auto scene = MainScene::createScene();
			Director::getInstance()->replaceScene(TransitionPageTurn::create(2, scene, false));
		}
		break;
	}
}


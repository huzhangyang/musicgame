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
	buttonRetry->addTouchEventListener(this, toucheventselector(ClearScene::touchEvent));
	buttonReturn->addTouchEventListener(this, toucheventselector(ClearScene::touchEvent));
	AudioEngine::getInstance()->createLoop("bgm/clear.ogg");
	char temp[64];
	sprintf(temp, "%d", counter.perfect);
	labelPerfect->setString(temp);
	sprintf(temp, "%d", counter.good);
	labelGood->setString(temp);
	sprintf(temp, "%d", counter.miss);
	labelMiss->setString(temp);
	sprintf(temp, "%d", counter.total);
	labelTotal->setString(temp);
	sprintf(temp, "%.2f", counter.percent);
	labelComplete->setString(strcat(temp, "%"));
	if (counter.combo == counter.total&&counter.percent > 95)
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
	labelPerfect->setOpacity(0);
	labelGood->setOpacity(0);
	labelMiss->setOpacity(0);
	labelTotal->setOpacity(0);
	labelComplete->setOpacity(0);
	labelJudge->setOpacity(0);
	labelJudge->setScale(3);
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
	auto labelDifficulty = dynamic_cast<Text*>(UIlayer->getChildByTag(CLEARSCENE_DIFFICULTY));
	auto labelPerfect = dynamic_cast<Text*>(UIlayer->getChildByTag(CLEARSCENE_PNO));
	auto labelGood = dynamic_cast<Text*>(UIlayer->getChildByTag(CLEARSCENE_GNO));
	auto labelMiss = dynamic_cast<Text*>(UIlayer->getChildByTag(CLEARSCENE_MNO));
	auto labelTotal = dynamic_cast<Text*>(UIlayer->getChildByTag(CLEARSCENE_TNO));
	auto labelComplete = dynamic_cast<Text*>(UIlayer->getChildByTag(CLEARSCENE_CNO));
	auto labelJudge = dynamic_cast<ImageView*>(UIlayer->getChildByTag(CLEARSCENE_JUDGE));
	auto difficulty = UserDefault::getInstance()->getIntegerForKey("difficulty");//获取当前难度
	if (difficulty == 0)
	{
		labelDifficulty->setString("Easy");
		labelDifficulty->setColor(Color3B(45, 65, 30));
		labelLevel->setColor(Color3B(45, 65, 30));
	}
	else if (difficulty == 1)
	{
		labelDifficulty->setString("Hard");
		labelDifficulty->setColor(Color3B(150, 15, 15));
		labelLevel->setColor(Color3B(150, 15, 15));
	}
	labelLevel->setString("LV." + Level);
	labelInfo->setString(FileName);//没获取到则显示文件名
	//////////
	labelTotal->runAction(FadeIn::create(1));
	labelPerfect->runAction(Sequence::create(DelayTime::create(1), FadeIn::create(1), NULL));
	labelGood->runAction(Sequence::create(DelayTime::create(2), FadeIn::create(1), NULL));
	labelMiss->runAction(Sequence::create(DelayTime::create(3), FadeIn::create(1), NULL));
	labelComplete->runAction(Sequence::create(DelayTime::create(4), FadeIn::create(1), NULL));
	labelJudge->runAction(Sequence::create(DelayTime::create(4.3f), FadeIn::create(0.2f), EaseElasticOut::create(ScaleTo::create(1.5f, 1.5f)), NULL));
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


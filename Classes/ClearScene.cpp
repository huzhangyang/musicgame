#include "ClearScene.h"
#include "MainScene.h"
#include "GameScene.h"

extern std::string FileName;//音乐文件名称

Scene* ClearScene::createScene(std::string filename)
{
	auto scene = Scene::create();
	auto layer = ClearScene::create();
	FileName = filename;
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
	AudioEngine::getInstance()->createLoop("music/clear.mp3");
	auto sceneNode = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("clearScene.json");
	addChild(sceneNode);
	auto UINode = sceneNode->getChildByTag(10003);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("GUIComponent");
	auto UIlayer = UIComponent->getNode();
	auto background = dynamic_cast<ImageView*>(UIlayer->getChildByTag(CLEARSCENE_BG));
	auto buttonRetry = dynamic_cast<Button*>(UIlayer->getChildByTag(CLEARSCENE_RETRY));
	auto buttonReturn = dynamic_cast<Button*>(UIlayer->getChildByTag(CLEARSCENE_RETURN));
	auto labelPerfect = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(CLEARSCENE_PNO));
	auto labelGood = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(CLEARSCENE_GNO));
	auto labelMiss = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(CLEARSCENE_MNO));
	auto labelTotal = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(CLEARSCENE_TNO));
	auto labelComplete = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(CLEARSCENE_CNO));
	auto labelJudge = dynamic_cast<ImageView*>(UIlayer->getChildByTag(CLEARSCENE_JUDGE));
	auto labelInfo = dynamic_cast<Text*>(UIlayer->getChildByTag(CLEARSCENE_INFO));
	auto labelDifficulty = dynamic_cast<ImageView*>(UIlayer->getChildByTag(CLEARSCENE_DIFFICULTY));
	labelInfo->setText(FileName);
	auto difficulty = UserDefault::getInstance()->getIntegerForKey("difficulty");//获取当前难度
	if (difficulty == 0)
	{
		labelDifficulty->loadTexture("gameSceneUI/easy.png");
	}
	else if (difficulty == 1)
	{
		labelDifficulty->loadTexture("gameSceneUI/hard.png");
	}
	buttonRetry->addTouchEventListener(this, toucheventselector(ClearScene::touchEvent));
	buttonReturn->addTouchEventListener(this, toucheventselector(ClearScene::touchEvent));
	char temp[64];
	sprintf(temp, "%d", counter.perfect);
	labelPerfect->setText(temp);
	sprintf(temp, "%d", counter.good);
	labelGood->setText(temp);
	sprintf(temp, "%d", counter.miss);
	labelMiss->setText(temp);
	sprintf(temp, "%d", counter.total);
	labelTotal->setText(temp);
	float completePercent = counter.percent*0.2 / (float)counter.total;
	completePercent += (float)counter.perfect / (float)counter.total * 0.8;
	completePercent += (float)counter.good / (float)counter.total * 0.4;
	sprintf(temp, "%.2f", completePercent * 100);
	labelComplete->setText(strcat(temp, "%"));
	if (completePercent >= 0.99)
		labelJudge->loadTexture("clearSceneUI/S.png");
	else if (completePercent >= 0.9)
		labelJudge->loadTexture("clearSceneUI/A.png");
	else if (completePercent >= 0.8)
		labelJudge->loadTexture("clearSceneUI/B.png");
	else if (completePercent >= 0.7)
		labelJudge->loadTexture("clearSceneUI/C.png");
	else if (completePercent >= 0.6)
		labelJudge->loadTexture("clearSceneUI/D.png");
	else
	{
		background->loadTexture("clearSceneUI/QAQ.png");
		labelJudge->loadTexture("clearSceneUI/F.png");
	}
	return true;
}

void ClearScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->play();
}

void ClearScene::onExitTransitionDidStart()
{
	Layer::onExitTransitionDidStart();
	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->stop();
}

void ClearScene::menuCloseCallback(Object* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void ClearScene::touchEvent(Object* obj, gui::TouchEventType eventType)
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


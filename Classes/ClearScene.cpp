#include "Global.h"
#include "ClearScene.h"
#include "MainScene.h"
#include "GameScene.h"

Scene* ClearScene::createScene()
{
	auto scene = Scene::create();
	auto layer = ClearScene::create();
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
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/clear.mp3");
	auto sceneNode = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("clearScene.json");
	addChild(sceneNode);
	auto UINode = sceneNode->getChildByTag(10003);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("GUIComponent");
	auto UIlayer = UIComponent->getNode();
	auto buttonRetry = dynamic_cast<Button*>(UIlayer->getChildByTag(CLEARSCENE_RETRY));
	buttonRetry->addTouchEventListener(this, toucheventselector(ClearScene::touchEvent));
	auto buttonReturn = dynamic_cast<Button*>(UIlayer->getChildByTag(CLEARSCENE_RETURN));
	buttonReturn->addTouchEventListener(this, toucheventselector(ClearScene::touchEvent));
	auto labelPerfect = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(CLEARSCENE_PNO));
	labelPerfect->setText(std::to_string(counterPerfect).c_str());
	auto labelGood = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(CLEARSCENE_GNO));
	labelGood->setText(std::to_string(counterGood).c_str());
	auto labelMiss = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(CLEARSCENE_MNO));
	labelMiss->setText(std::to_string(counterMiss).c_str());
	auto labelTotal = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(CLEARSCENE_TNO));
	labelTotal->setText(std::to_string(counterTotal).c_str());
	auto labelMaxCombo = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(CLEARSCENE_CNO));
	if (counterMaxcombo == 0)
		counterMaxcombo = counterTotal;//È«³ÌÎÞmiss
	labelMaxCombo->setText(std::to_string(counterMaxcombo).c_str());
	auto labelComplete = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(CLEARSCENE_COMPLETE));
	float completePercent = (float)counterPerfect / (float)counterTotal * 0.7;
	completePercent += (float)counterGood / (float)counterTotal * 0.6;
	completePercent += (float)counterMaxcombo / (float)counterTotal * 0.3;
	std::string cp = std::to_string(completePercent * 100).substr(0, 5) + "% Complete";
	labelComplete->setText(cp.c_str());
	return true;
}

void ClearScene::onEnterTransitionDidFinish()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/clear.mp3");
}

void ClearScene::onExit()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
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
			auto scene = GameScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
		}
		else if (tag == CLEARSCENE_RETURN)
		{
			auto scene = MainScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
		}
		break;
	}
}


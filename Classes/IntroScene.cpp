#include "IntroScene.h"
#include "MainScene.h"

Scene* IntroScene::createScene()
{
	auto scene = Scene::create();
	auto layer = IntroScene::create();
	scene->addChild(layer);
	return scene;
}

bool IntroScene::init()
{
	//super init
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->createLoop("music/intro.mp3");
	auto sceneNode = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("introScene.json");
	addChild(sceneNode);
	auto UINode = sceneNode->getChildByTag(10003);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("GUIComponent");
	auto UIlayer = UIComponent->getNode();
	auto background = dynamic_cast<ImageView*>(UIlayer->getChildByTag(INTROSCENE_BG));
	auto buttonStart = dynamic_cast<Button*>(UIlayer->getChildByTag(INTROSCENE_START));
	auto title = dynamic_cast<ImageView*>(UIlayer->getChildByTag(INTROSCENE_ECHO));
	title->runAction(RepeatForever::create(Sequence::create(FadeIn::create(3), FadeOut::create(3), NULL)));
	buttonStart->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1), DelayTime::create(1), FadeOut::create(1), NULL)));
	buttonStart->addTouchEventListener(this, toucheventselector(IntroScene::touchEvent));

	auto logo = Sprite::create("introSceneUI/logo.png");
	logo->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	logo->setLocalZOrder(1);
	sceneNode->addChild(logo);
	logo->runAction(Sequence::create(DelayTime::create(1), CallFunc::create(CC_CALLBACK_0(IntroScene::playIntro, this)), FadeOut::create(2), NULL));//logo�л���Ч
	return true;
}

void IntroScene::onExitTransitionDidStart()
{
	Layer::onExitTransitionDidStart();
	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->stop();
}

void IntroScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void IntroScene::playIntro()
{
	AudioEngine::getInstance()->play();
	auto emitter = ParticleSnow::createWithTotalParticles(1000);
	emitter->setGravity(Point(0, -20));
	emitter->setAutoRemoveOnFinish(true);
	addChild(emitter, 10);
}

void IntroScene::touchEvent(Ref* obj, TouchEventType eventType)
{
	auto widget = dynamic_cast<Widget*>(obj);
	int tag = widget->getTag();
	switch (eventType)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		if (tag == INTROSCENE_START)
		{
			auto scene = MainScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(2, scene, Color3B(255, 255, 255)));
		}
		break;
	}
}
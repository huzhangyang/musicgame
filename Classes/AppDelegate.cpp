#include "AppDelegate.h"
#include "IntroScene.h"
#include "GameScene.h"

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() {
	auto director = Director::getInstance();
	auto eglView = EGLView::getInstance();
	director->setOpenGLView(eglView);
	eglView->setDesignResolutionSize(1280, 720, ResolutionPolicy::SHOW_ALL);
	director->setDisplayStats(true);
	director->setAnimationInterval(1.0 / 60);//设置帧率
	auto scene = GameScene::createScene();
	director->runWithScene(scene);//运行场景
	return true;
}
//切入后台
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}
//切回前台
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

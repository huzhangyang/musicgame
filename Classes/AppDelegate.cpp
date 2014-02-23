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
	auto scene = IntroScene::createScene();
	director->runWithScene(scene);//运行场景
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	FileUtils::getInstance()->addSearchPath("/mnt/sdcard/");//设置搜索路径
#else
	FileUtils::getInstance()->addSearchPath("../files/");//设置搜索路径
#endif
	return true;
}
//切入后台
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->pause();
	AudioEngine::getInstance()->pause();

}
//切回前台
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->resume();
	AudioEngine::getInstance()->resume();
}

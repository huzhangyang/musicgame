#include "AppDelegate.h"
#include "IntroScene.h"

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
	director->setAnimationInterval(1.0 / 60);//����֡��
	auto scene = IntroScene::createScene();
	director->runWithScene(scene);//���г���
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	FileUtils::getInstance()->addSearchPath("/mnt/sdcard/");//��������·��
#else
	FileUtils::getInstance()->addSearchPath("../files/");//��������·��
#endif
	return true;
}
//�����̨
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->pause();
	AudioEngine::getInstance()->pause();

}
//�л�ǰ̨
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->resume();
	AudioEngine::getInstance()->resume();
}

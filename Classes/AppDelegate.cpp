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
	auto glview = director->getOpenGLView();
	if (!glview) {
		glview = GLView::createWithRect("Echo v0.4", Rect(300, 300, 960, 540));
		director->setOpenGLView(glview);
	}
	glview->setDesignResolutionSize(1280, 720, ResolutionPolicy::SHOW_ALL);
	director->setDisplayStats(true);
	director->setAnimationInterval(1.0 / 60);//设置帧率
	auto scene = IntroScene::createScene();
	director->runWithScene(scene);//运行场景
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	FileUtils::getInstance()->addSearchPath("/mnt/sdcard/");//设置搜索路径
#else
	FileUtils::getInstance()->addSearchPath("../files/");//设置搜索路径
#endif
	cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("note/note0.ExportJson");
	cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("note/note1.ExportJson");
	cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("note/note2.ExportJson");
	return true;
}
//切入后台
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();
	AudioEngine::getInstance()->pause();

}
//切回前台
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();
	AudioEngine::getInstance()->resume();
}

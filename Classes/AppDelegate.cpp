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
	director->setAnimationInterval(1.0 / 60);//����֡��
	auto scene = IntroScene::createScene();
	director->runWithScene(scene);//���г���
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	FileUtils::getInstance()->addSearchPath("/mnt/sdcard/");//��������·��
#else
	FileUtils::getInstance()->addSearchPath("../files/");//��������·��
#endif
	cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("note/note0.ExportJson");
	cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("note/note1.ExportJson");
	cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("note/note2.ExportJson");
	return true;
}
//�����̨
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();
	AudioEngine::getInstance()->pause();

}
//�л�ǰ̨
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();
	AudioEngine::getInstance()->resume();
}

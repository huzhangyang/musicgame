#include "MainScene.h"
#include "GameScene.h"
#include "SelectScene.h"

Scene* MainScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainScene::create();
	scene->addChild(layer);
	return scene;
}

bool MainScene::init()
{
	//super init
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->createLoop("bgm/main.ogg");
	auto sceneNode = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("mainScene.json");
	addChild(sceneNode);
	auto UINode = sceneNode->getChildByTag(10005);
	ExitNode = sceneNode->getChildByTag(10004);
	DialogNode = sceneNode->getChildByTag(10006);
	HelpNode = sceneNode->getChildByTag(10007);
	OptionNode = sceneNode->getChildByTag(10008);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("mainSceneUI");
	auto ExitComponent = (cocostudio::ComRender*) ExitNode->getComponent("exitSelectUI");
	auto DialogComponent = (cocostudio::ComRender*) DialogNode->getComponent("dialogBoxUI");
	auto HelpComponent = (cocostudio::ComRender*) HelpNode->getComponent("helpUI");
	auto OptionComponent = (cocostudio::ComRender*) OptionNode->getComponent("optionUI");
	auto UILayer = (Layer*)UIComponent->getNode();
	auto ExitLayer = (Layer*)ExitComponent->getNode();
	auto DialogLayer = (Layer*)DialogComponent->getNode();
	auto HelpLayer = (Layer*)HelpComponent->getNode();
	auto OptionLayer = (Layer*)OptionComponent->getNode();
	//////////
	auto imageTable = dynamic_cast<ImageView*>(UILayer->getChildByTag(MAINSCENE_IMAGE_TABLE));
	auto imagePaper = dynamic_cast<Button*>(UILayer->getChildByTag(MAINSCENE_IMAGE_PAPER));
	auto imageShelf = dynamic_cast<Button*>(UILayer->getChildByTag(MAINSCENE_IMAGE_SHELF));
	auto imageClock = dynamic_cast<ImageView*>(UILayer->getChildByTag(MAINSCENE_IMAGE_CLOCK));
	auto imageCat = dynamic_cast<ImageView*>(UILayer->getChildByTag(MAINSCENE_IMAGE_CAT));
	auto imageCharacter = dynamic_cast<ImageView*>(UILayer->getChildByTag(MAINSCENE_IMAGE_CHARACTER));
	auto buttonOption = dynamic_cast<Button*>(UILayer->getChildByTag(MAINSCENE_BUTTON_OPTION));
	auto buttonHelp = dynamic_cast<Button*>(UILayer->getChildByTag(MAINSCENE_BUTTON_HELP));
	auto buttonExit = dynamic_cast<Button*>(UILayer->getChildByTag(MAINSCENE_BUTTON_EXIT));
	imageTable->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	imagePaper->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	imageShelf->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	imageClock->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	imageCat->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	imageCharacter->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	buttonOption->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	buttonHelp->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	buttonExit->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	//////////
	auto bgExit = dynamic_cast<ImageView*>(ExitLayer->getChildByTag(MAINSCENE_EXIT_BG));
	auto buttonYes = dynamic_cast<Button*>(ExitLayer->getChildByTag(MAINSCENE_EXIT_BUTTON_YES));
	auto buttonNo = dynamic_cast<Button*>(ExitLayer->getChildByTag(MAINSCENE_EXIT_BUTTON_NO));
	bgExit->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	buttonYes->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	buttonNo->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	bgExit->setEnabled(false);
	buttonYes->setEnabled(false);
	buttonNo->setEnabled(false);
	//////////
	auto bgDialog = dynamic_cast<ImageView*>(DialogLayer->getChildByTag(MAINSCENE_DIALOG_BG));
	bgDialog->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	bgDialog->setEnabled(false);
	//////////
	auto boxEasy = dynamic_cast<CheckBox*>(OptionLayer->getChildByTag(MAINSCENE_SETTING_EASY));
	auto boxHard = dynamic_cast<CheckBox*>(OptionLayer->getChildByTag(MAINSCENE_SETTING_HARD));
	auto boxScanline = dynamic_cast<CheckBox*>(OptionLayer->getChildByTag(MAINSCENE_SETTING_SCANLINE));
	auto sliderLag = dynamic_cast<Slider*>(OptionLayer->getChildByTag(MAINSCENE_SETTING_SLIDER));
	auto buttonClose = dynamic_cast<Button*>(OptionLayer->getChildByTag(MAINSCENE_SETTING_CLOSE));
	auto bgSetting = dynamic_cast<ImageView*>(OptionLayer->getChildByTag(MAINSCENE_SETTING_BG));
	boxEasy->addEventListenerCheckBox(this, checkboxselectedeventselector(MainScene::checkboxEvent));
	boxHard->addEventListenerCheckBox(this, checkboxselectedeventselector(MainScene::checkboxEvent));
	boxScanline->addEventListenerCheckBox(this, checkboxselectedeventselector(MainScene::checkboxEvent));
	sliderLag->addEventListenerSlider(this, sliderpercentchangedselector(MainScene::sliderEvent));
	buttonClose->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	bgSetting->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	bgSetting->setEnabled(false);
	buttonClose->setEnabled(false);
	sliderLag->setEnabled(false);
	boxEasy->setEnabled(false);
	boxHard->setEnabled(false);
	boxScanline->setEnabled(false);
	//////////
	auto page = dynamic_cast<PageView*>(HelpLayer->getChildByTag(MAINSCENE_HELP_PAGE));
	auto bgHelp = dynamic_cast<ImageView*>(HelpLayer->getChildByTag(MAINSCENE_HELP_BG));
	bgHelp->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	bgHelp->setEnabled(false);
	page->setEnabled(false);
	return true;
}

void MainScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->play();
}

void MainScene::onExitTransitionDidStart()
{
	Layer::onExitTransitionDidStart();
	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->stop();
}

void MainScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void MainScene::createDialog(std::string key)
{
	auto DialogComponent = (cocostudio::ComRender*) DialogNode->getComponent("dialogBoxUI");
	auto DialogLayer = (Layer*)DialogComponent->getNode();
	auto labelWord = dynamic_cast<Text*>(DialogLayer->getChildByTag(MAINSCENE_DIALOG_WORDS));
	auto bgDialog = dynamic_cast<ImageView*>(DialogLayer->getChildByTag(MAINSCENE_DIALOG_BG));
	ValueMap strings = FileUtils::getInstance()->getValueMapFromFile("strings.xml");
	DialogNode->setVisible(true);
	bgDialog->setEnabled(true);
	labelWord->setText(strings.at(key).asString());
}

void MainScene::touchEvent(Ref* obj, TouchEventType eventType)
{
	auto widget = dynamic_cast<Widget*>(obj);
	int tag = widget->getTag();
	auto ExitComponent = (cocostudio::ComRender*) ExitNode->getComponent("exitSelectUI");
	auto DialogComponent = (cocostudio::ComRender*) DialogNode->getComponent("dialogBoxUI");
	auto HelpComponent = (cocostudio::ComRender*) HelpNode->getComponent("helpUI");
	auto OptionComponent = (cocostudio::ComRender*) OptionNode->getComponent("optionUI");
	auto ExitLayer = (Layer*)ExitComponent->getNode();
	auto DialogLayer = (Layer*)DialogComponent->getNode();
	auto HelpLayer = (Layer*)HelpComponent->getNode();
	auto OptionLayer = (Layer*)OptionComponent->getNode();
	auto bgExit = dynamic_cast<ImageView*>(ExitLayer->getChildByTag(MAINSCENE_EXIT_BG));
	auto bgDialog = dynamic_cast<ImageView*>(DialogLayer->getChildByTag(MAINSCENE_DIALOG_BG));
	auto buttonYes = dynamic_cast<Button*>(ExitLayer->getChildByTag(MAINSCENE_EXIT_BUTTON_YES));
	auto buttonNo = dynamic_cast<Button*>(ExitLayer->getChildByTag(MAINSCENE_EXIT_BUTTON_NO));
	auto boxEasy = dynamic_cast<CheckBox*>(OptionLayer->getChildByTag(MAINSCENE_SETTING_EASY));
	auto boxHard = dynamic_cast<CheckBox*>(OptionLayer->getChildByTag(MAINSCENE_SETTING_HARD));
	auto boxScanline = dynamic_cast<CheckBox*>(OptionLayer->getChildByTag(MAINSCENE_SETTING_SCANLINE));
	auto sliderLag = dynamic_cast<Slider*>(OptionLayer->getChildByTag(MAINSCENE_SETTING_SLIDER));
	auto buttonClose = dynamic_cast<Button*>(OptionLayer->getChildByTag(MAINSCENE_SETTING_CLOSE));
	auto labelLag = dynamic_cast<Text*>(OptionLayer->getChildByTag(MAINSCENE_SETTING_SNO));
	auto bgSetting = dynamic_cast<ImageView*>(OptionLayer->getChildByTag(MAINSCENE_SETTING_BG));
	auto bgHelp = dynamic_cast<ImageView*>(HelpLayer->getChildByTag(MAINSCENE_HELP_BG));
	auto page = dynamic_cast<PageView*>(HelpLayer->getChildByTag(MAINSCENE_HELP_PAGE));
	Scene* scene;
	char temp[64];
	int lag = UserDefault::getInstance()->getIntegerForKey("lag");
	sprintf(temp, "%.2f", lag / 100.0);
	if (eventType == TouchEventType::TOUCH_EVENT_ENDED)
	{
		switch (tag)
		{
		case MAINSCENE_IMAGE_TABLE:
			this->createDialog("dialogTable");
			break;
		case MAINSCENE_IMAGE_PAPER:
			scene = SelectScene::createScene(0);
			Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
			break;
		case MAINSCENE_IMAGE_SHELF:
			scene = SelectScene::createScene(1);
			Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
			break;
		case MAINSCENE_IMAGE_CLOCK:
			this->createDialog("dialogClock");
			break;
		case MAINSCENE_IMAGE_CAT:
			this->createDialog("dialogCat");
			break;
		case MAINSCENE_BUTTON_OPTION:
			OptionNode->setVisible(true);
			bgSetting->setEnabled(true);
			sliderLag->setEnabled(true);
			boxEasy->setEnabled(true);
			boxHard->setEnabled(true);
			boxScanline->setEnabled(true);
			buttonClose->setEnabled(true);
			if (UserDefault::getInstance()->getBoolForKey("scanline"))
				boxScanline->setSelectedState(true);
			if (UserDefault::getInstance()->getIntegerForKey("difficulty") == 0)
				boxEasy->setSelectedState(true);
			else
				boxHard->setSelectedState(true);
			sliderLag->setPercent(lag);
			labelLag->setText(temp);
			break;
		case MAINSCENE_BUTTON_HELP:
			HelpNode->setVisible(true);
			bgHelp->setEnabled(true);
			page->setEnabled(true);
			break;
		case MAINSCENE_HELP_BG:
			HelpNode->setVisible(false);
			bgHelp->setEnabled(false);
			page->setEnabled(false);
			break;
		case MAINSCENE_BUTTON_EXIT:
			ExitNode->setVisible(true);
			bgExit->setEnabled(true);
			buttonYes->setEnabled(true);
			buttonNo->setEnabled(true);
			break;
		case  MAINSCENE_EXIT_BUTTON_YES:
			Director::getInstance()->popScene();
			break;
		case  MAINSCENE_EXIT_BUTTON_NO:
			ExitNode->setVisible(false);
			bgExit->setEnabled(false);
			buttonYes->setEnabled(false);
			buttonNo->setEnabled(false);
			break;
		case MAINSCENE_IMAGE_CHARACTER:
			this->createDialog("dialogCharacter");
			break;
		case MAINSCENE_DIALOG_BG:
			DialogNode->setVisible(false);
			bgDialog->setEnabled(false);
			break;
		case MAINSCENE_SETTING_CLOSE:
			OptionNode->setVisible(false);
			bgSetting->setEnabled(false);
			sliderLag->setEnabled(false);
			boxEasy->setEnabled(false);
			boxHard->setEnabled(false);
			boxScanline->setEnabled(false);
			buttonClose->setEnabled(false);
			break;
		}
	}
}

void MainScene::checkboxEvent(Ref* obj, CheckBoxEventType eventType)
{
	auto OptionComponent = (cocostudio::ComRender*) OptionNode->getComponent("optionUI");
	auto OptionLayer = (Layer*)OptionComponent->getNode();
	auto boxEasy = dynamic_cast<CheckBox*>(OptionLayer->getChildByTag(MAINSCENE_SETTING_EASY));
	auto boxHard = dynamic_cast<CheckBox*>(OptionLayer->getChildByTag(MAINSCENE_SETTING_HARD));
	auto widget = dynamic_cast<CheckBox*>(obj);
	int tag = widget->getTag();
	if (eventType == CheckBoxEventType::CHECKBOX_STATE_EVENT_SELECTED)
	{
		switch (tag)
		{
		case MAINSCENE_SETTING_EASY:
			if (boxHard->getSelectedState() == true)
				boxHard->setSelectedState(false);
			UserDefault::getInstance()->setIntegerForKey("difficulty", 0);
			break;
		case MAINSCENE_SETTING_HARD:
			if (boxEasy->getSelectedState() == true)
				boxEasy->setSelectedState(false);
			UserDefault::getInstance()->setIntegerForKey("difficulty", 1);
			break;
		case MAINSCENE_SETTING_SCANLINE:
			UserDefault::getInstance()->setBoolForKey("scanline", true);
			break;
		}
	}
	else if (eventType == CheckBoxEventType::CHECKBOX_STATE_EVENT_UNSELECTED)
	{
		switch (tag)
		{
		case MAINSCENE_SETTING_EASY:
			if (boxHard->getSelectedState() == false)
				boxHard->setSelectedState(true);
			UserDefault::getInstance()->setIntegerForKey("difficulty", 1);
			break;
		case MAINSCENE_SETTING_HARD:
			if (boxEasy->getSelectedState() == false)
				boxEasy->setSelectedState(true);
			UserDefault::getInstance()->setIntegerForKey("difficulty", 0);
			break;
		case MAINSCENE_SETTING_SCANLINE:
			UserDefault::getInstance()->setBoolForKey("scanline", false);
			break;
		}
	}
}

void MainScene::sliderEvent(Ref* obj, SliderEventType eventType)
{
	auto widget = dynamic_cast<Slider*>(obj);
	auto OptionComponent = (cocostudio::ComRender*) OptionNode->getComponent("optionUI");
	auto OptionLayer = (Layer*)OptionComponent->getNode();
	auto labelLag = dynamic_cast<Text*>(OptionLayer->getChildByTag(MAINSCENE_SETTING_SNO));
	char temp[64];
	int lag;
	if (eventType == SliderEventType::SLIDER_PERCENTCHANGED)
	{
		lag = widget->getPercent();
		UserDefault::getInstance()->setIntegerForKey("lag", lag);
		sprintf(temp, "%.2f", lag / 100.0);
		labelLag->setText(temp);
	}
}
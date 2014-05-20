#include "SelectScene.h"
#include "MainScene.h"
#include "GameScene.h"
#include "MapUtils.h"

int selectMode;//0为制谱，1为游戏
Node *LoadingNode;
ListView* list;
std::vector<std::string> filepaths;
float BPM = 120.00;

Scene* SelectScene::createScene(int mode)
{
	auto scene = Scene::create();
	auto layer = SelectScene::create();
	scene->addChild(layer);
	selectMode = mode;
	return scene;

}

bool SelectScene::init()
{
	//super init
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////////////////////////////
	auto sceneNode = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("selectScene.json");
	addChild(sceneNode);
	UINode = sceneNode->getChildByTag(10003);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("selectScene");
	auto UILayer = UIComponent->getNode();
	LoadingNode = sceneNode->getChildByTag(10007);
	auto LoadingComponent = (cocostudio::ComRender*) LoadingNode->getComponent("loadingUI");
	auto LoadingLayer = (Layer*)LoadingComponent->getNode();
	list = dynamic_cast<ListView*>(UILayer->getChildByTag(SELECTSCENE_LIST));
	auto bg = dynamic_cast<ImageView*>(UILayer->getChildByTag(SELECTSCENE_LIST)->getChildByTag(SELECTSCENE_BG));
	auto info = dynamic_cast<Text*>(bg->getChildByTag(SELECTSCENE_INFO));
	auto labelDifficulty = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_DIFFICULTY));
	auto buttonReturn = dynamic_cast<Button*>(UILayer->getChildByTag(SELECTSCENE_RETURN));
	auto buttonStart = dynamic_cast<Button*>(UILayer->getChildByTag(SELECTSCENE_START));
	//////////
	buttonStart->addTouchEventListener(this, toucheventselector(SelectScene::touchEvent));
	buttonReturn->addTouchEventListener(this, toucheventselector(SelectScene::touchEvent));
	list->addEventListenerListView(this, listvieweventselector(SelectScene::listViewEvent));
	list->setItemModel(bg);
	list->setEnabled(false);
	//////////
	auto bgLoading = dynamic_cast<ImageView*>(LoadingLayer->getChildByTag(SELECTSCENE_LOADING_BG));
	auto imageWords = dynamic_cast<ImageView*>(LoadingLayer->getChildByTag(SELECTSCENE_LOADING_WORDS));
	auto imageLight = dynamic_cast<ImageView*>(LoadingLayer->getChildByTag(SELECTSCENE_LOADING_LIGHT));
	bgLoading->setEnabled(false);
	imageLight->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1), FadeOut::create(1), NULL)));
	imageWords->runAction(RepeatForever::create(RotateBy::create(5, 360)));
	return true;
}

void SelectScene::onEnterTransitionDidFinish()
{

	Layer::onEnterTransitionDidFinish();
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("selectScene");
	auto UILayer = UIComponent->getNode();
	auto labelDifficulty = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_DIFFICULTY));
	auto labelLevel = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_LEVEL));
	auto labelScore = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_SCORE));
	/////////////////////////////////////////////////////
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	scan_dir("/mnt/sdcard/Echo/music/");//设置搜索路径
#else
	scan_dir("../music/");//设置搜索路径
#endif
	int index = 0;
	/*加载歌曲list*/
	for (auto& filepath : filepaths)
	{
		auto filename = filepath.substr(filepath.find_last_of('/') + 1, filepath.find_last_of('.') - filepath.find_last_of('/') - 1);
		auto mapname = FileUtils::getInstance()->getWritablePath() + filename + ".gnm";
		if (selectMode == 0)
		{
			if (index > 0)
				list->pushBackDefaultItem();
			((Text*)(list->getItem(index++)->getChildByTag(SELECTSCENE_INFO)))->setText(filename);
			if (!FileUtils::getInstance()->isFileExist(mapname))
				((ImageView*)list->getItem(index - 1))->loadTexture("selectSceneUI/songinformationBG2.png");
		}
		else if (selectMode == 1 && FileUtils::getInstance()->isFileExist(mapname))
		{
			if (index > 0)
				list->pushBackDefaultItem();
			((Text*)(list->getItem(index++)->getChildByTag(SELECTSCENE_INFO)))->setText(filename);
		}
	}
	/*获取选中歌曲的难度，以及播放歌曲预览*/
	auto bg = (ImageView*)list->getItem(0);
	bg->loadTexture("selectSceneUI/songinformationBG1.png");
	auto FileName = ((Text*)(bg->getChildByTag(SELECTSCENE_INFO)))->getStringValue();
	if (selectMode == 1 && FileName != "")
	{
		auto difficulty = UserDefault::getInstance()->getIntegerForKey("difficulty");//获取当前难度
		MusicInfo musicinfo = MapUtils::loadMap(FileName.c_str());
		if (difficulty == 0)
		{
			labelDifficulty->setText("Easy");
			labelLevel->setText("LV." + musicinfo.Level_Easy);
			labelLevel->setColor(Color3B(45, 65, 30));
			labelDifficulty->setColor(Color3B(45, 65, 30));
		}
		else if (difficulty == 1)
		{
			labelDifficulty->setText("Hard");
			labelLevel->setText("LV." + musicinfo.Level_Hard);
			labelLevel->setColor(Color3B(150, 15, 15));
			labelDifficulty->setColor(Color3B(150, 15, 15));
		}
	}
	if (FileName != "")
	{
		list->setEnabled(true);
		auto musicname = FileUtils::getInstance()->fullPathForFilename("music/" + FileName + ".mp3");
		AudioEngine::getInstance()->create(musicname.c_str());
		AudioEngine::getInstance()->play();
		AudioEngine::getInstance()->setPosition(AudioEngine::getInstance()->getLength() / 2);
	}
}

void SelectScene::onExitTransitionDidStart()
{
	Layer::onExitTransitionDidStart();
	/////////////////////////////////////////////////////
	filepaths.clear();
	MapUtils::closeMap();
	AudioEngine::getInstance()->stop();
}

void SelectScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void SelectScene::loadingEnd()
{
	auto LoadingComponent = (cocostudio::ComRender*) LoadingNode->getComponent("loadingUI");
	auto LoadingLayer = (Layer*)LoadingComponent->getNode();
	auto bgLoading = dynamic_cast<ImageView*>(LoadingLayer->getChildByTag(SELECTSCENE_LOADING_BG));
	LoadingNode->setVisible(false);
	bgLoading->setEnabled(false);
	auto index = list->getCurSelectedIndex();
	auto bg = (ImageView*)list->getItem(index);
	bg->loadTexture("selectSceneUI/songinformationBG.png");
}

void SelectScene::touchEvent(Ref* obj, TouchEventType eventType)
{
	auto LoadingComponent = (cocostudio::ComRender*) LoadingNode->getComponent("loadingUI");
	auto LoadingLayer = (Layer*)LoadingComponent->getNode();
	auto bgLoading = dynamic_cast<ImageView*>(LoadingLayer->getChildByTag(SELECTSCENE_LOADING_BG));
	auto index = list->getCurSelectedIndex();
	auto bg = list->getItem(index);
	auto info = dynamic_cast<Text*>(bg->getChildByTag(SELECTSCENE_INFO));
	auto widget = dynamic_cast<Widget*>(obj);
	int tag = widget->getTag();
	Scene* scene;
	switch (eventType)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		switch (tag)
		{
		case SELECTSCENE_RETURN:
			scene = MainScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
			break;
		case SELECTSCENE_START:
			if (selectMode == 0 && list->isEnabled())
			{
				MapUtils::generateMap(info->getStringValue());
			}
			else if (selectMode == 1 && list->isEnabled())
			{
				scene = GameScene::createScene(info->getStringValue());
				Director::getInstance()->replaceScene(TransitionPageTurn::create(2, scene, true));
			}
			break;
		}
		break;
	}
}

void SelectScene::listViewEvent(Ref* obj, ListViewEventType eventType)
{
	auto index = list->getCurSelectedIndex();
	auto bg = (ImageView*)list->getItem(index);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("selectScene");
	auto UILayer = UIComponent->getNode();
	auto info = dynamic_cast<Text*>(bg->getChildByTag(SELECTSCENE_INFO));
	auto labelLevel = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_LEVEL));
	auto labelScore = dynamic_cast<Text*>(UILayer->getChildByTag(SELECTSCENE_SCORE));
	auto difficulty = UserDefault::getInstance()->getIntegerForKey("difficulty");//获取当前难度

	switch (eventType)
	{
	case ListViewEventType::LISTVIEW_ONSELECTEDITEM_END:
		for (int i = 0; i < list->getItems().size(); i++)
		{
			auto currbg = (ImageView*)list->getItem(i);
			auto currinfo = dynamic_cast<Text*>(currbg->getChildByTag(SELECTSCENE_INFO));
			auto mapname = FileUtils::getInstance()->getWritablePath() + currinfo->getStringValue() + ".gnm";
			if (i == index)
				currbg->loadTexture("selectSceneUI/songinformationBG1.png");
			else if (FileUtils::getInstance()->isFileExist(mapname))
				currbg->loadTexture("selectSceneUI/songinformationBG.png");
			else
				currbg->loadTexture("selectSceneUI/songinformationBG2.png");
		}
		MapUtils::closeMap();
		auto FileName = info->getStringValue();
		if (selectMode == 1 && FileName != "")
		{
			MusicInfo musicinfo = MapUtils::loadMap(FileName.c_str());
			if (difficulty == 0)
			{
				labelLevel->setText("LV." + musicinfo.Level_Easy);
			}
			else if (difficulty == 1)
			{
				labelLevel->setText("LV." + musicinfo.Level_Hard);
			}
		}
		auto musicname = FileUtils::getInstance()->fullPathForFilename("music/" + FileName + ".mp3");
		AudioEngine::getInstance()->create(musicname.c_str());
		AudioEngine::getInstance()->play();
		AudioEngine::getInstance()->setPosition(AudioEngine::getInstance()->getLength() *0.3);
		break;
	}
}

void SelectScene::scan_dir(std::string path)
{
#ifdef WIN32
	_finddata_t FileInfo;
	std::string strfind = path + "\\*";
	long Handle = _findfirst(strfind.c_str(), &FileInfo);

	if (Handle == -1L)
	{
		log("open error!");
		return;
	}
	do{
		if (FileInfo.attrib & _A_SUBDIR)
		{
			if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
			{
				std::string newPath = path + FileInfo.name;
				log("%s\n", newPath.c_str());
				scan_dir(newPath);//递归遍历子目录
			}
		}
		else
		{
			std::string filename = (path + FileInfo.name);//得到文件名
			filepaths.push_back(filename);
			log("%s\n", filename.c_str());
		}
	} while (_findnext(Handle, &FileInfo) == 0);

	_findclose(Handle);
#else
	DIR *dp;  
	struct dirent *entry;  
	struct stat statbuf;  

	if((dp = opendir(path.c_str())) == NULL)  
	{  
		log("open error!");  
		return;  
	}  
	chdir(path.c_str());  
	while ((entry = readdir(dp)) != NULL) {  
		stat(entry->d_name, &statbuf);  
		if (S_ISDIR(statbuf.st_mode)) {  
			if ((strcmp(entry->d_name, ".") != 0)  
				&& (strcmp(entry->d_name, "..") != 0)  
				&& (entry->d_name[0] != '.')) {  
				scan_dir(entry->d_name);  
			}  
		} else {  
			int size = strlen(entry->d_name);  
			if (entry->d_name[0] != '.'&& strcmp(entry->d_name + (size - 4), ".mp3") == 0){  
				char* parentPath = (char*)malloc(1024);
				char* absolutePath = (char*)malloc(1024);
				getcwd(parentPath, 1024);
				strcpy(absolutePath, parentPath);
				absolutePath = strcat(absolutePath, "/");
				absolutePath = strcat(absolutePath, entry->d_name);
				log("%s\n", absolutePath);
				filepaths.push_back(absolutePath);
				free(parentPath);
				parentPath = NULL;
				free(absolutePath);
				absolutePath = NULL;
			}
		}
	}
	chdir("..");
	closedir(dp);
#endif
}
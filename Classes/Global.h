#ifndef __GLOBAL_H__
#define __GLOBAL_H__
//计数器结构
struct Counter
{
	int total = 0;//音符总数
	int perfect = 0;//完美数
	int good = 0;//普通击中数
	int miss = 0;//错过数
	int combo = 0;//连击数
	float percent = 0;//总分
};
extern Counter counter;
//曲谱结构
struct Noteline
{
	int time = 0;//时间
	int difficulty = 0;//难度
	int type = 0;//类型
	int length = 0;//长音符与滑动音符持续长度
	int posX = 0;//X坐标
	int posY = 0;//Y坐标
};
extern Noteline noteline;
//Beat信息结构
struct BeatInfo
{
	int beginTime = 0;
	int endTime = 0;
	int lastEasyTime = 0;
	int lastHardTime = 0;
	float maxPeak = 0;
};
//曲谱信息结构
struct MusicInfo
{
	int length = 0;
	int NoteNumber_Easy = 0;
	int NoteNumber_Hard = 0;
	std::string Level_Easy = "";
	std::string Level_Hard = "";
};
//文件信息结构
struct FileInfo
{
	std::string name;
	float BPM;
	float score;
};
//开始界面Tag
const int INTROSCENE_BG = 2;
const int INTROSCENE_START = 3;
const int INTROSCENE_ECHO = 6;
//主界面Tag
const int MAINSCENE_BUTTON_OPTION = 7;
const int MAINSCENE_BUTTON_HELP = 8;
const int MAINSCENE_BUTTON_EXIT = 9;
const int MAINSCENE_IMAGE_CHARACTER = 10;
const int MAINSCENE_IMAGE_CAT = 11;
const int MAINSCENE_IMAGE_SHELF = 12;
const int MAINSCENE_IMAGE_TABLE = 13;
const int MAINSCENE_IMAGE_PAPER = 14;
const int MAINSCENE_IMAGE_CLOCK = 15;
const int MAINSCENE_DIALOG_BG = 2;
const int MAINSCENE_DIALOG_WORDS = 5;
const int MAINSCENE_EXIT_BUTTON_YES = 3;
const int MAINSCENE_EXIT_BUTTON_NO = 4;
const int MAINSCENE_EXIT_BG = 6;
const int MAINSCENE_SETTING_EASY = 20;
const int MAINSCENE_SETTING_HARD = 21;
const int MAINSCENE_SETTING_SLIDER = 22;
const int MAINSCENE_SETTING_SCANLINE = 23;
const int MAINSCENE_SETTING_BG = 24;
const int MAINSCENE_SETTING_CLOSE = 25;
const int MAINSCENE_SETTING_SNO = 26;
const int MAINSCENE_HELP_BG = 4;
const int MAINSCENE_HELP_PAGE = 5;
const int MAINSCENE_HELP_P1 = 6;
const int MAINSCENE_HELP_P2 = 7;

//游戏界面Tag
const int GAMESCENE_BUTTON_PAUSE = 5;
const int GAMESCENE_LOADINGBAR = 9;
const int GAMESCENE_LABEL_COMBO = 6;
const int GAMESCENE_LABEL_DIFFICULTY = 7;
const int GAMESCENE_LABEL_INFO = 8;
const int GAMESCENE_LABEL_PERCENT = 10;
const int GAMESCENE_LABEL_LEVEL = 12;
const int GAMESCENE_MENU_BG = 3;
const int GAMESCENE_MENU_RESUME = 14;
const int GAMESCENE_MENU_RETRY = 15;
const int GAMESCENE_MENU_RETURN = 17;
//结算界面Tag
const int CLEARSCENE_BG = 2;
const int CLEARSCENE_TNO = 3;
const int CLEARSCENE_PNO = 4;
const int CLEARSCENE_GNO = 5;
const int CLEARSCENE_MNO = 6;
const int CLEARSCENE_CNO = 7;
const int CLEARSCENE_INFO = 12;
const int CLEARSCENE_JUDGE = 13;
const int CLEARSCENE_RETRY = 14;
const int CLEARSCENE_RETURN = 15;
const int CLEARSCENE_DIFFICULTY = 16;
const int CLEARSCENE_LEVEL = 17;
//选曲界面Tag
const int SELECTSCENE_LIST = 5;
const int SELECTSCENE_INFO = 6;
const int SELECTSCENE_BG = 7;
const int SELECTSCENE_LEVEL = 8;
const int SELECTSCENE_DIFFICULTY = 9;
const int SELECTSCENE_SCORE = 10;
const int SELECTSCENE_RETURN = 11;
const int SELECTSCENE_START= 12;
const int SELECTSCENE_LOADING_BG = 16;
const int SELECTSCENE_LOADING_LIGHT = 18;
const int SELECTSCENE_LOADING_WORDS = 19;
//其它全局变量
extern float BPM;
#endif 

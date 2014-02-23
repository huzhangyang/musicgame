#ifndef __GLOBAL_H__
#define __GLOBAL_H__

struct Counter//计数器结构
{
	int frame;//帧数
	int total;//音符总数
	int perfect;//完美数
	int good;//普通击中数
	int miss;//错过数
	int combo;//连击数
	int maxcombo;//最大连击数
};
extern Counter counter;

//开始界面Tag
const int INTROSCENE_START = 3;
//主界面Tag
const int MAINSCENE_OPTION = 7;
const int MAINSCENE_HELP = 8;
const int MAINSCENE_EXIT = 9;
const int MAINSCENE_CAT = 11;
const int MAINSCENE_SHELF = 12;
const int MAINSCENE_TABLE = 13;
const int MAINSCENE_PAPER = 14;
const int MAINSCENE_CLOCK = 15;
const int MAINSCENE_CHECKMARK = 3;
const int MAINSCENE_CROSSMARK = 4;

//游戏界面Tag
const int GAMESCENE_PAUSE = 5;
const int GAMESCENE_COMBO = 6;
const int GAMESCENE_JUDGE = 7;
const int GAMESCENE_INFO = 8;
//结算界面Tag
const int CLEARSCENE_PNO = 6;
const int CLEARSCENE_GNO = 7;
const int CLEARSCENE_MNO = 8;
const int CLEARSCENE_TNO = 11;
const int CLEARSCENE_CNO = 12;
const int CLEARSCENE_COMPLETE = 13;
const int CLEARSCENE_RETRY = 14;
const int CLEARSCENE_RETURN = 15;

#endif 

#ifndef __GLOBAL_H__
#define __GLOBAL_H__
//全局变量
extern int counterTotal, counterPerfect, counterGood, counterMiss, counterCombo, counterMaxcombo;
//开始界面Tag
const int INTROSCENE_START = 3;
//主界面Tag
const int MAINSCENE_INK = 14;
const int MAINSCENE_SHELF = 12;
const int MAINSCENE_OPTION = 7;
const int MAINSCENE_HELP = 8;
const int MAINSCENE_EXIT = 9;
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

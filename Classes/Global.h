#ifndef __GLOBAL_H__
#define __GLOBAL_H__
//�������ṹ
struct Counter
{
	int total = 0;//��������
	int perfect = 0;//������
	int good = 0;//��ͨ������
	int miss = 0;//�����
	int combo = 0;//������
	float percent = 0;//�ܷ�
};
extern Counter counter;
//���׽ṹ
struct Noteline
{
	int time = 0;//ʱ��
	int difficulty = 0;//�Ѷ�
	int type = 0;//����
	int length = 0;//�������뻬��������������
	int posX = 0;//X����
	int posY = 0;//Y����
};
extern Noteline noteline;
//Beat��Ϣ�ṹ
struct BeatInfo
{
	int beginTime = 0;
	int endTime = 0;
	int lastEasyTime = 0;
	int lastHardTime = 0;
	float maxPeak = 0;
};
//������Ϣ�ṹ
struct MusicInfo
{
	int length = 0;
	int NoteNumber_Easy = 0;
	int NoteNumber_Hard = 0;
	std::string Level_Easy = "";
	std::string Level_Hard = "";
};
//�ļ���Ϣ�ṹ
struct FileInfo
{
	std::string name;
	float BPM;
	float score;
};
//��ʼ����Tag
const int INTROSCENE_BG = 2;
const int INTROSCENE_START = 3;
const int INTROSCENE_ECHO = 6;
//������Tag
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

//��Ϸ����Tag
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
//�������Tag
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
//ѡ������Tag
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
//����ȫ�ֱ���
extern float BPM;
#endif 

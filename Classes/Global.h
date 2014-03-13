#ifndef __GLOBAL_H__
#define __GLOBAL_H__
//�������ṹ
struct Counter
{
	int total;//��������
	int perfect;//������
	int good;//��ͨ������
	int miss;//�����
	int combo;//������
	float percent;//�ܷ�
};
extern Counter counter;
//���׽ṹ
struct Noteline
{
	int time;//ʱ��
	int difficulty;//�Ѷ�
	int type;//����
	int length;//�������뻬��������������
	int posX;//X����
	int posY;//Y����
};
extern Noteline noteline;
//��ʼ����Tag
const int INTROSCENE_BG = 2;
const int INTROSCENE_START = 3;
const int INTROSCENE_ECHO = 6;
//������Tag
const int MAINSCENE_OPTION = 7;
const int MAINSCENE_HELP = 8;
const int MAINSCENE_EXIT = 9;
const int MAINSCENE_CHARACTER = 10;
const int MAINSCENE_CAT = 11;
const int MAINSCENE_SHELF = 12;
const int MAINSCENE_TABLE = 13;
const int MAINSCENE_PAPER = 14;
const int MAINSCENE_CLOCK = 15;
const int MAINSCENE_DIALOGBG = 2;
const int MAINSCENE_CHECKMARK = 3;
const int MAINSCENE_CROSSMARK = 4;
const int MAINSCENE_WORD = 5;
const int MAINSCENE_EXITBG = 6;
const int MAINSCENE_LOADINGBG = 16;
const int MAINSCENE_LOADINGLIGHT = 18;
const int MAINSCENE_LOADINGWORDS = 19;
//��Ϸ����Tag
const int GAMESCENE_PAUSE = 5;
const int GAMESCENE_COMBO = 6;
const int GAMESCENE_DIFFICULTY = 7;
const int GAMESCENE_INFO = 8;
const int GAMESCENE_LOADINGBAR = 9;
const int GAMESCENE_PERCENT = 10;
const int GAMESCENE_PAUSEBG = 3;
const int GAMESCENE_RESUME = 14;
const int GAMESCENE_RETRY = 15;
const int GAMESCENE_OPTION = 16;
const int GAMESCENE_RETURN = 17;
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

//��������
const std::string FILENAME = "test";//�����ļ���
const float BPM = 139.65f;//�����ٶ�
#endif 

#pragma once
// �v���g�^�C�v�錾

#include<Windows.h>
#include<chrono>
#include<vector>
#include<string>

using namespace std;

bool CreateControl(HWND, WPARAM, LPARAM);
bool OnCreate(HWND, WPARAM, LPARAM);
bool OnCommand(HWND, WPARAM);
bool SendNowTimeToEdit(HWND, const char*);
bool OnFont(HWND);
const char* getMilliTime(std::chrono::system_clock::time_point);
bool SendMarginTimeToEdit(std::chrono::system_clock::duration);

bool ReadIniFile(HWND, LPARAM);
bool LateIniFunc(HWND, WPARAM);
// �R���{�{�b�N�X�̑I�����ڂ�NuberDelay�ɕϊ�
bool CalcLateNumberOfloops(HWND, int*, int, int, int, long, long);
bool SendLate2Combo(HWND, WPARAM);
//�R���{�{�b�N�X�Ō��ݑI������Ă��鍀�ڂ̃C���f�b�N�X���擾
bool GetNowComboStr(HWND);
// �G�f�B�b�g�{�b�N�X�������I�ɉ��܂ŃX�N���[������
bool ScrollToBottom(HWND);
// ����ꂽ���ԍ���csv�t�@�C���ɏ������ނ��߂̊֐�
bool WriteToCSV(HWND, vector<string>& MarginTime, const string& filename);
bool GetMarginTime(std::chrono::system_clock::duration);
bool ClearMarginTime(HWND, vector<string>& MarginTime);
bool SendCountButtonClicked(HWND, int&);

bool GetEditBoxTextFunc(HWND);

#define ID_STATIC1 2001
#define ID_STATIC2 2002
#define ID_STATIC_NUMBERDELAY1 2003
#define ID_STATIC_NUMBERDELAY2 2004
#define ID_STATIC_COUNTBUTTONCLICKED 2005
#define ID_STATIC_COUNTBUTTONCLICKED_2 2006
#define ID_STATIC_LATEDATACOMBO 2007
#define ID_STATIC_LATESETTING         2008
#define ID_STATIC_BPM  2009
#define ID_STATIC_BPMNUM 2010
#define ID_STATIC_LAB 2011
#define ID_STATIC_NORMALORIRREG 2012
#define ID_STATIC_COMBO_MS  2013
#define ID_STATIC_TRIALS 2014
#define ID_STATIC_TRIALS_NUM 2015
#define ID_STATIC_OUTPUTFILE 2016
#define ID_STATIC_FILENAME 2017
#define ID_STATIC_OLD 2018
#define ID_STATIC_NAME 2019

// �R���{�{�b�N�X
#define ID_LATEINI  3001
#define ID_LATESETTING 3002

// �v�b�V���{�^��
#define ID_BUTTON_CSV 4001

// �O���[�v�{�b�N�X
#define ID_GROUP_LATEDATA 5001
#define ID_GROUP_SETTING 5002
#define ID_GROUP_SUBJECTINFO 5003

// �G�f�B�b�g�{�b�N�X
#define ID_EDIT_OLD 6001
#define ID_EDIT_NAME 6002
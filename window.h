#pragma once
// プロトタイプ宣言

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
bool OnPaint(HWND, HDC);
const char* getMilliTime(std::chrono::system_clock::time_point);
bool SendMarginTimeToEdit(std::chrono::system_clock::duration);

bool ReadIniFile(HWND, LPARAM);
bool LateIniFunc(HWND, WPARAM);
// コンボボックスの選択項目をNuberDelayに変換
bool CalcLateNumberOfloops(HWND, int*, int, int, int, long, long);
bool SendLate2Combo(HWND, WPARAM);
//コンボボックスで現在選択されている項目のインデックスを取得
int GetNowComboStr(HWND, int);
// エディットボックスを自動的に下までスクロールする
bool ScrollToBottom(HWND);
// 得られた時間差をcsvファイルに書き込むための関数
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
#define ID_STATIC_LATEDATACOMBO_2 2020
#define ID_STATIC_LATEDATATIMING 2021
#define ID_STATIC_LATESETTING_2 2022
#define ID_STATIC_COMBO_MS_2 2023
#define ID_LATESETTING_2 2024

// コンボボックス
#define ID_LATEINI  3001
#define ID_LATESETTING 3002
#define ID_LATEINI_2 3003

// プッシュボタン
#define ID_BUTTON_CSV 4001

// グループボックス
#define ID_GROUP_LATEDATA 5001
#define ID_GROUP_SETTING 5002
#define ID_GROUP_SUBJECTINFO 5003
#define ID_GROUP_LATEDATA_2 5004

// エディットボックス
#define ID_EDIT_OLD 6001
#define ID_EDIT_NAME 6002
#define ID_EDIT_LATEDATA_TIMING 6003
#define ID_EDIT_FILEPATH 6004
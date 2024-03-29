#include<Windows.h>
#include<tchar.h>
#include<sstream>
#include<fstream>
#include"resource.h"
#include"window.h"
#include"main.h"

using namespace std;
// RECT構造体の定義
RECT rButtonClickedTime = {30, 70, 530, 430};
RECT rGroupIni = {rButtonClickedTime.right + 15, rButtonClickedTime.top, 725, 260};
RECT rGroupSetting = {rGroupIni.right + 5, rGroupIni.top, rGroupIni.right + 5 + 190, 260};
RECT rGroupIni2 = { rGroupSetting.right + 5, rGroupSetting.top, rGroupSetting.right + 5 + 190, 260 };

HWND hStaticNowTime, hStaticTime, hEdit1, htaticNumberOfloops, hStaticNumberOfloops2;

// ソース.cppと共有する変数
int Num = 0;
long inlatency, outlatency;
int lenBuffer, NumberOfloops;

//ウィンドウの作成
bool CreateControl(HWND hWnd, WPARAM wParam, LPARAM lParam) {

	hStaticTime = CreateWindow(
		_T("STATIC"),
		_T("ボタンを押した時刻"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		rButtonClickedTime.left,rButtonClickedTime.top - 40, 200, 30,
		hWnd, (HMENU)ID_STATIC2, 
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);

	hEdit1 = CreateWindow(
		_T("EDIT"),
		_T(""),
		WS_CHILD | WS_VISIBLE | WS_VSCROLL |
		ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL| WS_BORDER,
		rButtonClickedTime.left, rButtonClickedTime.top, 
		rButtonClickedTime.right - rButtonClickedTime.left, rButtonClickedTime.bottom - rButtonClickedTime.top,
		hWnd,
		NULL,
		((LPCREATESTRUCT)(lParam))->hInstance,
		NULL
	);

	htaticNumberOfloops = CreateWindow(
		_T("STATIC"),
		_T("ループ:"),
		WS_CHILD | WS_VISIBLE | SS_RIGHT,
		rGroupSetting.left + 10, rGroupSetting.top + 145, 65, 25,
		hWnd, (HMENU)ID_STATIC_NUMBERDELAY1,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);

	hStaticNumberOfloops2 = CreateWindow(
		_T("STATIC"),
		_T("1"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		rGroupSetting.left + 100, rGroupSetting.top + 145, 50, 25, 
		hWnd, (HMENU)ID_STATIC_NUMBERDELAY2,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);

	HWND hButtonSendCSV = CreateWindow(
		_T("BUTTON"),
		_T("↓ファイルを出力"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		rGroupIni.left + 260 + 35, 375, rGroupIni2.right - rGroupIni2.left + 50, 50,
		hWnd, (HMENU)ID_BUTTON_CSV,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);
	HWND hStaticFilename = CreateWindow(
		_T("STATIC"),
		_T("場所:"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		rGroupIni.left + 15, 390, 50, 25,
		hWnd, (HMENU)ID_STATIC_FILENAME,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);
	// ファイルの場所を示すエディットボックス
	HWND hEditFilePath = CreateWindow(
		_T("EDIT"),
		_T("No file selected"),
		WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY,
		rGroupIni.left + 70, 390, 200, 25,
		hWnd,
		(HMENU)ID_EDIT_FILEPATH,
		((LPCREATESTRUCT)(lParam))->hInstance,
		NULL
	);

	HWND hCountButtonClicked_2 = CreateWindow(
		_T("STATIC"),
		_T("0"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		360, 15, 60, 50,
		hWnd, (HMENU)ID_STATIC_COUNTBUTTONCLICKED_2,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);
	
	// 各設定値の表示
	HWND hGruoplatedata = CreateWindow(
		_T("BUTTON"),
		_T("実験条件"),
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		rGroupSetting.left, rGroupSetting.top,
		rGroupSetting.right - rGroupSetting.left, rGroupSetting.bottom - rGroupSetting.top,
		hWnd, (HMENU)ID_GROUP_SETTING,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);

	HWND hBPM = CreateWindow(
		_T("STATIC"),
		_T("BPM:"),
		WS_CHILD | WS_VISIBLE | SS_RIGHT,
	    rGroupSetting.left + 10, rGroupSetting.top + 35, 65, 25,
		hWnd, (HMENU)ID_STATIC_BPM,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);
	// 被験者情報の設定
	HWND hSubject = CreateWindow(
		_T("BUTTON"),
		_T("被験者情報の入力"),
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		rGroupIni.left, rGroupIni.bottom + 10,
		rGroupIni2.right - rGroupIni.left, 90,
		hWnd, (HMENU)ID_GROUP_SUBJECTINFO,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);
	// 年齢
	HWND hStaticOld = CreateWindow(
		_T("STATIC"),
		_T("年齢:"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		rGroupIni.left + 10, rGroupIni.bottom + 45, 50, 25,
		hWnd, (HMENU)ID_STATIC_OLD,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);

	HWND hEditOld = CreateWindow(
		_T("EDIT"),
		_T(""),
		WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER | ES_NUMBER,
		rGroupIni.left + 60, rGroupIni.bottom + 45, 70, 25,
		hWnd,
		(HMENU)ID_EDIT_OLD,
		((LPCREATESTRUCT)(lParam))->hInstance,
		NULL
	);
	// 名前
		// 年齢
	HWND hStaticName = CreateWindow(
		_T("STATIC"),
		_T("名前:"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		rGroupIni.left + 150, rGroupIni.bottom + 45, 50, 25,
		hWnd, (HMENU)ID_STATIC_NAME,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);

	HWND hEditName = CreateWindow(
		_T("EDIT"),
		_T(""),
		WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER | ES_AUTOHSCROLL,
		rGroupIni.left + 200, rGroupIni.bottom + 45, rGroupIni2.right - (rGroupIni.left + 210) - 10, 25,
		hWnd,
		(HMENU)ID_EDIT_NAME,
		((LPCREATESTRUCT)(lParam))->hInstance,
		NULL
	);
	/*HWND hBPM_NUM = CreateWindow(
		_T("STATIC"),
		_T("未選択"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		rGroupSetting.left + 100, rGroupSetting.top + 35, 80, 25,
		hWnd, (HMENU)ID_STATIC_BPMNUM,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);*/

	// BPMの選択用コンボボックス
	HWND hComboBPM = CreateWindow(
		_T("COMBOBOX"),
		NULL,
		WS_CHILD | WS_VISIBLE | WS_DISABLED | CBS_DROPDOWNLIST,
		rGroupSetting.left + 100, rGroupSetting.top + 35, 70, 25,
		hWnd, (HMENU)ID_STATIC_BPMNUM,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);

	// 項目追加
	SendMessage(hComboBPM, CB_ADDSTRING, 0, (LPARAM)_T("60"));
	SendMessage(hComboBPM, CB_ADDSTRING, 0, (LPARAM)_T("70"));
	SendMessage(hComboBPM, CB_ADDSTRING, 0, (LPARAM)_T("80"));
	SendMessage(hComboBPM, CB_ADDSTRING, 0, (LPARAM)_T("90"));
	SendMessage(hComboBPM, CB_ADDSTRING, 0, (LPARAM)_T("100"));
	SendMessage(hComboBPM, CB_ADDSTRING, 0, (LPARAM)_T("110"));
	// 初期選択項目を"90"に設定
	SendMessage(hComboBPM, CB_SETCURSEL, 3, 0);


	HWND hTrials = CreateWindow(
		_T("STATIC"),
		_T("取得回数:"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		rGroupSetting.left + 10, rGroupSetting.top + 75, 80, 25,
		hWnd, (HMENU)ID_STATIC_TRIALS,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);

	//HWND hTrialsNum = CreateWindow(
	//	_T("STATIC"),
	//	_T("30"),
	//	WS_CHILD | WS_VISIBLE | SS_LEFT,
	//	rGroupSetting.left + 100, rGroupSetting.top + 75, 30, 25,
	//	hWnd, (HMENU)ID_STATIC_TRIALS_NUM,
	//	((LPCREATESTRUCT)(lParam))->hInstance, NULL
	//);

	// 取得回数の選択用コンボボックス
	HWND hComboTraialsNum = CreateWindow(
		_T("COMBOBOX"),
		NULL,
		WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
		rGroupSetting.left + 100, rGroupSetting.top + 75, 70, 25,
		hWnd, (HMENU)ID_STATIC_TRIALS_NUM,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);
	// 項目追加
	SendMessage(hComboTraialsNum, CB_ADDSTRING, 0, (LPARAM)_T("20"));
	SendMessage(hComboTraialsNum, CB_ADDSTRING, 0, (LPARAM)_T("30"));
	SendMessage(hComboTraialsNum, CB_ADDSTRING, 0, (LPARAM)_T("40"));
	// 初期選択項目を"40"に設定
	SendMessage(hComboTraialsNum, CB_SETCURSEL, 2, 0);
	
	HWND hBPM_LAB = CreateWindow(
		_T("STATIC"),
		_T("実験方法:"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		rGroupSetting.left + 10, rGroupSetting.top + 110, 80, 25,
		hWnd, (HMENU)ID_STATIC_LAB,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);

	//HWND hBPM_IRREG = CreateWindow(
	//	_T("STATIC"),
	//	_T("通常"),
	//	WS_CHILD | WS_VISIBLE | SS_LEFT,
	//	rGroupSetting.left + 100, rGroupSetting.top + 110, 60, 25,
	//	hWnd, (HMENU)ID_STATIC_NORMALORIRREG,
	//	((LPCREATESTRUCT)(lParam))->hInstance, NULL
	//);
	// 実験方法の選択用コンボボックス
	HWND hComboLAB = CreateWindow(
		_T("COMBOBOX"),
		NULL,
		WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
		rGroupSetting.left + 100, rGroupSetting.top + 110, 70, 25,
		hWnd, (HMENU)ID_STATIC_NORMALORIRREG,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);
	// 項目追加
	SendMessage(hComboLAB, CB_ADDSTRING, 0, (LPARAM)_T("通常"));
	SendMessage(hComboLAB, CB_ADDSTRING, 0, (LPARAM)_T("変則"));
	// 初期選択項目を"通常"に設定
	SendMessage(hComboLAB, CB_SETCURSEL, 0, 0);


	return true;
}

//コンボボックス、 iniファイル読み込み
bool ReadIniFile(HWND hWnd, LPARAM lParam) {
	char Path[MAX_PATH + 1];
	char settingpath[MAX_PATH + 1];
	char lateName[256];

	lateName[0] = '\0';
	settingpath[0] = '\0';

	if (GetModuleFileName(NULL, Path, MAX_PATH) != 0) {
		char drive[MAX_PATH + 1], dir[MAX_PATH + 1], fname[MAX_PATH + 1], ext[MAX_PATH + 1];
		// パス名を分解
		_splitpath(Path, drive, dir, fname, ext);
		_stprintf_s(settingpath, MAX_PATH + 1, _T("%s%ssetting.ini"), drive, dir);
	}
	// iniファイルの読み込み
	GetPrivateProfileString(_T("setting"), _T("latedataname"), _T("Error"), lateName, sizeof(lateName), settingpath);

	std::stringstream ss, tt;
	ss << lateName;
	string s, t;

	
	// 遅延時間設定グループボックスの作成
	HWND hGruoplatedata = CreateWindow(
		_T("BUTTON"),
		_T("遅延時間の設定"),
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		rGroupIni.left, rGroupIni.top, rGroupIni.right - rGroupIni.left, rGroupIni.bottom - rGroupIni.top,
		hWnd, (HMENU)ID_GROUP_LATEDATA,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);
	//-------------------------------------------------------------------------
	//*遅延時間設定用グループボックスの作成1（変則var)*/
	HWND hGruoplatedata_2 = CreateWindow(
		_T("BUTTON"),
		_T("遅延時間の設定（変則)"),
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		rGroupIni2.left, rGroupIni2.top, rGroupIni2.right - rGroupIni2.left, rGroupIni2.bottom - rGroupIni2.top,
		hWnd, (HMENU)ID_GROUP_LATEDATA_2,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);

	HWND hStaticlatedataTiming = CreateWindow(
		_T("STATIC"),
		_T("タイミング:"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		rGroupIni2.left + 10, rGroupIni2.top + 35, 100, 25,
		hWnd, (HMENU)ID_STATIC_LATEDATATIMING,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);
	HWND hEditLateDataTiming = CreateWindow(
		_T("EDIT"),
		_T("2"),
		WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER | ES_NUMBER |  WS_DISABLED,
		rGroupIni2.left + 115, rGroupIni2.top + 35, 40, 25,
		hWnd,
		(HMENU)ID_EDIT_LATEDATA_TIMING,
		((LPCREATESTRUCT)(lParam))->hInstance,
		NULL
	);

	HWND hStaticlatedataCombo_2 = CreateWindow(
		_T("STATIC"),
		_T("基本:"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		rGroupIni2.left + 10, rGroupIni2.top + 85, 50, 25,
		hWnd, (HMENU)ID_STATIC_LATEDATACOMBO_2,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);
	HWND lateDataCombo_2 = CreateWindow(
		_T("COMBOBOX"), NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_DISABLED,
		rGroupIni2.left + 65, rGroupIni2.top + 85, 85, 50,
		hWnd, (HMENU)ID_LATEINI_2,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);

	HWND hStaticlatesetting_2 = CreateWindow(
		_T("STATIC"),
		_T("詳細: "),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		rGroupIni2.left + 10, rGroupIni2.top + 135, 50, 25,
		hWnd, (HMENU)ID_STATIC_LATESETTING_2,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);
	HWND hStaticlatedataCombo_MS_2 = CreateWindow(
		_T("STATIC"),
		_T("[ms]"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		rGroupIni2.right - 37, rGroupIni2.top + 141, 35, 25,
		hWnd, (HMENU)ID_STATIC_COMBO_MS_2,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);

	CreateWindow(
		_T("COMBOBOX"), NULL,
		WS_CHILD | WS_VISIBLE | CBS_SORT | CBS_DROPDOWN | ES_NUMBER | WS_DISABLED,
		rGroupIni2.left + 65, rGroupIni2.top + 135, 85, 50,
		hWnd, (HMENU)ID_LATESETTING_2,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL);

	/*-----------------------------------------------------------------------------------*/
	HWND hStaticlatedataCombo = CreateWindow(
		_T("STATIC"),
		_T("基本:"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		rGroupIni.left + 10, rGroupIni.top + 30, 100, 25,
		hWnd, (HMENU)ID_STATIC_LATEDATACOMBO,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);
	
	HWND lateDataCombo = CreateWindow(
		_T("COMBOBOX"), NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
		rGroupIni.left + 10, rGroupIni.top + 55, 130, 50,
		hWnd, (HMENU)ID_LATEINI,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);

	int i = 0;
	while (getline(ss, s, ',')){
		char* cstr = new char[s.size() + 1];
		char_traits<char>::copy(cstr, s.c_str(), s.size() + 1);
		// コンボボックスに文字列を挿入
		SendMessage(GetDlgItem(hWnd, ID_LATEINI), CB_INSERTSTRING, i, (LPARAM)cstr);
		SendMessage(GetDlgItem(hWnd, ID_LATEINI_2), CB_INSERTSTRING, i, (LPARAM)cstr);
		i++;
	}
	// コンボボックスに先頭の要素をセット
	SendMessage(GetDlgItem(hWnd, ID_LATEINI), CB_SETCURSEL, 0, 0);
	SendMessage(GetDlgItem(hWnd, ID_LATEINI_2), CB_SETCURSEL, 0, 0);

	// 2個目のコンボボックス
	HWND hStaticlatesetting = CreateWindow(
		_T("STATIC"),
		_T("詳細: "),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		rGroupIni.left + 10, rGroupIni.top + 115, 100, 25,
		hWnd, (HMENU)ID_STATIC_LATESETTING,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);
	HWND hStaticlatedataCombo_MS = CreateWindow(
		_T("STATIC"),
		_T("[ms]"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		rGroupIni.right - 37, rGroupIni.top + 146, 35, 25,
		hWnd, (HMENU)ID_STATIC_COMBO_MS,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);
	CreateWindow(
		_T("COMBOBOX"), NULL,
		WS_CHILD | WS_VISIBLE | CBS_SORT | CBS_DROPDOWN | ES_NUMBER,
		rGroupIni.left + 10, rGroupIni.top + 140, 130, 50,
		hWnd, (HMENU)ID_LATESETTING,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL);

	// コンボボックスの一覧内の文字列の長さを取得
	int intTxtLen = SendMessage(GetDlgItem(hWnd, (int)ID_LATEINI), CB_GETLBTEXTLEN, 0, 0);
	if (intTxtLen != CB_ERR){
		char* pszBuf = new char[intTxtLen + 1];
		// コンボボックスの一覧から選択した文字列を取得
		if (SendMessage(GetDlgItem(hWnd, (int)ID_LATEINI), CB_GETLBTEXT, 0, (LPARAM)pszBuf) != CB_ERR) {
			char   latedata[256];
			GetPrivateProfileString(pszBuf, _T("data"), _T("Error"), latedata, sizeof(latedata), settingpath);
			tt.str("");
			tt.clear(stringstream::goodbit);
			tt << latedata;
			i = 0;
			while (getline(tt, t, ',')) {
				char* tstr = new char[t.size() + 1];
				std::char_traits<char>::copy(tstr, t.c_str(), t.size() + 1);
				SendMessage(GetDlgItem(hWnd, (int)ID_LATESETTING), CB_INSERTSTRING, i, (LPARAM)tstr);
				SendMessage(GetDlgItem(hWnd, (int)ID_LATESETTING_2), CB_INSERTSTRING, i, (LPARAM)tstr);
				i++;
			}
		}

		delete[] pszBuf;
	}
	SendMessage(GetDlgItem(hWnd, (int)ID_LATESETTING), CB_SETCURSEL, 0, 0);
	SendMessage(GetDlgItem(hWnd, (int)ID_LATESETTING_2), CB_SETCURSEL, 0, 0);

	//コンボボックスで現在選択されている項目のインデックスを取得
	GetNowComboStr(hWnd, ID_LATESETTING);
	// NumberOfloopsを計算
	CalcLateNumberOfloops(hWnd, &NumberOfloops, Num, lenBuffer, FS, inlatency, outlatency);

	return true;
}
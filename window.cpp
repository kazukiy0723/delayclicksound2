#include<Windows.h>
#include<tchar.h>
#include<sstream>
#include<fstream>
#include"resource.h"
#include"window.h"
#include"main.h"

using namespace std;
// RECT�\���̂̒�`
RECT rButtonClickedTime = {30, 70, 530, 430};
RECT rGroupIni = {rButtonClickedTime.right + 15, rButtonClickedTime.top, 725, 260};
RECT rGroupSetting = {rGroupIni.right + 5, rGroupIni.top, rGroupIni.right + 5 + 190, 260};

HWND hStaticNowTime, hStaticTime, hEdit1, htaticNumberOfloops, hStaticNumberOfloops2;

// �\�[�X.cpp�Ƌ��L����ϐ�
int Num = 0;
long inlatency, outlatency;
int lenBuffer, NumberOfloops;

//�E�B���h�E�̍쐬
bool CreateControl(HWND hWnd, WPARAM wParam, LPARAM lParam) {

	hStaticTime = CreateWindow(
		_T("STATIC"),
		_T("�{�^��������������"),
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
		_T("���[�v:"),
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
		_T("���ʂ��o��"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		rGroupIni.left, 375, rGroupIni.right - rGroupIni.left, 50,
		hWnd, (HMENU)ID_BUTTON_CSV,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);
	HWND hStaticFilename = CreateWindow(
		_T("STATIC"),
		_T(">>> No File"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		rGroupSetting.left, 390, 200, 25,
		hWnd, (HMENU)ID_STATIC_FILENAME,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);
	
	HWND hCountButtonClicked_2 = CreateWindow(
		_T("STATIC"),
		_T("0"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		360, 15, 60, 50,
		hWnd, (HMENU)ID_STATIC_COUNTBUTTONCLICKED_2,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);
	
	// �e�ݒ�l�̕\��
	HWND hGruoplatedata = CreateWindow(
		_T("BUTTON"),
		_T("��������"),
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
	// �팱�ҏ��̐ݒ�
	HWND hSubject = CreateWindow(
		_T("BUTTON"),
		_T("�팱�ҏ��̓���"),
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		rGroupIni.left, rGroupIni.bottom + 10,
		rGroupSetting.right - rGroupIni.left, 90,
		hWnd, (HMENU)ID_GROUP_SUBJECTINFO,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);
	// �N��
	HWND hStaticOld = CreateWindow(
		_T("STATIC"),
		_T("�N��:"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		rGroupIni.left + 10, rGroupIni.bottom + 45, 50, 25,
		hWnd, (HMENU)ID_STATIC_OLD,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);

	HWND hEditOld = CreateWindow(
		_T("EDIT"),
		_T(""),
		WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
		rGroupIni.left + 60, rGroupIni.bottom + 45, 70, 25,
		hWnd,
		(HMENU)ID_EDIT_OLD,
		((LPCREATESTRUCT)(lParam))->hInstance,
		NULL
	);
	// ���O
		// �N��
	HWND hStaticName = CreateWindow(
		_T("STATIC"),
		_T("���O:"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		rGroupIni.left + 140, rGroupIni.bottom + 45, 50, 25,
		hWnd, (HMENU)ID_STATIC_NAME,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);

	HWND hEditName = CreateWindow(
		_T("EDIT"),
		_T(""),
		WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER | WS_HSCROLL | ES_AUTOHSCROLL,
		rGroupIni.left + 190, rGroupIni.bottom + 45, rGroupSetting.right - (rGroupIni.left + 190) - 10, 42,
		hWnd,
		(HMENU)ID_EDIT_NAME,
		((LPCREATESTRUCT)(lParam))->hInstance,
		NULL
	);
	/*HWND hBPM_NUM = CreateWindow(
		_T("STATIC"),
		_T("���I��"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		rGroupSetting.left + 100, rGroupSetting.top + 35, 80, 25,
		hWnd, (HMENU)ID_STATIC_BPMNUM,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);*/

	// BPM�̑I��p�R���{�{�b�N�X
	HWND hComboBPM = CreateWindow(
		_T("COMBOBOX"),
		NULL,
		WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
		rGroupSetting.left + 100, rGroupSetting.top + 35, 70, 25,
		hWnd, (HMENU)ID_STATIC_BPMNUM,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);

	// ���ڒǉ�
	SendMessage(hComboBPM, CB_ADDSTRING, 0, (LPARAM)_T("60"));
	SendMessage(hComboBPM, CB_ADDSTRING, 0, (LPARAM)_T("70"));
	SendMessage(hComboBPM, CB_ADDSTRING, 0, (LPARAM)_T("80"));
	SendMessage(hComboBPM, CB_ADDSTRING, 0, (LPARAM)_T("90"));
	SendMessage(hComboBPM, CB_ADDSTRING, 0, (LPARAM)_T("100"));
	SendMessage(hComboBPM, CB_ADDSTRING, 0, (LPARAM)_T("110"));
	// �����I�����ڂ�"90"�ɐݒ�
	SendMessage(hComboBPM, CB_SETCURSEL, 3, 0);


	HWND hTrials = CreateWindow(
		_T("STATIC"),
		_T("�擾��:"),
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

	// �擾�񐔂̑I��p�R���{�{�b�N�X
	HWND hComboTraialsNum = CreateWindow(
		_T("COMBOBOX"),
		NULL,
		WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
		rGroupSetting.left + 100, rGroupSetting.top + 75, 70, 25,
		hWnd, (HMENU)ID_STATIC_TRIALS_NUM,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);
	// ���ڒǉ�
	SendMessage(hComboTraialsNum, CB_ADDSTRING, 0, (LPARAM)_T("20"));
	SendMessage(hComboTraialsNum, CB_ADDSTRING, 0, (LPARAM)_T("30"));
	SendMessage(hComboTraialsNum, CB_ADDSTRING, 0, (LPARAM)_T("40"));
	// �����I�����ڂ�"30"�ɐݒ�
	SendMessage(hComboTraialsNum, CB_SETCURSEL, 1, 0);
	
	HWND hBPM_LAB = CreateWindow(
		_T("STATIC"),
		_T("�������@:"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		rGroupSetting.left + 10, rGroupSetting.top + 110, 80, 25,
		hWnd, (HMENU)ID_STATIC_LAB,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);

	//HWND hBPM_IRREG = CreateWindow(
	//	_T("STATIC"),
	//	_T("�ʏ�"),
	//	WS_CHILD | WS_VISIBLE | SS_LEFT,
	//	rGroupSetting.left + 100, rGroupSetting.top + 110, 60, 25,
	//	hWnd, (HMENU)ID_STATIC_NORMALORIRREG,
	//	((LPCREATESTRUCT)(lParam))->hInstance, NULL
	//);
	// �������@�̑I��p�R���{�{�b�N�X
	HWND hComboLAB = CreateWindow(
		_T("COMBOBOX"),
		NULL,
		WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
		rGroupSetting.left + 100, rGroupSetting.top + 110, 70, 25,
		hWnd, (HMENU)ID_STATIC_NORMALORIRREG,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);
	// ���ڒǉ�
	SendMessage(hComboLAB, CB_ADDSTRING, 0, (LPARAM)_T("�ʏ�"));
	SendMessage(hComboLAB, CB_ADDSTRING, 0, (LPARAM)_T("�ϑ�"));
	// �����I�����ڂ�"�ʏ�"�ɐݒ�
	SendMessage(hComboLAB, CB_SETCURSEL, 0, 0);


	return true;
}

//�R���{�{�b�N�X�A ini�t�@�C���ǂݍ���
bool ReadIniFile(HWND hWnd, LPARAM lParam) {
	char Path[MAX_PATH + 1];
	char settingpath[MAX_PATH + 1];
	char lateName[256];

	lateName[0] = '\0';
	settingpath[0] = '\0';

	if (GetModuleFileName(NULL, Path, MAX_PATH) != 0) {
		char drive[MAX_PATH + 1], dir[MAX_PATH + 1], fname[MAX_PATH + 1], ext[MAX_PATH + 1];
		// �p�X���𕪉�
		_splitpath(Path, drive, dir, fname, ext);
		_stprintf_s(settingpath, MAX_PATH + 1, _T("%s%ssetting.ini"), drive, dir);
	}
	// ini�t�@�C���̓ǂݍ���
	GetPrivateProfileString(_T("setting"), _T("latedataname"), _T("Error"), lateName, sizeof(lateName), settingpath);

	std::stringstream ss, tt;
	ss << lateName;
	string s, t;

	
	// �x�����Ԑݒ�O���[�v�{�b�N�X�̍쐬
	HWND hGruoplatedata = CreateWindow(
		_T("BUTTON"),
		_T("�x�����Ԃ̐ݒ�"),
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		rGroupIni.left, rGroupIni.top, rGroupIni.right - rGroupIni.left, rGroupIni.bottom - rGroupIni.top,
		hWnd, (HMENU)ID_GROUP_LATEDATA,
		((LPCREATESTRUCT)(lParam))->hInstance, NULL
	);

	HWND hStaticlatedataCombo = CreateWindow(
		_T("STATIC"),
		_T("��{:"),
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
		// �R���{�{�b�N�X�ɕ������}��
		SendMessage(GetDlgItem(hWnd, ID_LATEINI), CB_INSERTSTRING, i, (LPARAM)cstr);
		i++;
	}
	// �R���{�{�b�N�X�ɐ擪�̗v�f���Z�b�g
	SendMessage(GetDlgItem(hWnd, ID_LATEINI), CB_SETCURSEL, 0, 0);

	// 2�ڂ̃R���{�{�b�N�X
	HWND hStaticlatesetting = CreateWindow(
		_T("STATIC"),
		_T("�ڍ�: "),
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

	// �R���{�{�b�N�X�̈ꗗ���̕�����̒������擾
	int intTxtLen = SendMessage(GetDlgItem(hWnd, (int)ID_LATEINI), CB_GETLBTEXTLEN, 0, 0);
	if (intTxtLen != CB_ERR){
		char* pszBuf = new char[intTxtLen + 1];
		// �R���{�{�b�N�X�̈ꗗ����I��������������擾
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
				i++;
			}
		}

		delete[] pszBuf;
	}
	SendMessage(GetDlgItem(hWnd, (int)ID_LATESETTING), CB_SETCURSEL, 0, 0);

	// NumberOfloops�̏����l���Z�b�g
	// 
	//�R���{�{�b�N�X�Ō��ݑI������Ă��鍀�ڂ̃C���f�b�N�X���擾
	GetNowComboStr(hWnd);
	// NumberOfloops���v�Z
	CalcLateNumberOfloops(hWnd, &NumberOfloops, Num, lenBuffer, FS, inlatency, outlatency);

	return true;
}
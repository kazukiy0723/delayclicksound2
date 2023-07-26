//////
//Delay Click Sound by ASIO Ver. 2 (�I�[�o�[���b�v�Ȃ��A�Đ����J�n�_��������A�`���^�����O�l���A�����x������(�ʃ`�����l���Đ�))
//////
#include<windows.h>
#include<tchar.h>
#include<stdio.h>
#include<Xinput.h>
#include"resource.h"
#include"ginclude.h"
#include"asio.h"
#include"asiodrivers.h"

// �ǉ��t�@�C��
#include<chrono>
#include<iostream>
#include<sstream>
#include<fstream>
#include<iomanip>
#include<time.h>
#include<string>
#include<ctime>
#include<vector>
#include"window.h"
#include"main.h"
#include"file.h"
#include"dialogbox.h"

// Visual�X�^�C���L����
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib,"winmm.lib")

using namespace std;

LRESULT CALLBACK	windowfunc(HWND, UINT, WPARAM, LPARAM);
void				bufferswitch(long, ASIOBool);
ASIOTime* bufferswitchtimeinfo(ASIOTime*, long, ASIOBool);
void				sampleratedidchange(ASIOSampleRate);
long				asiomessage(long, long, void*, double*);

extern AsioDrivers* asioDrivers;
const TCHAR			szWinName[] = _T("delayclicksound2");
TCHAR				strASIOInfo[LENSTR], strWaveFileInfo[LENSTR];
ASIODriverInfo		DriverInfo;
ASIOBufferInfo		BufInfo[MAXNUMINCHS + MAXNUMOUTCHS];
ASIOChannelInfo		ChannelInfo[MAXNUMINCHS + MAXNUMOUTCHS];
ASIOCallbacks		CallBacks;
int					numInChannels, numOutChannels, idDevice, idChannelClick, idChannelSound;
bool				SupportASIOOutputReady;
HMMIO				hMmio;
void* tmpWaveData, * WaveData;
int					lenWaveData, idxWaveData, BitsPerSample;
bool				isPlaying, isLoadedDriver, isOpenedDevice, isCreatedBuffer;
ASIOSampleType		SampleType;
extern long inlatency, outlatency;
extern int lenBuffer;

// �ǉ��ϐ�
extern HWND hStaticNowTime, hStaticTime, hEdit1, htaticNumberOfloops, hStaticNumberOfloops2;
HFONT hFont1, hFont2, hFont3;
RECT recthEdit1 = { 198, 98, 702, 402 };
int call_count, temp_count;
bool wavePlayed = false;
extern int NumberOfloops;
// �������Ԍv���p
std::chrono::system_clock::time_point start, before_start;
extern int Num;
//�@���ԍ���ۑ����邽�߂̃x�N�^
vector<string> MarginTime;
// �{�^���̉����񐔂�ێ�����ϐ�
int CountButtonClicked;
// �e�E�B���h�E�̃n���h����ێ�
HWND hParentWindow;
// ���ʂ̏o�͐�̃p�X��ێ�����ϐ�
string CSVFILENAME;
// �������@�̑I��
bool LABNormal;
int TempNumberOfLoops;
// ���s�񐔂�ێ�����ϐ�
int TempNumberOfTrials;
// �t�@�C����
static char FileNameCSV[MAX_PATH];
// �^�C���X�^���v
string TimeStampButtonClicked = _T("Nothing");

// dialogbox.cpp�Ƌ��L
extern int DelayTiming, DelayTime_ms;
int DelayTimeUnique;

//WinMain�֐�
int WINAPI WinMain(_In_ HINSTANCE hThisInst, _In_opt_ HINSTANCE hPrevInst, _In_ LPSTR lpszArgs, _In_ int nWinMode)
{
	HWND			hwnd;
	MSG				msg;
	WNDCLASSEX		wcl;
	HACCEL			haccel;

	//�E�B���h�E�N���X�̒�`
	wcl.cbSize = sizeof(WNDCLASSEX);				//WNDCLASSEX�\���̂̃T�C�Y
	wcl.style = 0;									//�E�B���h�E�N���X�X�^�C��
	wcl.lpfnWndProc = windowfunc;					//�E�B���h�E�֐�
	wcl.cbClsExtra = 0;								//�E�B���h�E�N���X�̃G�L�X�g��
	wcl.cbWndExtra = 0;								//�E�B���h�E�C���X�^���X�̃G�L�X�g��
	wcl.hInstance = hThisInst;						//���̃v���O�����̃C���X�^���X�ւ̃n���h��
	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//�A�C�R���ւ̃n���h��
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);		//�J�[�\���ւ̃n���h��
	wcl.hbrBackground = (HBRUSH)COLOR_WINDOW;		//�w�i�u���V�ւ̃n���h��
	wcl.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);	//���j���[
	wcl.lpszClassName = szWinName;					//�E�B���h�E�N���X��
	wcl.hIconSm = LoadIcon(NULL, IDI_WINLOGO);		//�X���[���A�C�R���ւ̃n���h��

	//�E�B���h�E�N���X�̓o�^
	if (!RegisterClassEx(&wcl)) {
		return(0);
	}

	//�E�B���h�E�̐���
	hwnd = CreateWindow(
		szWinName,				//�E�B���h�E�N���X��
		szWinName,				//�E�B���h�E��
		WS_OVERLAPPEDWINDOW,	//�E�B���h�E�X�^�C��
		CW_USEDEFAULT,			//x���W
		CW_USEDEFAULT,			//y���W
		CW_USEDEFAULT,			//��
		CW_USEDEFAULT,			//����
		HWND_DESKTOP,			//�e�E�B���h�E�ւ̃n���h��
		NULL,					//���j���[�ւ̃n���h��
		hThisInst,				//���̃v���O�����̃C���X�^���X�ւ̃n���h��	
		NULL					//�ǉ�����
	);

	// �e�E�B���h�E�n���h���̋L��
	hParentWindow = hwnd;

	//�E�B���h�E�̕\��
	ShowWindow(hwnd, nWinMode);
	UpdateWindow(hwnd);

	//�L�[�{�[�h�A�N�Z�����[�^�̃��[�h
	haccel = LoadAccelerators(hThisInst, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	//���b�Z�[�W���[�v�̐���
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(hwnd, haccel, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return((int)msg.wParam);
}

//�E�B���h�E�֐�
LRESULT CALLBACK windowfunc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	ASIOError		asioresult;
	ASIOSampleRate	rate;
	char			devnames[MAXNUMDEVS][LENDEVNAME], * tmpnames[MAXNUMDEVS];
	TCHAR			str[LENSTR];
	long			numinchs, numoutchs, minlenbuffer, maxlenbuffer, preflenbuffer, granularity, tmplong;
	int				numdevs, idev, ich, ibuf, isample;
	bool			boolresult;
	double			tmpdouble;
	TCHAR			wavefilename[] = WAVEFILENAME;
	MMCKINFO		mmckinfoparent, mmckinfosubchunk;
	WAVEFORMATEX	wf;

	switch (message) {
	case WM_CREATE:
		//�E�B���h�E�̏�����
		// �E�B���h�E�̍쐬
		CreateControl(hwnd, wparam, lparam);
		// ���s�t�@�C���̃p�X���̎擾
		TCHAR lpFileName[MAX_PATH];
		GetModuleFileName(NULL, lpFileName, sizeof(lpFileName));
		// �E�B���h�E�̃^�C�g�������s�t�@�C���̃p�X���ɕύX
		SetWindowText(hwnd, lpFileName);
		//�p�����[�^�̏�����
		idDevice = -1;
		hMmio = NULL;
		tmpWaveData = NULL;
		WaveData = NULL;
		isPlaying = false;
		isLoadedDriver = false;
		isOpenedDevice = false;
		isCreatedBuffer = false;
		idxWaveData = 0;
		// �ǉ�
		call_count = 1;
		temp_count = 0;
		NumberOfloops = 1;
		TempNumberOfLoops = 1;
		CountButtonClicked = 0;
		LABNormal = true;
		TempNumberOfTrials = 35;
		//ASIO�f�o�C�X�p�������̈�̊m��
		if (asioDrivers == NULL) {
			asioDrivers = new AsioDrivers();
		}

		//ASIO�f�o�C�X���̎擾
		for (idev = 0; idev < MAXNUMDEVS; idev++) {
			tmpnames[idev] = devnames[idev];
		}
		numdevs = asioDrivers->getDriverNames(tmpnames, MAXNUMDEVS);
		if (numdevs == 0) {
			MessageBox(NULL, _T("asioDrivers->getDriverNames"), _T("Error"), MB_ICONSTOP | MB_OK);
		}

		//ASIO�f�o�C�X�̃h���C�o�̃��[�h
		boolresult = asioDrivers->loadDriver(devnames[IDDEV]);
		if (boolresult == false) {
			isLoadedDriver = false;
			_stprintf_s(str, LENSTR, _T("asioDrivers->loadDriver\nDevice Name: %s\n"), devnames[IDDEV]);
			MessageBox(NULL, str, _T("Error"), MB_ICONSTOP | MB_OK);
		}
		else {
			isLoadedDriver = true;
		}

		//ASIO�f�o�C�X�̃I�[�v��
		memset(&DriverInfo, 0, sizeof(ASIODriverInfo));
		asioresult = ASIOInit(&DriverInfo);
		if (asioresult != ASE_OK) {
			isOpenedDevice = false;
			_stprintf_s(str, LENSTR, _T("ASIOInit\nDevice Name: %s\n"), devnames[IDDEV]);
			MessageBox(NULL, str, _T("Error"), MB_ICONSTOP | MB_OK);

			//ASIO�f�o�C�X�̃h���C�o�̃A�����[�h
			if (isLoadedDriver == true) {
				asioDrivers->removeCurrentDriver();
				isLoadedDriver = false;
			}
		}
		else {
			isOpenedDevice = true;
		}

		if (isOpenedDevice == false) {
			for (idev = 0; idev < numdevs; idev++) {
				//				for (idev = numdevs-1; idev >=0; idev--) {
				if (idev != IDDEV) {
					boolresult = asioDrivers->loadDriver(devnames[idev]);
					if (boolresult == false) {
						isLoadedDriver = false;
						_stprintf_s(str, LENSTR, _T("asioDrivers->loadDriver\nDevice Name: %s\n"), devnames[idev]);
						MessageBox(NULL, str, _T("Error"), MB_ICONSTOP | MB_OK);
					}
					else {
						isLoadedDriver = true;
					}

					//ASIO�f�o�C�X�̃I�[�v��
					memset(&DriverInfo, 0, sizeof(ASIODriverInfo));
					asioresult = ASIOInit(&DriverInfo);
					if (asioresult != ASE_OK) {
						isOpenedDevice = false;
						_stprintf_s(str, LENSTR, _T("ASIOInit\nDevice Name: %s\n"), devnames[idev]);
						MessageBox(NULL, str, _T("Error"), MB_ICONSTOP | MB_OK);

						//ASIO�f�o�C�X�̃h���C�o�̃A�����[�h
						if (isLoadedDriver == true) {
							asioDrivers->removeCurrentDriver();
							isLoadedDriver = false;
						}
					}
					else {
						isOpenedDevice = true;
						break;
					}
				}
			}
		}

		//ASIO�f�o�C�X�̃T���v�����O���g���̃T�|�[�g�̊m�F
		rate = FS;
		asioresult = ASIOCanSampleRate(rate);
		if (asioresult != ASE_OK) {
			MessageBox(NULL, _T("ASIOCanSampleRate"), _T("Error"), MB_ICONSTOP | MB_OK);
		}

		//ASIO�f�o�C�X�̃T���v�����O���g���̐ݒ�
		asioresult = ASIOSetSampleRate(rate);
		if (asioresult != ASE_OK) {
			MessageBox(NULL, _T("ASIOSetSampleRate"), _T("Error"), MB_ICONSTOP | MB_OK);
		}

		//ASIO�f�o�C�X�̃`�����l�����̎擾
		asioresult = ASIOGetChannels(&numinchs, &numoutchs);
		if (asioresult != ASE_OK) {
			MessageBox(NULL, _T("ASIOGetChannels"), _T("Error"), MB_ICONSTOP | MB_OK);
		}

		//ASIO�f�o�C�X�̘^���`�����l�����̐ݒ�
		if (numinchs <= MAXNUMINCHS) {
			numInChannels = numinchs;
		}
		else {
			numInChannels = MAXNUMINCHS;
		}

		//ASIO�f�o�C�X�̍Đ��p�`�����l�����̐ݒ�
		if (numoutchs <= MAXNUMOUTCHS) {
			numOutChannels = numoutchs;
		}
		else {
			numOutChannels = MAXNUMOUTCHS;
		}
		if (CHCLICK <= numOutChannels) {
			idChannelClick = CHCLICK - 1;
		}
		else {
			idChannelClick = 0;
		}
		if (CHSOUND <= numOutChannels) {
			idChannelSound = CHSOUND - 1;
		}
		else {
			idChannelSound = 0;
		}

		//ASIO�f�o�C�X�̃o�b�t�@�T�C�Y�̎擾
		asioresult = ASIOGetBufferSize(&minlenbuffer, &maxlenbuffer, &preflenbuffer, &granularity);
		if (asioresult != ASE_OK) {
			MessageBox(NULL, _T("ASIOGetBufferSize"), _T("Error"), MB_ICONSTOP | MB_OK);
		}

		//ASIO�f�o�C�X�̃o�b�t�@�T�C�Y�̐ݒ�
		lenBuffer = preflenbuffer;

		//ASIO�f�o�C�X�̘^���p�o�b�t�@�̏�����
		for (ich = 0; ich < numInChannels; ich++) {
			memset(&(BufInfo[ich]), 0, sizeof(ASIOBufferInfo));
			BufInfo[ich].isInput = ASIOTrue;
			BufInfo[ich].channelNum = ich;
		}

		//ASIO�f�o�C�X�̍Đ��p�o�b�t�@�̏�����
		for (ich = 0; ich < numOutChannels; ich++) {
			memset(&(BufInfo[numInChannels + ich]), 0, sizeof(ASIOBufferInfo));
			BufInfo[numInChannels + ich].isInput = ASIOFalse;
			BufInfo[numInChannels + ich].channelNum = ich;
		}

		//ASIO�f�o�C�X�p�R�[���o�b�N�֐��̐ݒ�
		CallBacks.bufferSwitch = &bufferswitch;
		CallBacks.bufferSwitchTimeInfo = &bufferswitchtimeinfo;
		CallBacks.sampleRateDidChange = &sampleratedidchange;
		CallBacks.asioMessage = &asiomessage;

		//ASIO�f�o�C�X�̃o�b�t�@�̐���
		asioresult = ASIOCreateBuffers(BufInfo, numInChannels + numOutChannels, lenBuffer, &CallBacks);
		if (asioresult != ASE_OK) {
			isCreatedBuffer = false;
			MessageBox(NULL, _T("ASIOCreateBuffers"), _T("Error"), MB_ICONSTOP | MB_OK);
		}
		else {
			isCreatedBuffer = true;
		}

		//ASIO�f�o�C�X�̃`�����l�����̎擾
		for (ich = 0; ich < numInChannels + numOutChannels; ich++) {
			memset(&(ChannelInfo[ich]), 0, sizeof(ASIOChannelInfo));
			ChannelInfo[ich].isInput = BufInfo[ich].isInput;
			ChannelInfo[ich].channel = BufInfo[ich].channelNum;
			asioresult = ASIOGetChannelInfo(&(ChannelInfo[ich]));
			if (asioresult != ASE_OK) {
				MessageBox(NULL, _T("ASIOGetChannelInfo"), _T("Error"), MB_ICONSTOP | MB_OK);
			}
		}
		SampleType = ChannelInfo[0].type;
		switch (SampleType) {
		case ASIOSTInt16LSB:
			BitsPerSample = 16;
			break;
		case ASIOSTInt24LSB:
			BitsPerSample = 24;
			break;
		case ASIOSTInt32LSB:
			BitsPerSample = 32;
			break;
		default:
			break;
		}

		//ASIO�f�o�C�X�̃��C�e���V�̎擾
		asioresult = ASIOGetLatencies(&inlatency, &outlatency);
		if (asioresult != ASE_OK) {
			MessageBox(NULL, _T("ASIOGetLatencies"), _T("Error"), MB_ICONSTOP | MB_OK);
		}

		//ASIOOutputReady�֐��̃T�|�[�g�̊m�F
		asioresult = ASIOOutputReady();
		if (asioresult == ASE_OK) {
			SupportASIOOutputReady = true;
		}
		else {
			SupportASIOOutputReady = false;
		}

		//ASIO�f�o�C�X�̐ݒ���e�̊m�F
		ASIOGetSampleRate(&rate);
		_stprintf_s(strASIOInfo, LENSTR,
			_T("Device Name: %s\n")
			_T("Max. No. Input Channels: %d\n")
			_T("Max. No. Output Channels: %d\n")
			_T("Sampling Frequency: %f Hz\n")
			_T("Min. Buffer Size: %d points = %f ms\n")
			_T("Max. Buffer Size: %d points = %f ms\n")
			_T("Preferred Buffer Size: %d points = %f ms\n")
			_T("Allocated Buffer Size: %d points = %f ms\n")
			_T("Input Latency: %d points = %f ms\n")
			_T("Output Latency: %d points = %f ms\n")
			_T("Total Latency: %d points = %f ms\n")
			_T("Support \"ASIOOutputReady\": %s"),
			DriverInfo.name,
			numinchs,
			numoutchs,
			rate,
			minlenbuffer, (double)minlenbuffer * 1000.0 / rate,
			maxlenbuffer, (double)maxlenbuffer * 1000.0 / rate,
			preflenbuffer, (double)preflenbuffer * 1000.0 / rate,
			lenBuffer, (double)lenBuffer * 1000.0 / rate,
			inlatency, (double)inlatency * 1000.0 / rate,
			outlatency, (double)outlatency * 1000.0 / rate,
			inlatency + outlatency, ((double)inlatency + (double)outlatency) * 1000.0 / rate,
			SupportASIOOutputReady == true ? _T("Supported.") : _T("Not Supported."));
		MessageBox(NULL, strASIOInfo, _T("ASIO"), MB_OK);

		// �R���{�{�b�N�X�̍쐬��ini�t�@�C���̓ǂݍ���
		ReadIniFile(hwnd, lparam);

		//WAVE�t�@�C���̃I�[�v��
		if ((hMmio = mmioOpen(wavefilename, NULL, MMIO_READ)) == NULL) {
			//�t�@�C�����I�[�v���ł��Ȃ������ꍇ�̏���
			_stprintf_s(str, LENSTR, _T("�G���[: �t�@�C�����J�����Ƃ��ł��܂���\n�t�@�C����: %s"), wavefilename);
			MessageBox(hwnd, str, _T("�t�@�C�����J��"), MB_ICONSTOP | MB_OK);
			return(0);
		}

		//�t�@�C����WAVE�`�����N�ֈړ�
		memset(&mmckinfoparent, 0, sizeof(MMCKINFO));
		mmckinfoparent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		if (mmioDescend(hMmio, &mmckinfoparent, NULL, MMIO_FINDRIFF) != MMSYSERR_NOERROR) {
			//�`�����N���ړ��ł��Ȃ������ꍇ�̏���
			_stprintf_s(str, LENSTR, _T("�G���[: �f�[�^��ǂݍ��ނ��Ƃ��ł��܂���\n�t�@�C����: %s"), wavefilename);
			MessageBox(hwnd, str, _T("�t�@�C�����J��"), MB_ICONSTOP | MB_OK);
			return(0);
		}

		//�t�@�C����fmt�`�����N�ֈړ�
		memset(&mmckinfosubchunk, 0, sizeof(MMCKINFO));
		mmckinfosubchunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
		if (mmioDescend(hMmio, &mmckinfosubchunk, &mmckinfoparent, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
			//�`�����N���ړ��ł��Ȃ������ꍇ�̏���
			_stprintf_s(str, LENSTR, _T("�G���[: �f�[�^��ǂݍ��ނ��Ƃ��ł��܂���\n�t�@�C����: %s"), wavefilename);
			MessageBox(hwnd, str, _T("�t�@�C�����J��"), MB_ICONSTOP | MB_OK);
			return(0);
		}

		//�t�@�C���̃f�[�^�`���̓ǂݍ���
		memset(&wf, 0, sizeof(WAVEFORMATEX));
		if (mmioRead(hMmio, (HPSTR)&wf, mmckinfosubchunk.cksize) == -1) {
			//�t�@�C���̃f�[�^�`����ǂݍ��߂Ȃ������ꍇ�̏���
			_stprintf_s(str, LENSTR, _T("�G���[: �f�[�^��ǂݍ��ނ��Ƃ��ł��܂���\n�t�@�C����: %s"), wavefilename);
			MessageBox(hwnd, str, _T("�t�@�C�����J��"), MB_ICONSTOP | MB_OK);
			return(0);
		}

		//�t�@�C���̃f�[�^�`���̊m�F
		if (wf.wFormatTag != WAVE_FORMAT_PCM) {
			//�t�@�C���̃f�[�^�`����PCM�ł͂Ȃ������ꍇ�̏���
			_stprintf_s(str, LENSTR, _T("�G���[: PCM�`��WAVE�f�[�^�ł͂Ȃ��̂œǂݍ��ނ��Ƃ��ł��܂���\n�t�@�C����: %s"), wavefilename);
			MessageBox(hwnd, str, _T("�t�@�C�����J��"), MB_ICONSTOP | MB_OK);
			return(0);
		}

		//�t�@�C����data�`�����N�ֈړ�
		if (mmioAscend(hMmio, &mmckinfosubchunk, 0) != MMSYSERR_NOERROR) {
			//�`�����N���ړ��ł��Ȃ������ꍇ�̏���
			_stprintf_s(str, LENSTR, _T("�G���[: �f�[�^��ǂݍ��ނ��Ƃ��ł��܂���\n�t�@�C����: %s"), wavefilename);
			MessageBox(hwnd, str, _T("�t�@�C�����J��"), MB_ICONSTOP | MB_OK);
			return(0);
		}
		memset(&mmckinfosubchunk, 0, sizeof(MMCKINFO));
		mmckinfosubchunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
		if (mmioDescend(hMmio, &mmckinfosubchunk, &mmckinfoparent, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
			//�`�����N���ړ��ł��Ȃ������ꍇ�̏���
			_stprintf_s(str, LENSTR, _T("�G���[: �f�[�^��ǂݍ��ނ��Ƃ��ł��܂���\n�t�@�C����: %s"), wavefilename);
			MessageBox(hwnd, str, _T("�t�@�C�����J��"), MB_ICONSTOP | MB_OK);
			return(0);
		}
		lenWaveData = mmckinfosubchunk.cksize / wf.nBlockAlign;

		//���b�Z�[�W�{�b�N�X�ւ̃e�L�X�g�̏o��
		_stprintf_s(strWaveFileInfo, LENSTR,
			_T("File Name: %s\n")
			_T("Sampling Frequency: %d Hz\n")
			_T("Bits per Sample: %d bits\n")
			_T("Number of Channels: %d channels\n")
			_T("Length of Data: %d samples"),
			wavefilename, wf.nSamplesPerSec, wf.wBitsPerSample, wf.nChannels, lenWaveData);
		MessageBox(NULL, strWaveFileInfo, _T("Read a Wave File"), MB_OK);

		//WAVE�f�[�^�p�o�b�t�@�̐���
		tmpWaveData = (void*)malloc(mmckinfosubchunk.cksize);
		WaveData = (void*)malloc(lenWaveData * (BitsPerSample / 8));
		if (tmpWaveData == NULL || WaveData == NULL) {
			//�o�b�t�@�𐶐��ł��Ȃ������ꍇ�̏���
			MessageBox(hwnd, _T("�G���[: �������[������܂���"), _T("�t�@�C�����J��"), MB_ICONSTOP | MB_OK);
			return(0);
		}

		//WAVE�f�[�^�̓ǂݍ���
		if (mmioRead(hMmio, (HPSTR)tmpWaveData, mmckinfosubchunk.cksize) <= 0) {
			//�f�[�^��ǂݍ��߂Ȃ������ꍇ�̏���
			_stprintf_s(str, LENSTR, _T("�G���[: �f�[�^��ǂݍ��ނ��Ƃ��ł��܂���\n�t�@�C����: %s"), wavefilename);
			MessageBox(hwnd, str, _T("�t�@�C�����J��"), MB_ICONSTOP | MB_OK);
			return(0);
		}

		//WAVE�t�@�C���̃N���[�Y
		if (hMmio != NULL) {
			mmioClose(hMmio, 0);
			hMmio = NULL;
		}

		//WAVE�f�[�^�̕ϊ�
		for (isample = 0; isample < lenWaveData; isample++) {
			tmpdouble = 0.0;
			for (ich = 0; ich < wf.nChannels; ich++) {
				//WAVE�f�[�^��short int�^�ւ̕ϊ�
				switch (wf.wBitsPerSample) {
				case 8:
					tmplong = ((long)((unsigned char*)tmpWaveData)[isample * wf.nChannels + ich] - 0x80L) * 0x100L;
					break;
				case 16:
					tmplong = (long)((short int*)tmpWaveData)[isample * wf.nChannels + ich];
					break;
				}
				if (wf.nChannels > 1) {
					tmpdouble += (double)tmplong;
				}
			}
			if (wf.nChannels > 1) {
				tmplong = (long)(tmpdouble / (double)wf.nChannels);
			}

			//WAVE�f�[�^��short int�^����̕ϊ�
			switch (SampleType) {
			case ASIOSTInt16LSB:
				((char*)WaveData)[isample * 2] = (char)tmplong;
				tmplong >>= 8;
				((char*)WaveData)[isample * 2 + 1] = (char)tmplong;
				break;
			case ASIOSTInt24LSB:
				tmplong *= 0x100L;
				((char*)WaveData)[isample * 3] = (char)tmplong;
				tmplong >>= 8;
				((char*)WaveData)[isample * 3 + 1] = (char)tmplong;
				tmplong >>= 8;
				((char*)WaveData)[isample * 3 + 2] = (char)tmplong;
				break;
			case ASIOSTInt32LSB:
				tmplong *= 0x10000L;
				((char*)WaveData)[isample * 4] = (char)tmplong;
				tmplong >>= 8;
				((char*)WaveData)[isample * 4 + 1] = (char)tmplong;
				tmplong >>= 8;
				((char*)WaveData)[isample * 4 + 2] = (char)tmplong;
				tmplong >>= 8;
				((char*)WaveData)[isample * 4 + 3] = (char)tmplong;
				break;
			default:
				break;
			}
		}

		//WAVE�f�[�^�p�o�b�t�@�̉��
		if (tmpWaveData != NULL) {
			free(tmpWaveData);
			tmpWaveData = NULL;
		}

		// �t�H���g�̏����ݒ�
		OnFont(hwnd);

		//ASIO�f�o�C�X�̍Đ��p�o�b�t�@�̏�����
		for (ich = 0; ich < numOutChannels; ich++) {
			for (ibuf = 0; ibuf < NUMASIOBUFFERS; ibuf++) {
				switch (SampleType) {
				case ASIOSTInt16LSB:
					memset(BufInfo[numInChannels + ich].buffers[ibuf], 0, lenBuffer * 2);
					break;
				case ASIOSTInt24LSB:
					memset(BufInfo[numInChannels + ich].buffers[ibuf], 0, lenBuffer * 3);
					break;
				case ASIOSTInt32LSB:
					memset(BufInfo[numInChannels + ich].buffers[ibuf], 0, lenBuffer * 4);
					break;
				default:
					break;
				}
			}
		}

		//ASIO�f�o�C�X�̊J�n
		asioresult = ASIOStart();
		if (asioresult != ASE_OK) {
			MessageBox(NULL, _T("ASIOStart"), _T("Error"), MB_ICONSTOP | MB_OK);
		}
		break;

	case WM_COMMAND:
		OnCommand(hwnd, wparam);
		break;
	case WM_CLOSE:
		//�v���O�����̏I��
		if (IDYES == MessageBox(hwnd, _T("�I�����܂����H"), _T("�I���m�F"), MB_YESNO)) {
			DestroyWindow(hwnd);
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		//DrawEdge(hdc, &recthEdit1, EDGE_BUMP, BF_RECT);
		EndPaint(hwnd, &ps);
	}
	break;

	case WM_DESTROY:

		// ��n��
		DeleteObject(hFont1);
		DeleteObject(hFont2);
		DeleteObject(hFont3);
		//ASIO�f�o�C�X�̒�~
		asioresult = ASIOStop();
		if (asioresult != ASE_OK) {
			MessageBox(NULL, _T("ASIOStop"), _T("Error"), MB_ICONSTOP | MB_OK);
		}

		//ASIO�f�o�C�X�̃o�b�t�@�̉��
		asioresult = ASIODisposeBuffers();
		if (asioresult != ASE_OK) {
			MessageBox(NULL, _T("ASIODisposeBuffers"), _T("Error"), MB_ICONSTOP | MB_OK);
		}

		//ASIO�f�o�C�X�̃N���[�Y
		if (isOpenedDevice == true) {
			asioresult = ASIOExit();
			if (asioresult != ASE_OK) {
				MessageBox(NULL, _T("ASIOExit"), _T("Error"), MB_ICONSTOP | MB_OK);
			}
			isOpenedDevice = false;
		}

		//ASIO�f�o�C�X�̃h���C�o�̃A�����[�h
		if (isLoadedDriver == true) {
			asioDrivers->removeCurrentDriver();
			isLoadedDriver = false;
		}

		//ASIO�f�o�C�X�p�������̈�̉��
		if (asioDrivers != NULL) {
			delete asioDrivers;
		}

		//WAVE�f�[�^�p�o�b�t�@�̉��
		if (tmpWaveData != NULL) {
			free(tmpWaveData);
			tmpWaveData = NULL;
		}
		if (WaveData != NULL) {
			free(WaveData);
			WaveData = NULL;
		}

		//WAVE�t�@�C���̃N���[�Y
		if (hMmio != NULL) {
			mmioClose(hMmio, 0);
			hMmio = NULL;
		}

		PostQuitMessage(0);
		break;

	default:
		return(DefWindowProc(hwnd, message, wparam, lparam));
	}

	return(0);
}



//ASIO�f�o�C�X�̃o�b�t�@�����p�R�[���o�b�N�֐�
void bufferswitch(long index, ASIOBool processNow) {
	ASIOError	asioresult;
	int			tmplenbuffer, isample, position;
	double	tmpdouble;
	long		tmplong;
	

	//�^���f�[�^(�N���b�N�M��)�̕ϊ�
	for (isample = 0; isample < lenBuffer; isample++) {
		switch (SampleType) {
		case ASIOSTInt16LSB:
			tmplong = 0L;
			tmplong += ((unsigned char*)(BufInfo[idChannelClick].buffers[index]))[isample * 2 + 1];
			tmplong <<= 8;
			tmplong += ((unsigned char*)(BufInfo[idChannelClick].buffers[index]))[isample * 2];
			tmpdouble = (double)tmplong / (double)0x8000L;
			break;
		case ASIOSTInt24LSB:
			tmplong = 0L;
			tmplong += ((unsigned char*)(BufInfo[idChannelClick].buffers[index]))[isample * 3 + 2];
			tmplong <<= 8;
			tmplong += ((unsigned char*)(BufInfo[idChannelClick].buffers[index]))[isample * 3 + 1];
			tmplong <<= 8;
			tmplong += ((unsigned char*)(BufInfo[idChannelClick].buffers[index]))[isample * 3];
			tmpdouble = (double)tmplong / (double)0x800000L;
			break;
		case ASIOSTInt32LSB:
			tmplong = 0L;
			tmplong += ((unsigned char*)(BufInfo[idChannelClick].buffers[index]))[isample * 4 + 3];
			tmplong <<= 8;
			tmplong += ((unsigned char*)(BufInfo[idChannelClick].buffers[index]))[isample * 4 + 2];
			tmplong <<= 8;
			tmplong += ((unsigned char*)(BufInfo[idChannelClick].buffers[index]))[isample * 4 + 1];
			tmplong <<= 8;
			tmplong += ((unsigned char*)(BufInfo[idChannelClick].buffers[index]))[isample * 4];
			tmpdouble = (double)tmplong / (double)0x80000000UL;
			break;
		default:
			tmpdouble = 0.0;
			break;
		}

		if (tmpdouble >= THRESH || -tmpdouble >= THRESH) {
			if (isPlaying == false || idxWaveData + isample > (FS * LENCHATTERING) / 1000) {
				// �p�����[�^�̐ݒ�
				isPlaying = true;
				idxWaveData = 0;
				position = isample;
				call_count = 1;
				// �O��̃{�^�����������̎擾
				if (CountButtonClicked)  before_start = start;
				// ���ݎ����̎擾
				start = std::chrono::system_clock::now();
				// �G�f�B�b�g�{�b�N�X�Ƀ{�^���̉���������\��
				SendNowTimeToEdit(hEdit1, getMilliTime(start));
				if (CountButtonClicked) {
					// �O��̃{�^�����������Ƃ̎��ԍ����v�Z���A�\��
					auto time = start - before_start;
					SendMarginTimeToEdit(time);
					// �x�N�^�[�z��Ɍ��ʂ�ۑ�
					GetMarginTime(time);
					// �e�L�X�g�̍Ō�ɃJ�[�\�����ړ�
					ScrollToBottom(hEdit1);
				}
				if (!LABNormal) {
					/////////////////////////
					// �������@���ϑ��̏ꍇ
					// ///////////////////////
					/*�����ɃN���b�N���̍Đ��^�C�~���O�����炷���߂̃v���O�����������B*/
					if (!(CountButtonClicked % DelayTiming)) {
						// �{�^���̉����񐔂�4�̔{���̂Ƃ��A�Đ��^�C�~���O�����炷
						CalcLateNumberOfloops(hParentWindow, &DelayTimeUnique, DelayTime_ms, lenBuffer, FS, inlatency, outlatency);
						NumberOfloops = NumberOfloops + DelayTimeUnique;
					}else if(NumberOfloops != TempNumberOfLoops) {
						NumberOfloops = TempNumberOfLoops;
					}
				}
				// �����I���̂��߂̃��b�Z�[�W�{�b�N�X���o��
				if (CountButtonClicked == TempNumberOfTrials) {
					string TempNumberString = to_string(TempNumberOfTrials);
					MessageBox(NULL, TempNumberString.c_str(), _T("�I���̍��}"), MB_OK);
				}
				// �{�^���̉����񐔂��X�V
				SendCountButtonClicked(GetDlgItem(hParentWindow, ID_STATIC_COUNTBUTTONCLICKED_2), CountButtonClicked);
				break;
			}
		}
	}
	if (isample == lenBuffer) {
		position = 0;
	}

	//ASIO�f�o�C�X�̃N���b�N���Đ��p�o�b�t�@�̃N���A
	switch (SampleType) {
	case ASIOSTInt16LSB:
		memset(BufInfo[numInChannels + idChannelClick].buffers[index], 0, lenBuffer * 2);
		break;
	case ASIOSTInt24LSB:
		memset(BufInfo[numInChannels + idChannelClick].buffers[index], 0, lenBuffer * 3);
		break;
	case ASIOSTInt32LSB:
		memset(BufInfo[numInChannels + idChannelClick].buffers[index], 0, lenBuffer * 4);
		break;
	default:
		break;
	}

	if (isPlaying == true) {
		if (lenBuffer - position > lenWaveData - idxWaveData) {
			tmplenbuffer = lenWaveData - idxWaveData;
		}
		else {
			tmplenbuffer = lenBuffer - position;
		}
		// �{�^����������Ă���(NumberOfloops)��ڂ�bufferswitch�Ăяo������Wave�f�[�^���R�s�[
		if (call_count % NumberOfloops != 0) {
			// �^���f�[�^(�����M��)�̃R�s�[�i�o�̓o�b�t�@�[2�ɃR�s�[�j
			/*switch (SampleType) {
			case ASIOSTInt16LSB:
				memcpy_s(BufInfo[numInChannels + idChannelClick].buffers[index], lenBuffer * 2, BufInfo[idChannelSound].buffers[index], lenBuffer * 2);
				break;
			case ASIOSTInt24LSB:
				memcpy_s(BufInfo[numInChannels + idChannelClick].buffers[index], lenBuffer * 3, BufInfo[idChannelSound].buffers[index], lenBuffer * 3);
				break;
			case ASIOSTInt32LSB:
				memcpy_s(BufInfo[numInChannels + idChannelClick].buffers[index], lenBuffer * 4, BufInfo[idChannelSound].buffers[index], lenBuffer * 4);
				break;
			default:
				break;
			}*/
			call_count++;
		}
		else{
			//WAVE�f�[�^�̃N���b�N���Đ��p�o�b�t�@�ւ̃R�s�[�i�o�̓o�b�t�@�[2�ɃR�s�[�j
			switch (SampleType) {
			case ASIOSTInt16LSB:
				memcpy_s((void*)&(((char*)(BufInfo[numInChannels + idChannelClick].buffers[index]))[position * 2]), tmplenbuffer * 2, (void*)&(((char*)WaveData)[idxWaveData * 2]), tmplenbuffer * 2);
				//memcpy_s((void*)&(((char*)(BufInfo[numInChannels + idChannelSound].buffers[index]))[position * 2]), tmplenbuffer * 2, (void*)&(((char*)WaveData)[idxWaveData * 2]), tmplenbuffer * 2);
				break;
			case ASIOSTInt24LSB:
				memcpy_s((void*)&(((char*)(BufInfo[numInChannels + idChannelClick].buffers[index]))[position * 3]), tmplenbuffer * 3, (void*)&(((char*)WaveData)[idxWaveData * 3]), tmplenbuffer * 3);
				//memcpy_s((void*)&(((char*)(BufInfo[numInChannels + idChannelSound].buffers[index]))[position * 3]), tmplenbuffer * 3, (void*)&(((char*)WaveData)[idxWaveData * 3]), tmplenbuffer * 3);
				break;
			case ASIOSTInt32LSB:
				memcpy_s((void*)&(((char*)(BufInfo[numInChannels + idChannelClick].buffers[index]))[position * 4]), tmplenbuffer * 4, (void*)&(((char*)WaveData)[idxWaveData * 4]), tmplenbuffer * 4);
				//memcpy_s((void*)&(((char*)(BufInfo[numInChannels + idChannelSound].buffers[index]))[position * 4]), tmplenbuffer * 4, (void*)&(((char*)WaveData)[idxWaveData * 4]), tmplenbuffer * 4);
				break;
			default:
				break;
			}
			idxWaveData += tmplenbuffer;
			if (idxWaveData >= lenWaveData) {
				/////////////////////
				//�p�����[�^�̐ݒ�
				////////////////////
				isPlaying = false;
				idxWaveData = 0;
				call_count = 1;
			}
			
		}
	}
		//�^���f�[�^(�����M��)�̃R�s�[�i�o�̓o�b�t�@�[1�ɃR�s�[�j
		switch (SampleType) {
		case ASIOSTInt16LSB:
			memcpy_s(BufInfo[numInChannels + idChannelSound].buffers[index], lenBuffer * 2, BufInfo[idChannelSound].buffers[index], lenBuffer * 2);
			break;
		case ASIOSTInt24LSB:
			memcpy_s(BufInfo[numInChannels + idChannelSound].buffers[index], lenBuffer * 3, BufInfo[idChannelSound].buffers[index], lenBuffer * 3);
			break;
		case ASIOSTInt32LSB:
			memcpy_s(BufInfo[numInChannels + idChannelSound].buffers[index], lenBuffer * 4, BufInfo[idChannelSound].buffers[index], lenBuffer * 4);
			break;
		default:
			break;
		}
	

	//�Đ��p�o�b�t�@�̏��������̒ʒm
	if (SupportASIOOutputReady == true) {
		asioresult = ASIOOutputReady();
		if (asioresult != ASE_OK) {
			MessageBox(NULL, _T("ASIOOutputReady"), _T("Error"), MB_ICONSTOP | MB_OK);
		}
	}
}

// �x������[ms]��\��
bool SendMarginTimeToEdit(std::chrono::system_clock::duration time) {

	auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
	std::string string_msec = std::to_string(msec);
	std::string b = _T("���ԍ�[ms]: ");
	string_msec = b + string_msec;
	const char* str_msec = string_msec.c_str();
	SendNowTimeToEdit(hEdit1, str_msec);
	

	return true;
}

// ���ԍ���z��ɑ�����Ă������߂̊֐�
bool GetMarginTime(std::chrono::system_clock::duration time) {

	auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
	string stringTime = to_string(msec);
	MarginTime.push_back(stringTime);

	return true;
}

// ����ꂽ���ԍ���csv�t�@�C���ɏ������ނ��߂̊֐�
bool WriteToCSV(HWND hwnd, vector<string>& MarginTime, const string& filename) {
	char Temp[MAX_PATH];
	_stprintf_s(Temp, MAX_PATH,
		_T("�ȉ��̃t�@�C���ɏ������݂܂��B��낵���ł����H\r\n%s"),
		filename.c_str());

	// MargintTime����̏ꍇ
	if (MarginTime.empty()) {
		MessageBox(hwnd, _T("�������ނ��̂�����܂���B"), NULL, MB_OK);
		return true;
	}
	else if (MessageBox(hwnd, Temp, _T("�o�͐�t�@�C���̊m�F"), MB_YESNO) == IDYES){
	// �o�̓t�@�C���X�g���[���I�u�W�F�N�g���쐬
	// �t�@�C�������݂��Ȃ��ꍇ�͐V���Ƀt�@�C�����쐬�A���݂���ꍇ�̓t�@�C���̖����ɏ�������
		ofstream file(filename, ios::app);
		// �J���}��؂�ŃX�g���[���ɑ���
		for (size_t i = 0; i < MarginTime.size(); ++i) {
			file << MarginTime[i];
			if (i != MarginTime.size() - 1) {
				file << ",";
			}
		}
		file << "\n";
		file.close();

		// �x�N�^�[�̒��g�̍폜
		MarginTime.clear();
		// �������݊m�F
		MessageBox(hwnd, _T("�t�@�C���ɏ������݂܂����B"), _T("�o�͊m�F"), MB_OK);
		//SetWindowText(hwnd, filename.c_str());
		// �����񐔂̍X�V
		CountButtonClicked = 0;
		SetWindowText(GetDlgItem(hwnd, ID_STATIC_COUNTBUTTONCLICKED_2), _T("0"));
		// �G�f�B�b�g�{�b�N�X�̕\�����e���N���A
		SetWindowText(hEdit1, _T(""));

	}
	
	return true;
}

// �x�N�^�[�̒��g���폜
bool ClearMarginTime(HWND hwnd, vector<string>& MarginTime) {

	// �x�N�^�[�̒��g�̍폜
	if (!MarginTime.empty()) {
		MarginTime.clear();
		// �{�^���̉����񐔂̍X�V
		CountButtonClicked = 0;
		SetWindowText(GetDlgItem(hwnd, ID_STATIC_COUNTBUTTONCLICKED_2), _T("0"));
		// �G�f�B�b�g�{�b�N�X�̕\�����e���N���A
		SetWindowText(hEdit1, _T(""));
	}
	else
	{
		MessageBox(hwnd, _T("�z��ɂ͉��������Ă܂���B"), NULL, MB_OK);
	}
	return true;
}

// �{�^���̉����񐔂̍X�V
bool SendCountButtonClicked(HWND hStatic, int& CountButtonClicked) {
	// �{�^���̉����񐔂̑��M
	CountButtonClicked++;
	string CountString = to_string(CountButtonClicked);
	SetWindowText(hStatic, CountString.c_str());
	return true;
}

// �G�f�B�b�g�{�b�N�X�Ɏ�����\��
bool SendNowTimeToEdit(HWND hwndEdit, const char* timeStr) {

	// �G�f�B�b�g�{�b�N�X���̌��݂̕�������擾
	int length = GetWindowTextLength(hwndEdit);
	char* existingText = new char[length + 1];
	GetWindowText(hwndEdit, existingText, length + 1);

	
	char* newText = new char[length + strlen(timeStr) + 3];
	sprintf(newText, "%s\r\n%s", existingText, timeStr);

	// �V�����G�f�B�b�g�{�b�N�X�ɕ������Z�b�g
	SetWindowText(hwndEdit, newText);

	// ��n��
	delete[] existingText;
	delete[] newText;

	return true;
}

// ���ݎ������~���b�܂Ŏ擾
const char* getMilliTime(std::chrono::system_clock::time_point now) {
	// ���݂̎������擾
	//auto now = std::chrono::system_clock::now();

	// �G�|�b�N����̌o�ߎ��Ԃ�b�ƃ~���b�ɕ���
	auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now - seconds);

	// std::time_t�ɕϊ�
	std::time_t tt = std::chrono::system_clock::to_time_t(seconds);

	// ���������[�J���^�C���ɕϊ�
	std::tm* tm = std::localtime(&tt);

	// �w�肳�ꂽ�`���ŏo��
	std::ostringstream oss;
	oss << std::put_time(tm, "%H:%M:%S") << '.'
		<< std::setfill('0') << std::setw(3) << milliseconds.count();

	// ���ʂ�static std::string�ɕϊ�
	static std::string str, a;
	
	str = oss.str();
	// �O���[�o���ϐ��Ɏ������L���icsv�t�@�C���ɏ������ލۂɕK�v)
	TimeStampButtonClicked = str;
	a = _T("�{�^����������: ");
	// ���ʂ�const char*��Ԃ�
	str = a + str;
	return str.c_str();
}




//ASIO�f�o�C�X�̃o�b�t�@�������̃^�C���X�^���v�p�R�[���o�b�N�֐�
ASIOTime* bufferswitchtimeinfo(ASIOTime* params, long doubleBufferIndex, ASIOBool directProcess)
{
	
	return(0L);
}

//ASIO�f�o�C�X�̃T���v�����O���g���ύX���o�p�R�[���o�b�N�֐�
void sampleratedidchange(ASIOSampleRate sRate)
{
	return;
}

//ASIO�f�o�C�X����̃��b�Z�[�W�̏����p�R�[���o�b�N�֐�
long asiomessage(long selector, long value, void* message, double* opt)
{
	return(0L);
}



// NumberOfloops�̕\��
bool DispNumberOfloops() {

	return true;
}
// �t�H���g�̐ݒ�
bool OnFont(HWND hwnd) {

	hFont1 = CreateFont(25, 0, 0, 0, FW_MEDIUM, false, false, false, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, (VARIABLE_PITCH | FF_DONTCARE), _T("���C���I"));

	hFont2 = CreateFont(22, 0, 0, 0, FW_MEDIUM, false, false, false, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, (VARIABLE_PITCH | FF_DONTCARE), _T("���C���I"));

	hFont3 = CreateFont(45, 0, 0, 0, FW_BOLD, false, false, false, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, (VARIABLE_PITCH | FF_DONTCARE), _T("���C���I"));

	SendMessage(hEdit1, WM_SETFONT, (WPARAM)hFont1, MAKELPARAM(false, 0));
	SendMessage(hStaticTime, WM_SETFONT, (WPARAM)hFont1, MAKELPARAM(false, 0));
	// �X�e�e�B�b�N�R���g���[��
	SendMessage(GetDlgItem(hwnd, ID_STATIC_NUMBERDELAY1), WM_SETFONT, (WPARAM)hFont2, MAKELPARAM(false, 0));
	SendMessage(GetDlgItem(hwnd, ID_STATIC_NUMBERDELAY2), WM_SETFONT, (WPARAM)hFont2, MAKELPARAM(false, 0));
	SendMessage(GetDlgItem(hwnd, ID_STATIC_COUNTBUTTONCLICKED), WM_SETFONT, (WPARAM)hFont2, MAKELPARAM(false, 0));
	SendMessage(GetDlgItem(hwnd, ID_STATIC_COUNTBUTTONCLICKED_2), WM_SETFONT, (WPARAM)hFont3, MAKELPARAM(false, 0));
	SendMessage(GetDlgItem(hwnd, ID_STATIC_NUMBERDELAY1), WM_SETFONT, (WPARAM)hFont2, MAKELPARAM(false, 0));
	SendMessage(GetDlgItem(hwnd, ID_STATIC_NUMBERDELAY2), WM_SETFONT, (WPARAM)hFont2, MAKELPARAM(false, 0));
	SendMessage(GetDlgItem(hwnd, ID_GROUP_SETTING), WM_SETFONT, (WPARAM)hFont2, MAKELPARAM(false, 0));
	SendMessage(GetDlgItem(hwnd, ID_STATIC_BPM), WM_SETFONT, (WPARAM)hFont2, MAKELPARAM(false, 0));
	SendMessage(GetDlgItem(hwnd, ID_STATIC_BPMNUM), WM_SETFONT, (WPARAM)hFont2, MAKELPARAM(false, 0));
	SendMessage(GetDlgItem(hwnd, ID_STATIC_TRIALS), WM_SETFONT, (WPARAM)hFont2, MAKELPARAM(false, 0));
	SendMessage(GetDlgItem(hwnd, ID_STATIC_TRIALS_NUM), WM_SETFONT, (WPARAM)hFont2, MAKELPARAM(false, 0));
	SendMessage(GetDlgItem(hwnd, ID_STATIC_LAB), WM_SETFONT, (WPARAM)hFont2, MAKELPARAM(false, 0));
	SendMessage(GetDlgItem(hwnd, ID_STATIC_NORMALORIRREG), WM_SETFONT, (WPARAM)hFont2, MAKELPARAM(false, 0));
	SendMessage(GetDlgItem(hwnd, ID_GROUP_LATEDATA), WM_SETFONT, (WPARAM)hFont2, MAKELPARAM(false, 0));
	SendMessage(GetDlgItem(hwnd, ID_STATIC_FILENAME), WM_SETFONT, (WPARAM)hFont2, MAKELPARAM(false, 0));
	SendMessage(GetDlgItem(hwnd, ID_STATIC_OLD), WM_SETFONT, (WPARAM)hFont2, MAKELPARAM(false, 0));
	SendMessage(GetDlgItem(hwnd, ID_STATIC_NAME), WM_SETFONT, (WPARAM)hFont2, MAKELPARAM(false, 0));
	SendMessage(GetDlgItem(hwnd, ID_GROUP_SUBJECTINFO), WM_SETFONT, (WPARAM)hFont2, MAKELPARAM(false, 0));
	SendMessage(GetDlgItem(hwnd, ID_EDIT_OLD), WM_SETFONT, (WPARAM)hFont2, MAKELPARAM(false, 0));
	SendMessage(GetDlgItem(hwnd, ID_EDIT_NAME), WM_SETFONT, (WPARAM)hFont2, MAKELPARAM(false, 0));
	
	// �R���{�{�b�N�X
	SendMessage(GetDlgItem(hwnd, ID_LATEINI), WM_SETFONT, (WPARAM)hFont1, MAKELPARAM(false, 0));
	SendMessage(GetDlgItem(hwnd, ID_LATESETTING), WM_SETFONT, (WPARAM)hFont1, MAKELPARAM(false, 0));
	// �v�b�V���{�^��
	SendMessage(GetDlgItem(hwnd, ID_BUTTON_CSV), WM_SETFONT, (WPARAM)hFont1, MAKELPARAM(false, 0));
	return true;
}



// �R���{�{�b�N�X�Ƀt�H�[�J�X��������
bool LateIniFunc(HWND hWnd, WPARAM wParam) {

		//�R���{�{�b�N�X�Ō��ݑI������Ă��鍀�ڂ̃C���f�b�N�X���擾
		GetNowComboStr(hWnd);
		// NumberOfloops���v�Z
		CalcLateNumberOfloops(hWnd, &NumberOfloops, Num, lenBuffer, FS, inlatency, outlatency);
		// NumberOfloops��std::string�^�ɕϊ�
		string stringnum = to_string(NumberOfloops);
		// ��ʏ�ɕ\��
		SetWindowText(hStaticNumberOfloops2, stringnum.c_str());
		// �O���[�o���ϐ���NumberOfLoops�̒l���L��������
		TempNumberOfLoops = NumberOfloops;

	return true;
}

// �R���{�{�b�N�X1�őI�����ꂽ�L�[���ɑΉ������x�����Ԃ��R���{�{�b�N�X�ɋl�߂�B
bool SendLate2Combo(HWND hwnd, WPARAM wParam) {

	
		//�R���{�{�b�N�X�Ō��ݑI������Ă��鍀�ڂ̃C���f�b�N�X���擾
		int intCurrentIndex = SendMessage(GetDlgItem(hwnd, (int)ID_LATEINI), CB_GETCURSEL, 0, 0);

		// ���ݑI������Ă��鍀�ڂ̂̕�����̒������擾
		int intTxtLen = SendMessage(GetDlgItem(hwnd, (int)ID_LATEINI), CB_GETLBTEXTLEN, intCurrentIndex, 0);

		if (intTxtLen != CB_ERR){
			char* pszBuf = new char[intTxtLen + 1];
			if (SendMessage(GetDlgItem(hwnd, ID_LATEINI), CB_GETLBTEXT, intCurrentIndex, (LPARAM)pszBuf) != CB_ERR) {
				char Path[MAX_PATH + 1];
				char settingpath[MAX_PATH + 1];
				if (GetModuleFileName(NULL, Path, MAX_PATH) != 0) {
					char drive[MAX_PATH + 1], dir[MAX_PATH + 1], fname[MAX_PATH + 1], ext[MAX_PATH + 1];
					// �p�X���𕪉�
					_splitpath(Path, drive, dir, fname, ext);
					_stprintf_s(settingpath, MAX_PATH + 1, _T("%s%ssetting.ini"), drive, dir);
				}
				char latedata[256];
				// ini�t�@�C���̓ǂݍ���
				GetPrivateProfileString(pszBuf, _T("data"), _T("Error"), latedata, sizeof(latedata), settingpath);

				// �R���{�{�b�N�X�̒��g������
				while (SendMessage(GetDlgItem(hwnd, (int)ID_LATESETTING), CB_GETCOUNT, 0, 0)){
					SendMessage(GetDlgItem(hwnd, (int)ID_LATESETTING), CB_DELETESTRING, 0, 0);
				}

				stringstream ss, tt;
				ss << latedata;
				string s, t;

				int i = 0;
				while (getline(ss, s, ',')) {
					char* cstr = new char[s.size() + 1];
					char_traits<char>::copy(cstr, s.c_str(), s.size() + 1);
					// �R���{�{�b�N�X�ɕ������}��
					SendMessage(GetDlgItem(hwnd, ID_LATESETTING), CB_INSERTSTRING, i, (LPARAM)cstr);
					i++;
				}
				// �R���{�{�b�N�X�ɐ擪�̗v�f���Z�b�g
				SendMessage(GetDlgItem(hwnd, ID_LATESETTING), CB_SETCURSEL, 0, 0);
			}
			
			delete[] pszBuf;
		}
		
	

	return true;
}

// �R���{�{�b�N�X�Ō��ݑI������Ă��鍀�ڂ̕�������擾����֐�
bool GetNowComboStr(HWND hWnd) {

	//�R���{�{�b�N�X�Ō��ݑI������Ă��鍀�ڂ̃C���f�b�N�X���擾
	int intCurrentIndex = SendMessage(GetDlgItem(hWnd, (int)ID_LATESETTING), CB_GETCURSEL, 0, 0);

	// �R���{�{�b�N�X�̌��ݑI������Ă��鍀�ڂ̕�����̒������擾
	int intTxtLen = SendMessage(GetDlgItem(hWnd, (int)ID_LATESETTING), CB_GETLBTEXTLEN, intCurrentIndex, 0);

	if (intTxtLen != CB_ERR) {
		char* pszBuf = new char[intTxtLen + 1];
		// �R���{�{�b�N�X�̈ꗗ����I���������ڂ̕�������擾
		if (SendMessage(GetDlgItem(hWnd, (int)ID_LATESETTING), CB_GETLBTEXT, intCurrentIndex, (LPARAM)pszBuf) != CB_ERR) {
			// char*�^��int�^�ɕϊ�
			Num = atoi(pszBuf);
		}

		delete[] pszBuf;
	}
	return true;
}


// �R���{�{�b�N�X�̑I�����ڂ���x�����Ԃ��Z�o
bool CalcLateNumberOfloops(HWND hwnd, int* NumberOfloops, int  Num, int lenbuffer, int rate, long inlatency, long outlatency){
	/* ����: �E�B���h�E�n���h��, �R���{�{�b�N�X���瓾�镶����, �o�b�t�@��, �T���v�����O���g��, 
	*          �C���v�b�g���C�e���V, �A�E�g�v�b�g���C�e���V
	*/
	////////////////////////
	// �ϐ���ms�P�ʂɕϊ�
	//////////////////////////
	TCHAR NumberOfloopsInfo[LENSTR];
	// ���̓��C�e���V�[�i�o�b�t�@2���̎��Ԃ����Z�j
	double inlatency_ms = (double)inlatency * 1000.0 / (double)rate - (double)lenbuffer * 2.0 * 1000.0 / (double)rate;
	// �o�̓��C�e���V�[
	double outlatency_ms = (double)outlatency * 1000.0 / (double)rate;
	// �p���X�g������H�ł̒x��
	double kairo = (double)KAIROLATE;

	// NumberOfloops�̌v�Z
	*NumberOfloops = ((double)Num - (inlatency_ms + outlatency_ms + kairo)) * (double)rate / ((double)lenbuffer * 1000.0);

	////���b�Z�[�W�{�b�N�X�ւ̏o��
	//_stprintf_s(NumberOfloopsInfo, LENSTR,
	//	_T("���̓��C�e���V: %f [ms]\n"
	//	"�o�̓��C�e���V: %f [ms]\n"
	//	"���o�̓��C�e���V: %f [ms]\n"
	//	"�x��������������: %d [ms]\n"
	//	"��H�ɂ��x��: %f [ms]\n"
	//	"�o�b�t�@��: %d [points]\n"
	//	"Fs: %d [Hz]\n"
	//	"NumberOfloops = %d\n"
	//	"NumberOfloops(double) = %f"),
	//	inlatency_ms, outlatency_ms, inlatency_ms + outlatency_ms, Num, kairo, lenbuffer, rate, *NumberOfloops,
	//	((double)Num - (inlatency_ms + outlatency_ms + kairo)) * (double)rate / ((double)lenbuffer * 1000.0));

	//MessageBox(hwnd, NumberOfloopsInfo, _T("�v�Z����"), MB_OK);

	return true;

}

// �G�f�B�b�g�{�b�N�X�������I�ɉ��܂ŃX�N���[������
bool ScrollToBottom(HWND hEditBox) {
	// �G�f�B�b�g�{�b�N�X���̕�����̒������擾
	int len = GetWindowTextLength(hEditBox);
	// �����I�������ɃG�f�B�b�g�{�b�N�X���̃e�L�X�g�̍Ō�ɃL�����b�g���ړ�
	SendMessage(hEditBox, EM_SETSEL, (WPARAM)len, (LPARAM)len);
	// �G�f�B�b�g�{�b�N�X���J�[�\���܂ŃX�N���[������
	SendMessage(hEditBox, EM_SCROLLCARET, 0, 0);

	return true;
}

// �팱�ҏ����x�N�^�ɕۑ�
bool GetEditBoxTextFunc(HWND hwnd) {
	char* lpstringOld = new char[10];
	char* lpstringName = new char[50];

	GetWindowText(GetDlgItem(hwnd, ID_EDIT_OLD), lpstringOld, 10);
	GetWindowText(GetDlgItem(hwnd, ID_EDIT_NAME), lpstringName, 50);

	string Old = lpstringOld;
	string Name = lpstringName;

	MarginTime.insert(MarginTime.begin(), Old);
	MarginTime.insert(MarginTime.begin(), Name);

	delete[] lpstringOld;
	delete[] lpstringName;

	return true;
}

bool OnCommand(HWND hwnd, WPARAM wparam) {

	switch (LOWORD(wparam)) {
	case ID_ASIOINFORMATION:
		//ASIO���̕\��
		MessageBox(NULL, strASIOInfo, _T("ASIO"), MB_OK);
		break;

	case ID_WAVEFILEINFORMATION:
		//Wave�t�@�C�����̕\��
		MessageBox(NULL, strWaveFileInfo, _T("Wave File"), MB_OK);
		break;

	case ID_EXIT:
		//�v���O�����̏I��
		if (IDYES == MessageBox(hwnd, _T("�I�����܂����H"), _T("�I���m�F"), MB_YESNO)) {
			DestroyWindow(hwnd);
		}
		break;
		
		// �x�����Ԃ̐ݒ�
		// �z��̍폜
	case ID_MENU_RESTART:
		if (MessageBox(hwnd, _T("���ʂ��t�@�C���ɏ������݂܂������H"), _T("�m�F"), MB_YESNO) == IDYES) {
			if (ClearMarginTime(hwnd, MarginTime)) {
				MessageBox(hwnd, _T("���ʂ��폜���܂����B"), _T("�폜����"), MB_OK);
			}
		}
		break;
	case ID_DELETE_ARRAY:
		/*NumberOfloops = 1;
		SetWindowText(hStaticNumberOfloops2, _T("1"));*/
		if (ClearMarginTime(hwnd, MarginTime)) {
			MessageBox(hwnd, _T("���ʂ��폜���܂����B"), _T("�폜����"), MB_OK);
		}
		break;
		///////////////////
		// �R���{�{�b�N�X
		///////////////////
	case ID_LATEINI:
		// ��{���
		if (HIWORD(wparam) == CBN_SELCHANGE) {
			SendLate2Combo(hwnd, wparam);
		}
		break;
		
	case ID_LATESETTING:
		// �ڍ׏��
		if (HIWORD(wparam) == CBN_SELCHANGE) {
			LateIniFunc(hwnd, wparam);
		}
		break;
	// �擾�񐔂̐ݒ�
	case ID_STATIC_TRIALS_NUM:
		if (HIWORD(wparam) == CBN_SELCHANGE) {
			int itemIndex = SendMessage(GetDlgItem(hwnd, ID_STATIC_TRIALS_NUM), CB_GETCURSEL, 0, 0);
			if (itemIndex != CB_ERR) {
				char itemData[10] = {0};
				SendMessage(GetDlgItem(hwnd, ID_STATIC_TRIALS_NUM), CB_GETLBTEXT, itemIndex, (LPARAM)itemData);
				int Temp = _ttol(itemData);
				TempNumberOfTrials = Temp + 5;
			}
		}
		break;
		// �������@�̑I��
	case ID_STATIC_NORMALORIRREG:
		if (HIWORD(wparam) == CBN_SELCHANGE) {
			int itemIndex = SendMessage(GetDlgItem(hwnd, ID_STATIC_NORMALORIRREG), CB_GETCURSEL, 0, 0);
			if (itemIndex != CB_ERR) {
				if (itemIndex) {
					if (DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MYDIALOG), hwnd, DialogProc) == -1) {
						MessageBox(hwnd, _T("�_�C�A���O�{�b�N�X�̍쐬�Ɏ��s���܂����B"), _T("�G���["), MB_OK);
					}
					LABNormal = false;
				}
				else LABNormal = true;
			}
		}
		break;
		// BPM�̑I��
	case ID_STATIC_BPMNUM:
		if (HIWORD(wparam) == CBN_SELCHANGE) {
			int itemIndex = SendMessage(GetDlgItem(hwnd, ID_STATIC_BPMNUM), CB_GETCURSEL, 0, 0);
			if (itemIndex != CB_ERR) {
				/*BPM�̕ύX�ɔ��������������ɏ���*/
			}
		}
		break;
	////////////////////////////////
	// �u���j���[�v���u�������@�v
	// /////////////////////////////
	case ID_MENU_NORMAL:
		SetWindowText(GetDlgItem(hParentWindow, ID_STATIC_NORMALORIRREG), _T("�ʏ�"));
		LABNormal = true;
		break;
	case ID_MENU_IRREG:
		SetWindowText(GetDlgItem(hParentWindow, ID_STATIC_NORMALORIRREG), _T("�ϑ�"));
		LABNormal = false;
		break;
	///////////////////////////
	// �u���j���[�v���u�ݒ�v
	///////////////////////////
	// BPM�̑I���iBPM�ɂ���Č��ʂ̏o�͐��ύX����)
	case ID_BPM60:
		//CSVFILENAME = _T("RESULT\\ResultCSV60.csv"); 
		CSVFILENAME = _T("RESULT\\Result_onlylight\\Result60BPM10ms.csv");
		SetWindowText(GetDlgItem(hParentWindow, ID_STATIC_BPMNUM), _T("60"));
		break;
	case ID_BPM70:
		 //CSVFILENAME = _T("RESULT\\ResultCSV70.csv"); 
		 CSVFILENAME = _T("RESULT\\Result_onlylight\\Result70BPM10ms.csv");
		 SetWindowText(GetDlgItem(hParentWindow, ID_STATIC_BPMNUM), _T("70"));
		break;
	case ID_BPM80:
		//CSVFILENAME = _T("RESULT\\ResultCSV80.csv"); 
		CSVFILENAME = _T("RESULT\\Result_onlylight\\Result80BPM10ms.csv");
		SetWindowText(GetDlgItem(hParentWindow, ID_STATIC_BPMNUM), _T("80"));
		break;
	case ID_BPM90:
		//CSVFILENAME = _T("RESULT\\ResultCSV90.csv"); 
		CSVFILENAME = _T("RESULT\\Result_onlylight\\Result90BPM10ms.csv");
		SetWindowText(GetDlgItem(hParentWindow, ID_STATIC_BPMNUM), _T("90"));
		break;
	case ID_BPM100:
		//CSVFILENAME = _T("RESULT\\ResultCSV100.csv"); 
		CSVFILENAME = _T("RESULT\\Result_onlylight\\Result100BPM10ms.csv");
		SetWindowText(GetDlgItem(hParentWindow, ID_STATIC_BPMNUM), _T("100"));
		break;
	case ID_BPM110:
		//CSVFILENAME = _T("RESULT\\ResultCSV110.csv"); 
		CSVFILENAME = _T("RESULT\\Result_onlylight\\Result110BPM10ms.csv");
		SetWindowText(GetDlgItem(hParentWindow, ID_STATIC_BPMNUM), _T("110"));
		break;
	////////
	// ���s��
	///	/////////
	case ID_MENU_NUM20:
		TempNumberOfTrials = 25;
		SetWindowText(GetDlgItem(hParentWindow, ID_STATIC_TRIALS_NUM), _T("20"));
		break;
	case ID_MENU_NUM30:
		TempNumberOfTrials = 35;
		SetWindowText(GetDlgItem(hParentWindow, ID_STATIC_TRIALS_NUM), _T("30"));
		break;
	case ID_MENU_NUM40:
		TempNumberOfTrials = 45;
		SetWindowText(GetDlgItem(hParentWindow, ID_STATIC_TRIALS_NUM), _T("40"));
		break;
		///////////////
	// �t�@�C���̎w��
		////////////////
	case ID_MENU_CSV_OUTPUT:
	{
		// �t�@�C���I��p�_�C�A���O�̕\��
		SelectFile(hwnd, FileNameCSV);
		// �p�X����string�^�ɕϊ�
		CSVFILENAME = FileNameCSV;
		// �o�͐�t�@�C�����E�B���h�E��ɕ\��
		RelativepathFromAbsolutepath(FileNameCSV, hwnd);
	}
		break;
		// �v�b�V���{�^��
	case ID_BUTTON_CSV:
		if (HIWORD(wparam) == BN_CLICKED) {
			
			// �t�@�C���I��p�_�C�A���O�̕\��
			SelectFile(hwnd, FileNameCSV);
			// ���ɊJ����Ă��邩���`�F�b�N
			if (!CheckCanWriteFile(hwnd, FileNameCSV)) {
				// �t�@�C�����J���̂Ɏ��s�����ꍇ�A�������݂��s�\�ł���\��������
				MessageBox(hwnd, _T("�t�@�C���ɏ������߂܂���B���ɊJ����Ă���\��������܂��B\r\n�t�@�C������Ă���ēx�{�^���������Ă��������B"), _T("�x��"), MB_OK | MB_ICONWARNING);
				break;
			}
			// �p�X����string�^�ɕϊ�
			CSVFILENAME = FileNameCSV;
			// �o�͐�t�@�C�����E�B���h�E��ɕ\��
			RelativepathFromAbsolutepath(FileNameCSV, hwnd);
			// �l�̏�������
			GetEditBoxTextFunc(hParentWindow);                                            // �팱�ҏ����x�N�^�̐擪�ɑ�� 
			MarginTime.insert(MarginTime.begin(), to_string(Num));                 // �x�����Ԃ��x�N�^�̐擪�ɑ��
			MarginTime.insert(MarginTime.begin(), TimeStampButtonClicked);   // �����������x�N�^�̐擪�ɑ��
			if (!LABNormal) {
				string tempunique = _T("unique");
				string stringDelayTiming = to_string(DelayTiming);
				string stringDelayTime_ms = to_string(DelayTime_ms);
				// �����������ϑ��̏ꍇ�A�x�N�^�̐擪�ɕ��������
				MarginTime.insert(MarginTime.begin(), stringDelayTiming);        // �x���̃^�C�~���O
				MarginTime.insert(MarginTime.begin(), stringDelayTime_ms);    // �x������
				MarginTime.insert(MarginTime.begin(), tempunique);                // unique
			}
			WriteToCSV(hwnd, MarginTime, CSVFILENAME);
		}
		break;
	}

	return true;
}

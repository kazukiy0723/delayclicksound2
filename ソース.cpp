//////
//Delay Click Sound by ASIO Ver. 2 (オーバーラップなし、再生音開始点調整あり、チャタリング考慮、音声遅延あり(別チャンネル再生))
//////
#include<windows.h>
#include<tchar.h>
#include<stdio.h>
#include<Xinput.h>
#include"resource.h"
#include"ginclude.h"
#include"asio.h"
#include"asiodrivers.h"

// 追加ファイル
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

// Visualスタイル有効化
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

// 追加変数
extern HWND hStaticNowTime, hStaticTime, hEdit1, htaticNumberOfloops, hStaticNumberOfloops2;
HFONT hFont1, hFont2, hFont3;
RECT recthEdit1 = { 198, 98, 702, 402 };
int call_count, temp_count;
bool wavePlayed = false;
extern int NumberOfloops;
// 処理時間計測用
std::chrono::system_clock::time_point start, before_start;
extern int Num;
//　時間差を保存するためのベクタ
vector<string> MarginTime;
// ボタンの押下回数を保持する変数
int CountButtonClicked;
// 親ウィンドウのハンドルを保持
HWND hParentWindow;
// 結果の出力先のパスを保持する変数
string CSVFILENAME;
// 実験方法の選択
bool LABNormal;
int TempNumberOfLoops;
// 試行回数を保持する変数
int TempNumberOfTrials;
// ファイル名
static char FileNameCSV[MAX_PATH];
// タイムスタンプ
string TimeStampButtonClicked = _T("Nothing");

// dialogbox.cppと共有
extern int DelayTiming, DelayTime_ms;
int DelayTimeUnique;

//WinMain関数
int WINAPI WinMain(_In_ HINSTANCE hThisInst, _In_opt_ HINSTANCE hPrevInst, _In_ LPSTR lpszArgs, _In_ int nWinMode)
{
	HWND			hwnd;
	MSG				msg;
	WNDCLASSEX		wcl;
	HACCEL			haccel;

	//ウィンドウクラスの定義
	wcl.cbSize = sizeof(WNDCLASSEX);				//WNDCLASSEX構造体のサイズ
	wcl.style = 0;									//ウィンドウクラススタイル
	wcl.lpfnWndProc = windowfunc;					//ウィンドウ関数
	wcl.cbClsExtra = 0;								//ウィンドウクラスのエキストラ
	wcl.cbWndExtra = 0;								//ウィンドウインスタンスのエキストラ
	wcl.hInstance = hThisInst;						//このプログラムのインスタンスへのハンドル
	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//アイコンへのハンドル
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);		//カーソルへのハンドル
	wcl.hbrBackground = (HBRUSH)COLOR_WINDOW;		//背景ブラシへのハンドル
	wcl.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);	//メニュー
	wcl.lpszClassName = szWinName;					//ウィンドウクラス名
	wcl.hIconSm = LoadIcon(NULL, IDI_WINLOGO);		//スモールアイコンへのハンドル

	//ウィンドウクラスの登録
	if (!RegisterClassEx(&wcl)) {
		return(0);
	}

	//ウィンドウの生成
	hwnd = CreateWindow(
		szWinName,				//ウィンドウクラス名
		szWinName,				//ウィンドウ名
		WS_OVERLAPPEDWINDOW,	//ウィンドウスタイル
		CW_USEDEFAULT,			//x座標
		CW_USEDEFAULT,			//y座標
		CW_USEDEFAULT,			//幅
		CW_USEDEFAULT,			//高さ
		HWND_DESKTOP,			//親ウィンドウへのハンドル
		NULL,					//メニューへのハンドル
		hThisInst,				//このプログラムのインスタンスへのハンドル	
		NULL					//追加引数
	);

	// 親ウィンドウハンドルの記憶
	hParentWindow = hwnd;

	//ウィンドウの表示
	ShowWindow(hwnd, nWinMode);
	UpdateWindow(hwnd);

	//キーボードアクセラレータのロード
	haccel = LoadAccelerators(hThisInst, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	//メッセージループの生成
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(hwnd, haccel, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return((int)msg.wParam);
}

//ウィンドウ関数
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
		//ウィンドウの初期化
		// ウィンドウの作成
		CreateControl(hwnd, wparam, lparam);
		// 実行ファイルのパス名の取得
		TCHAR lpFileName[MAX_PATH];
		GetModuleFileName(NULL, lpFileName, sizeof(lpFileName));
		// ウィンドウのタイトルを実行ファイルのパス名に変更
		SetWindowText(hwnd, lpFileName);
		//パラメータの初期化
		idDevice = -1;
		hMmio = NULL;
		tmpWaveData = NULL;
		WaveData = NULL;
		isPlaying = false;
		isLoadedDriver = false;
		isOpenedDevice = false;
		isCreatedBuffer = false;
		idxWaveData = 0;
		// 追加
		call_count = 1;
		temp_count = 0;
		NumberOfloops = 1;
		TempNumberOfLoops = 1;
		CountButtonClicked = 0;
		LABNormal = true;
		TempNumberOfTrials = 35;
		//ASIOデバイス用メモリ領域の確保
		if (asioDrivers == NULL) {
			asioDrivers = new AsioDrivers();
		}

		//ASIOデバイス名の取得
		for (idev = 0; idev < MAXNUMDEVS; idev++) {
			tmpnames[idev] = devnames[idev];
		}
		numdevs = asioDrivers->getDriverNames(tmpnames, MAXNUMDEVS);
		if (numdevs == 0) {
			MessageBox(NULL, _T("asioDrivers->getDriverNames"), _T("Error"), MB_ICONSTOP | MB_OK);
		}

		//ASIOデバイスのドライバのロード
		boolresult = asioDrivers->loadDriver(devnames[IDDEV]);
		if (boolresult == false) {
			isLoadedDriver = false;
			_stprintf_s(str, LENSTR, _T("asioDrivers->loadDriver\nDevice Name: %s\n"), devnames[IDDEV]);
			MessageBox(NULL, str, _T("Error"), MB_ICONSTOP | MB_OK);
		}
		else {
			isLoadedDriver = true;
		}

		//ASIOデバイスのオープン
		memset(&DriverInfo, 0, sizeof(ASIODriverInfo));
		asioresult = ASIOInit(&DriverInfo);
		if (asioresult != ASE_OK) {
			isOpenedDevice = false;
			_stprintf_s(str, LENSTR, _T("ASIOInit\nDevice Name: %s\n"), devnames[IDDEV]);
			MessageBox(NULL, str, _T("Error"), MB_ICONSTOP | MB_OK);

			//ASIOデバイスのドライバのアンロード
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

					//ASIOデバイスのオープン
					memset(&DriverInfo, 0, sizeof(ASIODriverInfo));
					asioresult = ASIOInit(&DriverInfo);
					if (asioresult != ASE_OK) {
						isOpenedDevice = false;
						_stprintf_s(str, LENSTR, _T("ASIOInit\nDevice Name: %s\n"), devnames[idev]);
						MessageBox(NULL, str, _T("Error"), MB_ICONSTOP | MB_OK);

						//ASIOデバイスのドライバのアンロード
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

		//ASIOデバイスのサンプリング周波数のサポートの確認
		rate = FS;
		asioresult = ASIOCanSampleRate(rate);
		if (asioresult != ASE_OK) {
			MessageBox(NULL, _T("ASIOCanSampleRate"), _T("Error"), MB_ICONSTOP | MB_OK);
		}

		//ASIOデバイスのサンプリング周波数の設定
		asioresult = ASIOSetSampleRate(rate);
		if (asioresult != ASE_OK) {
			MessageBox(NULL, _T("ASIOSetSampleRate"), _T("Error"), MB_ICONSTOP | MB_OK);
		}

		//ASIOデバイスのチャンネル数の取得
		asioresult = ASIOGetChannels(&numinchs, &numoutchs);
		if (asioresult != ASE_OK) {
			MessageBox(NULL, _T("ASIOGetChannels"), _T("Error"), MB_ICONSTOP | MB_OK);
		}

		//ASIOデバイスの録音チャンネル数の設定
		if (numinchs <= MAXNUMINCHS) {
			numInChannels = numinchs;
		}
		else {
			numInChannels = MAXNUMINCHS;
		}

		//ASIOデバイスの再生用チャンネル数の設定
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

		//ASIOデバイスのバッファサイズの取得
		asioresult = ASIOGetBufferSize(&minlenbuffer, &maxlenbuffer, &preflenbuffer, &granularity);
		if (asioresult != ASE_OK) {
			MessageBox(NULL, _T("ASIOGetBufferSize"), _T("Error"), MB_ICONSTOP | MB_OK);
		}

		//ASIOデバイスのバッファサイズの設定
		lenBuffer = preflenbuffer;

		//ASIOデバイスの録音用バッファの初期化
		for (ich = 0; ich < numInChannels; ich++) {
			memset(&(BufInfo[ich]), 0, sizeof(ASIOBufferInfo));
			BufInfo[ich].isInput = ASIOTrue;
			BufInfo[ich].channelNum = ich;
		}

		//ASIOデバイスの再生用バッファの初期化
		for (ich = 0; ich < numOutChannels; ich++) {
			memset(&(BufInfo[numInChannels + ich]), 0, sizeof(ASIOBufferInfo));
			BufInfo[numInChannels + ich].isInput = ASIOFalse;
			BufInfo[numInChannels + ich].channelNum = ich;
		}

		//ASIOデバイス用コールバック関数の設定
		CallBacks.bufferSwitch = &bufferswitch;
		CallBacks.bufferSwitchTimeInfo = &bufferswitchtimeinfo;
		CallBacks.sampleRateDidChange = &sampleratedidchange;
		CallBacks.asioMessage = &asiomessage;

		//ASIOデバイスのバッファの生成
		asioresult = ASIOCreateBuffers(BufInfo, numInChannels + numOutChannels, lenBuffer, &CallBacks);
		if (asioresult != ASE_OK) {
			isCreatedBuffer = false;
			MessageBox(NULL, _T("ASIOCreateBuffers"), _T("Error"), MB_ICONSTOP | MB_OK);
		}
		else {
			isCreatedBuffer = true;
		}

		//ASIOデバイスのチャンネル情報の取得
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

		//ASIOデバイスのレイテンシの取得
		asioresult = ASIOGetLatencies(&inlatency, &outlatency);
		if (asioresult != ASE_OK) {
			MessageBox(NULL, _T("ASIOGetLatencies"), _T("Error"), MB_ICONSTOP | MB_OK);
		}

		//ASIOOutputReady関数のサポートの確認
		asioresult = ASIOOutputReady();
		if (asioresult == ASE_OK) {
			SupportASIOOutputReady = true;
		}
		else {
			SupportASIOOutputReady = false;
		}

		//ASIOデバイスの設定内容の確認
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

		// コンボボックスの作成とiniファイルの読み込み
		ReadIniFile(hwnd, lparam);

		//WAVEファイルのオープン
		if ((hMmio = mmioOpen(wavefilename, NULL, MMIO_READ)) == NULL) {
			//ファイルをオープンできなかった場合の処理
			_stprintf_s(str, LENSTR, _T("エラー: ファイルを開くことができません\nファイル名: %s"), wavefilename);
			MessageBox(hwnd, str, _T("ファイルを開く"), MB_ICONSTOP | MB_OK);
			return(0);
		}

		//ファイルのWAVEチャンクへ移動
		memset(&mmckinfoparent, 0, sizeof(MMCKINFO));
		mmckinfoparent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		if (mmioDescend(hMmio, &mmckinfoparent, NULL, MMIO_FINDRIFF) != MMSYSERR_NOERROR) {
			//チャンクを移動できなかった場合の処理
			_stprintf_s(str, LENSTR, _T("エラー: データを読み込むことができません\nファイル名: %s"), wavefilename);
			MessageBox(hwnd, str, _T("ファイルを開く"), MB_ICONSTOP | MB_OK);
			return(0);
		}

		//ファイルのfmtチャンクへ移動
		memset(&mmckinfosubchunk, 0, sizeof(MMCKINFO));
		mmckinfosubchunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
		if (mmioDescend(hMmio, &mmckinfosubchunk, &mmckinfoparent, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
			//チャンクを移動できなかった場合の処理
			_stprintf_s(str, LENSTR, _T("エラー: データを読み込むことができません\nファイル名: %s"), wavefilename);
			MessageBox(hwnd, str, _T("ファイルを開く"), MB_ICONSTOP | MB_OK);
			return(0);
		}

		//ファイルのデータ形式の読み込み
		memset(&wf, 0, sizeof(WAVEFORMATEX));
		if (mmioRead(hMmio, (HPSTR)&wf, mmckinfosubchunk.cksize) == -1) {
			//ファイルのデータ形式を読み込めなかった場合の処理
			_stprintf_s(str, LENSTR, _T("エラー: データを読み込むことができません\nファイル名: %s"), wavefilename);
			MessageBox(hwnd, str, _T("ファイルを開く"), MB_ICONSTOP | MB_OK);
			return(0);
		}

		//ファイルのデータ形式の確認
		if (wf.wFormatTag != WAVE_FORMAT_PCM) {
			//ファイルのデータ形式がPCMではなかった場合の処理
			_stprintf_s(str, LENSTR, _T("エラー: PCM形式WAVEデータではないので読み込むことができません\nファイル名: %s"), wavefilename);
			MessageBox(hwnd, str, _T("ファイルを開く"), MB_ICONSTOP | MB_OK);
			return(0);
		}

		//ファイルのdataチャンクへ移動
		if (mmioAscend(hMmio, &mmckinfosubchunk, 0) != MMSYSERR_NOERROR) {
			//チャンクを移動できなかった場合の処理
			_stprintf_s(str, LENSTR, _T("エラー: データを読み込むことができません\nファイル名: %s"), wavefilename);
			MessageBox(hwnd, str, _T("ファイルを開く"), MB_ICONSTOP | MB_OK);
			return(0);
		}
		memset(&mmckinfosubchunk, 0, sizeof(MMCKINFO));
		mmckinfosubchunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
		if (mmioDescend(hMmio, &mmckinfosubchunk, &mmckinfoparent, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
			//チャンクを移動できなかった場合の処理
			_stprintf_s(str, LENSTR, _T("エラー: データを読み込むことができません\nファイル名: %s"), wavefilename);
			MessageBox(hwnd, str, _T("ファイルを開く"), MB_ICONSTOP | MB_OK);
			return(0);
		}
		lenWaveData = mmckinfosubchunk.cksize / wf.nBlockAlign;

		//メッセージボックスへのテキストの出力
		_stprintf_s(strWaveFileInfo, LENSTR,
			_T("File Name: %s\n")
			_T("Sampling Frequency: %d Hz\n")
			_T("Bits per Sample: %d bits\n")
			_T("Number of Channels: %d channels\n")
			_T("Length of Data: %d samples"),
			wavefilename, wf.nSamplesPerSec, wf.wBitsPerSample, wf.nChannels, lenWaveData);
		MessageBox(NULL, strWaveFileInfo, _T("Read a Wave File"), MB_OK);

		//WAVEデータ用バッファの生成
		tmpWaveData = (void*)malloc(mmckinfosubchunk.cksize);
		WaveData = (void*)malloc(lenWaveData * (BitsPerSample / 8));
		if (tmpWaveData == NULL || WaveData == NULL) {
			//バッファを生成できなかった場合の処理
			MessageBox(hwnd, _T("エラー: メモリーが足りません"), _T("ファイルを開く"), MB_ICONSTOP | MB_OK);
			return(0);
		}

		//WAVEデータの読み込み
		if (mmioRead(hMmio, (HPSTR)tmpWaveData, mmckinfosubchunk.cksize) <= 0) {
			//データを読み込めなかった場合の処理
			_stprintf_s(str, LENSTR, _T("エラー: データを読み込むことができません\nファイル名: %s"), wavefilename);
			MessageBox(hwnd, str, _T("ファイルを開く"), MB_ICONSTOP | MB_OK);
			return(0);
		}

		//WAVEファイルのクローズ
		if (hMmio != NULL) {
			mmioClose(hMmio, 0);
			hMmio = NULL;
		}

		//WAVEデータの変換
		for (isample = 0; isample < lenWaveData; isample++) {
			tmpdouble = 0.0;
			for (ich = 0; ich < wf.nChannels; ich++) {
				//WAVEデータのshort int型への変換
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

			//WAVEデータのshort int型からの変換
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

		//WAVEデータ用バッファの解放
		if (tmpWaveData != NULL) {
			free(tmpWaveData);
			tmpWaveData = NULL;
		}

		// フォントの初期設定
		OnFont(hwnd);

		//ASIOデバイスの再生用バッファの初期化
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

		//ASIOデバイスの開始
		asioresult = ASIOStart();
		if (asioresult != ASE_OK) {
			MessageBox(NULL, _T("ASIOStart"), _T("Error"), MB_ICONSTOP | MB_OK);
		}
		break;

	case WM_COMMAND:
		OnCommand(hwnd, wparam);
		break;
	case WM_CLOSE:
		//プログラムの終了
		if (IDYES == MessageBox(hwnd, _T("終了しますか？"), _T("終了確認"), MB_YESNO)) {
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

		// 後始末
		DeleteObject(hFont1);
		DeleteObject(hFont2);
		DeleteObject(hFont3);
		//ASIOデバイスの停止
		asioresult = ASIOStop();
		if (asioresult != ASE_OK) {
			MessageBox(NULL, _T("ASIOStop"), _T("Error"), MB_ICONSTOP | MB_OK);
		}

		//ASIOデバイスのバッファの解放
		asioresult = ASIODisposeBuffers();
		if (asioresult != ASE_OK) {
			MessageBox(NULL, _T("ASIODisposeBuffers"), _T("Error"), MB_ICONSTOP | MB_OK);
		}

		//ASIOデバイスのクローズ
		if (isOpenedDevice == true) {
			asioresult = ASIOExit();
			if (asioresult != ASE_OK) {
				MessageBox(NULL, _T("ASIOExit"), _T("Error"), MB_ICONSTOP | MB_OK);
			}
			isOpenedDevice = false;
		}

		//ASIOデバイスのドライバのアンロード
		if (isLoadedDriver == true) {
			asioDrivers->removeCurrentDriver();
			isLoadedDriver = false;
		}

		//ASIOデバイス用メモリ領域の解放
		if (asioDrivers != NULL) {
			delete asioDrivers;
		}

		//WAVEデータ用バッファの解放
		if (tmpWaveData != NULL) {
			free(tmpWaveData);
			tmpWaveData = NULL;
		}
		if (WaveData != NULL) {
			free(WaveData);
			WaveData = NULL;
		}

		//WAVEファイルのクローズ
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



//ASIOデバイスのバッファ処理用コールバック関数
void bufferswitch(long index, ASIOBool processNow) {
	ASIOError	asioresult;
	int			tmplenbuffer, isample, position;
	double	tmpdouble;
	long		tmplong;
	

	//録音データ(クリック信号)の変換
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
				// パラメータの設定
				isPlaying = true;
				idxWaveData = 0;
				position = isample;
				call_count = 1;
				// 前回のボタン押下時刻の取得
				if (CountButtonClicked)  before_start = start;
				// 現在時刻の取得
				start = std::chrono::system_clock::now();
				// エディットボックスにボタンの押下時刻を表示
				SendNowTimeToEdit(hEdit1, getMilliTime(start));
				if (CountButtonClicked) {
					// 前回のボタン押下時刻との時間差を計算し、表示
					auto time = start - before_start;
					SendMarginTimeToEdit(time);
					// ベクター配列に結果を保存
					GetMarginTime(time);
					// テキストの最後にカーソルを移動
					ScrollToBottom(hEdit1);
				}
				if (!LABNormal) {
					/////////////////////////
					// 実験方法が変則の場合
					// ///////////////////////
					/*ここにクリック音の再生タイミングをずらすためのプログラムを書く。*/
					if (!(CountButtonClicked % DelayTiming)) {
						// ボタンの押下回数が4の倍数のとき、再生タイミングをずらす
						CalcLateNumberOfloops(hParentWindow, &DelayTimeUnique, DelayTime_ms, lenBuffer, FS, inlatency, outlatency);
						NumberOfloops = NumberOfloops + DelayTimeUnique;
					}else if(NumberOfloops != TempNumberOfLoops) {
						NumberOfloops = TempNumberOfLoops;
					}
				}
				// 実験終了のためのメッセージボックスを出力
				if (CountButtonClicked == TempNumberOfTrials) {
					string TempNumberString = to_string(TempNumberOfTrials);
					MessageBox(NULL, TempNumberString.c_str(), _T("終了の合図"), MB_OK);
				}
				// ボタンの押下回数を更新
				SendCountButtonClicked(GetDlgItem(hParentWindow, ID_STATIC_COUNTBUTTONCLICKED_2), CountButtonClicked);
				break;
			}
		}
	}
	if (isample == lenBuffer) {
		position = 0;
	}

	//ASIOデバイスのクリック音再生用バッファのクリア
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
		// ボタンが押されてから(NumberOfloops)回目のbufferswitch呼び出し時にWaveデータをコピー
		if (call_count % NumberOfloops != 0) {
			// 録音データ(音声信号)のコピー（出力バッファー2にコピー）
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
			//WAVEデータのクリック音再生用バッファへのコピー（出力バッファー2にコピー）
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
				//パラメータの設定
				////////////////////
				isPlaying = false;
				idxWaveData = 0;
				call_count = 1;
			}
			
		}
	}
		//録音データ(音声信号)のコピー（出力バッファー1にコピー）
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
	

	//再生用バッファの準備完了の通知
	if (SupportASIOOutputReady == true) {
		asioresult = ASIOOutputReady();
		if (asioresult != ASE_OK) {
			MessageBox(NULL, _T("ASIOOutputReady"), _T("Error"), MB_ICONSTOP | MB_OK);
		}
	}
}

// 遅延時間[ms]を表示
bool SendMarginTimeToEdit(std::chrono::system_clock::duration time) {

	auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
	std::string string_msec = std::to_string(msec);
	std::string b = _T("時間差[ms]: ");
	string_msec = b + string_msec;
	const char* str_msec = string_msec.c_str();
	SendNowTimeToEdit(hEdit1, str_msec);
	

	return true;
}

// 時間差を配列に代入していくための関数
bool GetMarginTime(std::chrono::system_clock::duration time) {

	auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
	string stringTime = to_string(msec);
	MarginTime.push_back(stringTime);

	return true;
}

// 得られた時間差をcsvファイルに書き込むための関数
bool WriteToCSV(HWND hwnd, vector<string>& MarginTime, const string& filename) {
	char Temp[MAX_PATH];
	_stprintf_s(Temp, MAX_PATH,
		_T("以下のファイルに書き込みます。よろしいですか？\r\n%s"),
		filename.c_str());

	// MargintTimeが空の場合
	if (MarginTime.empty()) {
		MessageBox(hwnd, _T("書き込むものがありません。"), NULL, MB_OK);
		return true;
	}
	else if (MessageBox(hwnd, Temp, _T("出力先ファイルの確認"), MB_YESNO) == IDYES){
	// 出力ファイルストリームオブジェクトを作成
	// ファイルが存在しない場合は新たにファイルを作成、存在する場合はファイルの末尾に書き込み
		ofstream file(filename, ios::app);
		// カンマ区切りでストリームに送る
		for (size_t i = 0; i < MarginTime.size(); ++i) {
			file << MarginTime[i];
			if (i != MarginTime.size() - 1) {
				file << ",";
			}
		}
		file << "\n";
		file.close();

		// ベクターの中身の削除
		MarginTime.clear();
		// 書き込み確認
		MessageBox(hwnd, _T("ファイルに書き込みました。"), _T("出力確認"), MB_OK);
		//SetWindowText(hwnd, filename.c_str());
		// 押下回数の更新
		CountButtonClicked = 0;
		SetWindowText(GetDlgItem(hwnd, ID_STATIC_COUNTBUTTONCLICKED_2), _T("0"));
		// エディットボックスの表示内容をクリア
		SetWindowText(hEdit1, _T(""));

	}
	
	return true;
}

// ベクターの中身を削除
bool ClearMarginTime(HWND hwnd, vector<string>& MarginTime) {

	// ベクターの中身の削除
	if (!MarginTime.empty()) {
		MarginTime.clear();
		// ボタンの押下回数の更新
		CountButtonClicked = 0;
		SetWindowText(GetDlgItem(hwnd, ID_STATIC_COUNTBUTTONCLICKED_2), _T("0"));
		// エディットボックスの表示内容をクリア
		SetWindowText(hEdit1, _T(""));
	}
	else
	{
		MessageBox(hwnd, _T("配列には何も入ってません。"), NULL, MB_OK);
	}
	return true;
}

// ボタンの押下回数の更新
bool SendCountButtonClicked(HWND hStatic, int& CountButtonClicked) {
	// ボタンの押下回数の送信
	CountButtonClicked++;
	string CountString = to_string(CountButtonClicked);
	SetWindowText(hStatic, CountString.c_str());
	return true;
}

// エディットボックスに時刻を表示
bool SendNowTimeToEdit(HWND hwndEdit, const char* timeStr) {

	// エディットボックス内の現在の文字列を取得
	int length = GetWindowTextLength(hwndEdit);
	char* existingText = new char[length + 1];
	GetWindowText(hwndEdit, existingText, length + 1);

	
	char* newText = new char[length + strlen(timeStr) + 3];
	sprintf(newText, "%s\r\n%s", existingText, timeStr);

	// 新しくエディットボックスに文字をセット
	SetWindowText(hwndEdit, newText);

	// 後始末
	delete[] existingText;
	delete[] newText;

	return true;
}

// 現在時刻をミリ秒まで取得
const char* getMilliTime(std::chrono::system_clock::time_point now) {
	// 現在の時刻を取得
	//auto now = std::chrono::system_clock::now();

	// エポックからの経過時間を秒とミリ秒に分割
	auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now - seconds);

	// std::time_tに変換
	std::time_t tt = std::chrono::system_clock::to_time_t(seconds);

	// 時刻をローカルタイムに変換
	std::tm* tm = std::localtime(&tt);

	// 指定された形式で出力
	std::ostringstream oss;
	oss << std::put_time(tm, "%H:%M:%S") << '.'
		<< std::setfill('0') << std::setw(3) << milliseconds.count();

	// 結果をstatic std::stringに変換
	static std::string str, a;
	
	str = oss.str();
	// グローバル変数に時刻を記憶（csvファイルに書き込む際に必要)
	TimeStampButtonClicked = str;
	a = _T("ボタン押下時刻: ");
	// 結果のconst char*を返す
	str = a + str;
	return str.c_str();
}




//ASIOデバイスのバッファ処理時のタイムスタンプ用コールバック関数
ASIOTime* bufferswitchtimeinfo(ASIOTime* params, long doubleBufferIndex, ASIOBool directProcess)
{
	
	return(0L);
}

//ASIOデバイスのサンプリング周波数変更検出用コールバック関数
void sampleratedidchange(ASIOSampleRate sRate)
{
	return;
}

//ASIOデバイスからのメッセージの処理用コールバック関数
long asiomessage(long selector, long value, void* message, double* opt)
{
	return(0L);
}



// NumberOfloopsの表示
bool DispNumberOfloops() {

	return true;
}
// フォントの設定
bool OnFont(HWND hwnd) {

	hFont1 = CreateFont(25, 0, 0, 0, FW_MEDIUM, false, false, false, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, (VARIABLE_PITCH | FF_DONTCARE), _T("メイリオ"));

	hFont2 = CreateFont(22, 0, 0, 0, FW_MEDIUM, false, false, false, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, (VARIABLE_PITCH | FF_DONTCARE), _T("メイリオ"));

	hFont3 = CreateFont(45, 0, 0, 0, FW_BOLD, false, false, false, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, (VARIABLE_PITCH | FF_DONTCARE), _T("メイリオ"));

	SendMessage(hEdit1, WM_SETFONT, (WPARAM)hFont1, MAKELPARAM(false, 0));
	SendMessage(hStaticTime, WM_SETFONT, (WPARAM)hFont1, MAKELPARAM(false, 0));
	// ステティックコントロール
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
	
	// コンボボックス
	SendMessage(GetDlgItem(hwnd, ID_LATEINI), WM_SETFONT, (WPARAM)hFont1, MAKELPARAM(false, 0));
	SendMessage(GetDlgItem(hwnd, ID_LATESETTING), WM_SETFONT, (WPARAM)hFont1, MAKELPARAM(false, 0));
	// プッシュボタン
	SendMessage(GetDlgItem(hwnd, ID_BUTTON_CSV), WM_SETFONT, (WPARAM)hFont1, MAKELPARAM(false, 0));
	return true;
}



// コンボボックスにフォーカスが来た時
bool LateIniFunc(HWND hWnd, WPARAM wParam) {

		//コンボボックスで現在選択されている項目のインデックスを取得
		GetNowComboStr(hWnd);
		// NumberOfloopsを計算
		CalcLateNumberOfloops(hWnd, &NumberOfloops, Num, lenBuffer, FS, inlatency, outlatency);
		// NumberOfloopsをstd::string型に変換
		string stringnum = to_string(NumberOfloops);
		// 画面上に表示
		SetWindowText(hStaticNumberOfloops2, stringnum.c_str());
		// グローバル変数にNumberOfLoopsの値を記憶させる
		TempNumberOfLoops = NumberOfloops;

	return true;
}

// コンボボックス1で選択されたキー名に対応した遅延時間をコンボボックスに詰める。
bool SendLate2Combo(HWND hwnd, WPARAM wParam) {

	
		//コンボボックスで現在選択されている項目のインデックスを取得
		int intCurrentIndex = SendMessage(GetDlgItem(hwnd, (int)ID_LATEINI), CB_GETCURSEL, 0, 0);

		// 現在選択されている項目のの文字列の長さを取得
		int intTxtLen = SendMessage(GetDlgItem(hwnd, (int)ID_LATEINI), CB_GETLBTEXTLEN, intCurrentIndex, 0);

		if (intTxtLen != CB_ERR){
			char* pszBuf = new char[intTxtLen + 1];
			if (SendMessage(GetDlgItem(hwnd, ID_LATEINI), CB_GETLBTEXT, intCurrentIndex, (LPARAM)pszBuf) != CB_ERR) {
				char Path[MAX_PATH + 1];
				char settingpath[MAX_PATH + 1];
				if (GetModuleFileName(NULL, Path, MAX_PATH) != 0) {
					char drive[MAX_PATH + 1], dir[MAX_PATH + 1], fname[MAX_PATH + 1], ext[MAX_PATH + 1];
					// パス名を分解
					_splitpath(Path, drive, dir, fname, ext);
					_stprintf_s(settingpath, MAX_PATH + 1, _T("%s%ssetting.ini"), drive, dir);
				}
				char latedata[256];
				// iniファイルの読み込み
				GetPrivateProfileString(pszBuf, _T("data"), _T("Error"), latedata, sizeof(latedata), settingpath);

				// コンボボックスの中身を消去
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
					// コンボボックスに文字列を挿入
					SendMessage(GetDlgItem(hwnd, ID_LATESETTING), CB_INSERTSTRING, i, (LPARAM)cstr);
					i++;
				}
				// コンボボックスに先頭の要素をセット
				SendMessage(GetDlgItem(hwnd, ID_LATESETTING), CB_SETCURSEL, 0, 0);
			}
			
			delete[] pszBuf;
		}
		
	

	return true;
}

// コンボボックスで現在選択されている項目の文字列を取得する関数
bool GetNowComboStr(HWND hWnd) {

	//コンボボックスで現在選択されている項目のインデックスを取得
	int intCurrentIndex = SendMessage(GetDlgItem(hWnd, (int)ID_LATESETTING), CB_GETCURSEL, 0, 0);

	// コンボボックスの現在選択されている項目の文字列の長さを取得
	int intTxtLen = SendMessage(GetDlgItem(hWnd, (int)ID_LATESETTING), CB_GETLBTEXTLEN, intCurrentIndex, 0);

	if (intTxtLen != CB_ERR) {
		char* pszBuf = new char[intTxtLen + 1];
		// コンボボックスの一覧から選択した項目の文字列を取得
		if (SendMessage(GetDlgItem(hWnd, (int)ID_LATESETTING), CB_GETLBTEXT, intCurrentIndex, (LPARAM)pszBuf) != CB_ERR) {
			// char*型をint型に変換
			Num = atoi(pszBuf);
		}

		delete[] pszBuf;
	}
	return true;
}


// コンボボックスの選択項目から遅延時間を算出
bool CalcLateNumberOfloops(HWND hwnd, int* NumberOfloops, int  Num, int lenbuffer, int rate, long inlatency, long outlatency){
	/* 引数: ウィンドウハンドル, コンボボックスから得る文字列, バッファ長, サンプリング周波数, 
	*          インプットレイテンシ, アウトプットレイテンシ
	*/
	////////////////////////
	// 変数をms単位に変換
	//////////////////////////
	TCHAR NumberOfloopsInfo[LENSTR];
	// 入力レイテンシー（バッファ2個分の時間を減算）
	double inlatency_ms = (double)inlatency * 1000.0 / (double)rate - (double)lenbuffer * 2.0 * 1000.0 / (double)rate;
	// 出力レイテンシー
	double outlatency_ms = (double)outlatency * 1000.0 / (double)rate;
	// パルス波発生回路での遅延
	double kairo = (double)KAIROLATE;

	// NumberOfloopsの計算
	*NumberOfloops = ((double)Num - (inlatency_ms + outlatency_ms + kairo)) * (double)rate / ((double)lenbuffer * 1000.0);

	////メッセージボックスへの出力
	//_stprintf_s(NumberOfloopsInfo, LENSTR,
	//	_T("入力レイテンシ: %f [ms]\n"
	//	"出力レイテンシ: %f [ms]\n"
	//	"入出力レイテンシ: %f [ms]\n"
	//	"遅延させたい時間: %d [ms]\n"
	//	"回路による遅延: %f [ms]\n"
	//	"バッファ長: %d [points]\n"
	//	"Fs: %d [Hz]\n"
	//	"NumberOfloops = %d\n"
	//	"NumberOfloops(double) = %f"),
	//	inlatency_ms, outlatency_ms, inlatency_ms + outlatency_ms, Num, kairo, lenbuffer, rate, *NumberOfloops,
	//	((double)Num - (inlatency_ms + outlatency_ms + kairo)) * (double)rate / ((double)lenbuffer * 1000.0));

	//MessageBox(hwnd, NumberOfloopsInfo, _T("計算結果"), MB_OK);

	return true;

}

// エディットボックスを自動的に下までスクロールする
bool ScrollToBottom(HWND hEditBox) {
	// エディットボックス内の文字列の長さを取得
	int len = GetWindowTextLength(hEditBox);
	// 何も選択せずにエディットボックス内のテキストの最後にキャレットを移動
	SendMessage(hEditBox, EM_SETSEL, (WPARAM)len, (LPARAM)len);
	// エディットボックスをカーソルまでスクロールする
	SendMessage(hEditBox, EM_SCROLLCARET, 0, 0);

	return true;
}

// 被験者情報をベクタに保存
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
		//ASIO情報の表示
		MessageBox(NULL, strASIOInfo, _T("ASIO"), MB_OK);
		break;

	case ID_WAVEFILEINFORMATION:
		//Waveファイル情報の表示
		MessageBox(NULL, strWaveFileInfo, _T("Wave File"), MB_OK);
		break;

	case ID_EXIT:
		//プログラムの終了
		if (IDYES == MessageBox(hwnd, _T("終了しますか？"), _T("終了確認"), MB_YESNO)) {
			DestroyWindow(hwnd);
		}
		break;
		
		// 遅延時間の設定
		// 配列の削除
	case ID_MENU_RESTART:
		if (MessageBox(hwnd, _T("結果をファイルに書き込みましたか？"), _T("確認"), MB_YESNO) == IDYES) {
			if (ClearMarginTime(hwnd, MarginTime)) {
				MessageBox(hwnd, _T("結果を削除しました。"), _T("削除完了"), MB_OK);
			}
		}
		break;
	case ID_DELETE_ARRAY:
		/*NumberOfloops = 1;
		SetWindowText(hStaticNumberOfloops2, _T("1"));*/
		if (ClearMarginTime(hwnd, MarginTime)) {
			MessageBox(hwnd, _T("結果を削除しました。"), _T("削除完了"), MB_OK);
		}
		break;
		///////////////////
		// コンボボックス
		///////////////////
	case ID_LATEINI:
		// 基本情報
		if (HIWORD(wparam) == CBN_SELCHANGE) {
			SendLate2Combo(hwnd, wparam);
		}
		break;
		
	case ID_LATESETTING:
		// 詳細情報
		if (HIWORD(wparam) == CBN_SELCHANGE) {
			LateIniFunc(hwnd, wparam);
		}
		break;
	// 取得回数の設定
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
		// 実験方法の選択
	case ID_STATIC_NORMALORIRREG:
		if (HIWORD(wparam) == CBN_SELCHANGE) {
			int itemIndex = SendMessage(GetDlgItem(hwnd, ID_STATIC_NORMALORIRREG), CB_GETCURSEL, 0, 0);
			if (itemIndex != CB_ERR) {
				if (itemIndex) {
					if (DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MYDIALOG), hwnd, DialogProc) == -1) {
						MessageBox(hwnd, _T("ダイアログボックスの作成に失敗しました。"), _T("エラー"), MB_OK);
					}
					LABNormal = false;
				}
				else LABNormal = true;
			}
		}
		break;
		// BPMの選択
	case ID_STATIC_BPMNUM:
		if (HIWORD(wparam) == CBN_SELCHANGE) {
			int itemIndex = SendMessage(GetDlgItem(hwnd, ID_STATIC_BPMNUM), CB_GETCURSEL, 0, 0);
			if (itemIndex != CB_ERR) {
				/*BPMの変更に伴う処理をここに書く*/
			}
		}
		break;
	////////////////////////////////
	// 「メニュー」→「実験方法」
	// /////////////////////////////
	case ID_MENU_NORMAL:
		SetWindowText(GetDlgItem(hParentWindow, ID_STATIC_NORMALORIRREG), _T("通常"));
		LABNormal = true;
		break;
	case ID_MENU_IRREG:
		SetWindowText(GetDlgItem(hParentWindow, ID_STATIC_NORMALORIRREG), _T("変則"));
		LABNormal = false;
		break;
	///////////////////////////
	// 「メニュー」→「設定」
	///////////////////////////
	// BPMの選択（BPMによって結果の出力先を変更する)
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
	// 試行回数
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
	// ファイルの指定
		////////////////
	case ID_MENU_CSV_OUTPUT:
	{
		// ファイル選択用ダイアログの表示
		SelectFile(hwnd, FileNameCSV);
		// パス名をstring型に変換
		CSVFILENAME = FileNameCSV;
		// 出力先ファイルをウィンドウ上に表示
		RelativepathFromAbsolutepath(FileNameCSV, hwnd);
	}
		break;
		// プッシュボタン
	case ID_BUTTON_CSV:
		if (HIWORD(wparam) == BN_CLICKED) {
			
			// ファイル選択用ダイアログの表示
			SelectFile(hwnd, FileNameCSV);
			// 既に開かれているかをチェック
			if (!CheckCanWriteFile(hwnd, FileNameCSV)) {
				// ファイルを開くのに失敗した場合、書き込みが不可能である可能性が高い
				MessageBox(hwnd, _T("ファイルに書き込めません。既に開かれている可能性があります。\r\nファイルを閉じてから再度ボタンを押してください。"), _T("警告"), MB_OK | MB_ICONWARNING);
				break;
			}
			// パス名をstring型に変換
			CSVFILENAME = FileNameCSV;
			// 出力先ファイルをウィンドウ上に表示
			RelativepathFromAbsolutepath(FileNameCSV, hwnd);
			// 値の書き込み
			GetEditBoxTextFunc(hParentWindow);                                            // 被験者情報をベクタの先頭に代入 
			MarginTime.insert(MarginTime.begin(), to_string(Num));                 // 遅延時間をベクタの先頭に代入
			MarginTime.insert(MarginTime.begin(), TimeStampButtonClicked);   // 押下時刻をベクタの先頭に代入
			if (!LABNormal) {
				string tempunique = _T("unique");
				string stringDelayTiming = to_string(DelayTiming);
				string stringDelayTime_ms = to_string(DelayTime_ms);
				// 実験条件が変則の場合、ベクタの先頭に文字列を代入
				MarginTime.insert(MarginTime.begin(), stringDelayTiming);        // 遅延のタイミング
				MarginTime.insert(MarginTime.begin(), stringDelayTime_ms);    // 遅延時間
				MarginTime.insert(MarginTime.begin(), tempunique);                // unique
			}
			WriteToCSV(hwnd, MarginTime, CSVFILENAME);
		}
		break;
	}

	return true;
}

#pragma once
// ソース.cppとwindow.cppで使用
// 定数

#define FS				48000	//ASIOデバイスのサンプリング周波数[Hz]
#define MAXNUMINCHS		2		//ASIOデバイスの録音用チャンネル数
#define MAXNUMOUTCHS	2		//ASIOデバイスの再生用チャンネル数
#define LENDEVNAME		32		//ASIOデバイスの名前の最大の長さ
#define MAXNUMDEVS		16		//ASIOデバイスの最大数
#define NUMASIOBUFFERS	2		//ASIOデバイスのバッファ数
#define THRESH			0.1		//トリガー用しきい値(0.0～1.0)
#define CHCLICK			2		//クリック信号受信チャンネル(1から始まるインデックス)
#define CHSOUND			1		//音声信号受信チャンネル(1から始まるインデックス)
#define LENCHATTERING	50		//チャタリングの最長継続時間[ms]
#define LENSTR			1024	//文字列の最大長

#define IDDEV			2		//ASIOデバイスのID(「devnames」に格納されるデバイス名リストで要確認)
#define N 1               // waveデータをコピーするタイミング
#define KAIROLATE 4  // 回路で発生する遅延[ms]

#define WAVEFILENAME	_T("Accent08-1_OtoLogic_short.wav")
//#define WAVEFILENAME	_T("Accent08-1_OtoLogic.wav")
//#define WAVEFILENAME	_T("VSQSE_0527_kiran_02_VSQ.wav")
//#define WAVEFILENAME	_T("スイッチ05_On-Jin.wav")
//#define WAVEFILENAME	_T("決定、ボタン押下34_効果音ラボ.wav")


//#define CSVFILENAME _T("RESULT\\ResultCSV60.csv")
//#define CSVFILENAME _T("RESULT\\ResultCSV70.csv")
//#define CSVFILENAME _T("RESULT\\ResultCSV80.csv")
//#define CSVFILENAME _T("RESULT\\ResultCSV90.csv")
//#define CSVFILENAME _T("RESULT\\ResultCSV100.csv")
//#define CSVFILENAME _T("RESULT\\ResultCSV110.csv")
//#define CSVFILENAME _T("RESULT\\ResultDelay\\r200ms90bpm.csv")
#define	SHIKOUKAISUU 44 // ボタンの押下回数（ソース.cppで使用）
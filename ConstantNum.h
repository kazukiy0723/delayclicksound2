#pragma once

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

#define ID_STATIC1 2001
#define ID_STATIC2 2002
#define ID_STATIC_NUMBERDELAY1 2003
#define ID_STATIC_NUMBERDELAY2 2004


#define N 1               // waveデータをコピーするタイミング
#define GetMonitorRect(rc)  SystemParametersInfo(SPI_GETWORKAREA,0,rc,0)  
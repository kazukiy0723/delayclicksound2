#pragma once
// �\�[�X.cpp��window.cpp�Ŏg�p
// �萔

#define FS				48000	//ASIO�f�o�C�X�̃T���v�����O���g��[Hz]
#define MAXNUMINCHS		2		//ASIO�f�o�C�X�̘^���p�`�����l����
#define MAXNUMOUTCHS	2		//ASIO�f�o�C�X�̍Đ��p�`�����l����
#define LENDEVNAME		32		//ASIO�f�o�C�X�̖��O�̍ő�̒���
#define MAXNUMDEVS		16		//ASIO�f�o�C�X�̍ő吔
#define NUMASIOBUFFERS	2		//ASIO�f�o�C�X�̃o�b�t�@��
#define THRESH			0.1		//�g���K�[�p�������l(0.0�`1.0)
#define CHCLICK			2		//�N���b�N�M����M�`�����l��(1����n�܂�C���f�b�N�X)
#define CHSOUND			1		//�����M����M�`�����l��(1����n�܂�C���f�b�N�X)
#define LENCHATTERING	50		//�`���^�����O�̍Œ��p������[ms]
#define LENSTR			1024	//������̍ő咷

#define IDDEV			2		//ASIO�f�o�C�X��ID(�udevnames�v�Ɋi�[�����f�o�C�X�����X�g�ŗv�m�F)
#define N 1               // wave�f�[�^���R�s�[����^�C�~���O
#define KAIROLATE 4  // ��H�Ŕ�������x��[ms]

#define WAVEFILENAME	_T("Accent08-1_OtoLogic_short.wav")
//#define WAVEFILENAME	_T("Accent08-1_OtoLogic.wav")
//#define WAVEFILENAME	_T("VSQSE_0527_kiran_02_VSQ.wav")
//#define WAVEFILENAME	_T("�X�C�b�`05_On-Jin.wav")
//#define WAVEFILENAME	_T("����A�{�^������34_���ʉ����{.wav")


//#define CSVFILENAME _T("RESULT\\ResultCSV60.csv")
//#define CSVFILENAME _T("RESULT\\ResultCSV70.csv")
//#define CSVFILENAME _T("RESULT\\ResultCSV80.csv")
//#define CSVFILENAME _T("RESULT\\ResultCSV90.csv")
//#define CSVFILENAME _T("RESULT\\ResultCSV100.csv")
//#define CSVFILENAME _T("RESULT\\ResultCSV110.csv")
//#define CSVFILENAME _T("RESULT\\ResultDelay\\r200ms90bpm.csv")
#define	SHIKOUKAISUU 44 // �{�^���̉����񐔁i�\�[�X.cpp�Ŏg�p�j
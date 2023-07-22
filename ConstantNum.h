#pragma once

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

#define ID_STATIC1 2001
#define ID_STATIC2 2002
#define ID_STATIC_NUMBERDELAY1 2003
#define ID_STATIC_NUMBERDELAY2 2004


#define N 1               // wave�f�[�^���R�s�[����^�C�~���O
#define GetMonitorRect(rc)  SystemParametersInfo(SPI_GETWORKAREA,0,rc,0)  
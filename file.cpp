

#include<Windows.h>
#include<tchar.h>
#include<string>
#include<Shlwapi.h>
#include"file.h"
#include"window.h"

using namespace std;

#pragma comment(lib, "shlwapi.lib")

//////////////////////////////////////////////
// [���O��t���ĕۑ�]�_�C�A���O�{�b�N�X�̍쐬
//////////////////////////////////////////////
bool SelectFile(HWND hWnd, char* FileName) {

	static OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(ofn));
	memset(FileName, 0, sizeof(char) * MAX_PATH);

	// OPENFILENAME �\���̂̏�����
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	//ofn.lpstrFilter = _T("Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0\0"); 
	ofn.lpstrFilter = _T("CSV Files(*.csv)\0*.csv\0All Files(*.*)\0*.*\0\0");
	ofn.lpstrFile = FileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = _T("csv");
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
	ofn.lpstrTitle = _T("���O��t���ĕۑ�");

	if (!GetSaveFileName(&ofn)) {
		MessageBox(hWnd, _T("�t�@�C�����쐬�ł��܂���ł����B"), _T("�G���["), MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}

// �t�@�C�����������݉\�����`�F�b�N
bool CheckCanWriteFile(HWND hWnd, char* FileName) {
	HANDLE hFile = CreateFile(FileName, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return false;
	} else {
		CloseHandle(hFile);
	}
	return true;
}

// �o�͐�t�@�C�����΃p�X����J�����g�f�B���N�g������̑��΃p�X�ɕϊ�
bool RelativepathFromAbsolutepath(char* absolutePath, HWND hwnd) {

	// �J�����g�f�B���N�g���̎擾
	char currentDirectory[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDirectory);

	char relativePath[MAX_PATH];
	PathRelativePathTo(
		relativePath,
		currentDirectory,
		FILE_ATTRIBUTE_DIRECTORY,
		absolutePath,
		FILE_ATTRIBUTE_NORMAL
	);

	string temp = relativePath;
	// ".\\"���폜
	temp.erase(temp.begin() + 0, temp.begin() + 2);
	SetWindowText(GetDlgItem(hwnd, ID_EDIT_FILEPATH), temp.c_str());
	return true;
}

//////////////////////////////
// �_�C�A���O�{�b�N�X�̍쐬
//////////////////////////////

#include<windows.h>
#include<tchar.h>

// ����t�@�C��
#include"dialogbox.h"
#include"window.h"
#include"main.h"
#include"file.h"
#include"resource.h"

short temp1, temp2;

// �\�[�X.cpp�Ƌ��L
int DelayTiming = 1;
int DelayTime_ms = 10;

// �_�C�A���O�{�b�N�X�̃v���V�[�W��
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_INITDIALOG:
        temp1 = 0;
        temp2 = 0;
        SetDlgItemText(hwndDlg, IDC_EDIT1, _T("2"));
        SetDlgItemText(hwndDlg, IDC_EDIT2, _T("100"));
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)){
     
        case IDC_EDIT1:
            temp1 = 1;
            break;
        case IDC_EDIT2:
            temp2 = 1;
            break;
        case IDOK: // OK �{�^�����N���b�N���ꂽ�Ƃ�
        {
            // �x���̃^�C�~���O
            char* lpstringDelayTiming = new char[50];
            // �x������
            char* lpstringDelayTime = new char[50];
            GetWindowText(GetDlgItem(hwndDlg, IDC_EDIT1), lpstringDelayTiming, 50);
            GetWindowText(GetDlgItem(hwndDlg, IDC_EDIT2), lpstringDelayTime, 50);
            DelayTiming = atoi(lpstringDelayTiming);
            DelayTime_ms = atoi(lpstringDelayTime);

            delete[] lpstringDelayTiming;
            delete[] lpstringDelayTime;
        }
        EndDialog(hwndDlg, wParam);
        return TRUE;

        case IDCANCEL: // Cancel �{�^�����N���b�N���ꂽ�Ƃ�
            EndDialog(hwndDlg, wParam);
            return TRUE;

        }
    default:
        if (temp2) EnableWindow(GetDlgItem(hwndDlg, IDOK), TRUE);
        break;
    }
        return FALSE;
    
}


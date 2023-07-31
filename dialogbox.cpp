//////////////////////////////
// ダイアログボックスの作成
//////////////////////////////

#include<windows.h>
#include<tchar.h>

// 自作ファイル
#include"dialogbox.h"
#include"window.h"
#include"main.h"
#include"file.h"
#include"resource.h"

short temp1, temp2;

// ソース.cppと共有
int DelayTiming = 1;
int DelayTime_ms = 10;

// ダイアログボックスのプロシージャ
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
        case IDOK: // OK ボタンがクリックされたとき
        {
            // 遅延のタイミング
            char* lpstringDelayTiming = new char[50];
            // 遅延時間
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

        case IDCANCEL: // Cancel ボタンがクリックされたとき
            EndDialog(hwndDlg, wParam);
            return TRUE;

        }
    default:
        if (temp2) EnableWindow(GetDlgItem(hwndDlg, IDOK), TRUE);
        break;
    }
        return FALSE;
    
}


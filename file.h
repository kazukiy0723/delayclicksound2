#pragma once
/// file.cpp‚ÅŽg—p
#include<Windows.h>
#include<string>

using namespace std;





// Function
bool SelectFile(HWND, TCHAR*);
bool RelativepathFromAbsolutepath(char*, HWND);
bool CheckCanWriteFile(HWND, char*);
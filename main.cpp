/**********************************************************************************
 **    MCBEMouseLocker                                                           **
 **    Copyright (C) 2023 orbitgw <orbitgw@foxmail.com>                          **
 **                                                                              **
 **    This program is free software: you can redistribute it and/or modify      **
 **    it under the terms of the GNU General Public License as published by      **
 **    the Free Software Foundation, either version 3 of the License, or         **
 **    (at your option) any later version.                                       **
 **                                                                              **
 **    This program is distributed in the hope that it will be useful,           **
 **    but WITHOUT ANY WARRANTY; without even the implied warranty of            **
 **    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             **
 **    GNU General Public License for more details.                              **
 **                                                                              **
 **    You should have received a copy of the GNU General Public License         **
 **    along with this program.  If not, see <https://www.gnu.org/licenses/>.    **
 **********************************************************************************/
#ifndef UNICODE
#define UNICODE
#endif

#include "resource.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <shellapi.h>
#include <WinUser.h>
#include <tlhelp32.h>
#include <string>


#define WM_TRAY (WM_USER + 100)
#define WM_TASKBAR_CREATED RegisterWindowMessage(TEXT("TaskbarCreated"))

#define APP_NAME TEXT("MCBEMouseLocker")

HINSTANCE hInst;
NOTIFYICONDATA nid;
HMENU hMenu;

bool locker = true;

INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

char cWindowsDirectory[MAX_PATH];

LPTSTR cWinDir;
LPTSTR sConLin;

STARTUPINFO si = { sizeof(STARTUPINFO) };
PROCESS_INFORMATION pi;

void InitTray(HINSTANCE hInstance, HWND hWnd){

    hInst = hInstance;

    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hWnd;
    nid.uID = IDI_SMALL;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
    nid.uCallbackMessage = WM_TRAY;
    nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
    lstrcpy(nid.szTip, APP_NAME);

    hMenu = CreatePopupMenu();

    AppendMenu(hMenu, MF_STRING, IDM_SWITCH, TEXT("Turn on/off"));
    AppendMenu(hMenu, MF_STRING, IDM_ABOUT, TEXT("About"));
    AppendMenu(hMenu, MF_STRING, IDM_WEBSITE, TEXT("Website"));
    AppendMenu(hMenu, MF_STRING, IDM_EXIT, TEXT("Exit"));

    Shell_NotifyIcon(NIM_ADD, &nid);
}

void ShowTrayMsg(LPCWSTR text){
    lstrcpy(nid.szInfoTitle, APP_NAME);
    lstrcpy(nid.szInfo, text);
    nid.uTimeout = 1;
    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg){
        case WM_TRAY: {
            switch (lParam){
                case WM_RBUTTONDOWN: {
                    POINT pt; GetCursorPos(&pt);
                    SetForegroundWindow(hWnd);

                    int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD, pt.x, pt.y, NULL, hWnd, NULL);
                    

                    if (cmd == IDM_SWITCH) {
                        if (!locker) {
                            
                            CreateProcessW(nullptr, sConLin, nullptr, nullptr, false, CREATE_NEW_CONSOLE, nullptr, nullptr, &si, &pi);
                            locker = true;
                            ShowTrayMsg(L"Locker is on now");
                        }
                        else {
                            TerminateProcess(pi.hProcess,0);
                            locker = false;
                            ShowTrayMsg(L"Locker is off now");
                        }
                        
                    }
                    if (cmd == IDM_ABOUT)
                        DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                    if (cmd == IDM_WEBSITE)
                        ShellExecute(NULL, L"open", L"https://slimermc.com/MCBEMouseLocker/", NULL, NULL, SW_SHOW);
                    if (cmd == IDM_EXIT)
                        PostMessage(hWnd, WM_DESTROY, NULL, NULL);
                }break;
            }
        }break;
        case WM_DESTROY: {
            Shell_NotifyIcon(NIM_DELETE, &nid);
            PostQuitMessage(0);
        }break;
        case WM_TIMER: {
            ShowTrayMsg(TEXT("welcome"));
            KillTimer(hWnd, wParam);
        }break;
    }
    if (uMsg == WM_TASKBAR_CREATED){
        Shell_NotifyIcon(NIM_ADD, &nid);
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int iCmdShow){
    cWinDir = new TCHAR[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, cWinDir);
    sConLin = wcscat(cWinDir, L".\\locker.exe --launchedbymcbemlexe");

    HWND hWnd;
    MSG msg;
    WNDCLASSW wc = { 0 };
    wc.style = NULL;
    wc.hIcon = NULL;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = APP_NAME;
    wc.hCursor = NULL;

    if (!RegisterClass(&wc)) return 0;

    hWnd = CreateWindowEx(WS_EX_TOOLWINDOW, APP_NAME, APP_NAME, WS_POPUP, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    InitTray(hInstance, hWnd);		                 

    CreateProcessW(nullptr, sConLin, nullptr, nullptr, false, CREATE_NEW_CONSOLE, nullptr, nullptr, &si, &pi);

    while (GetMessage(&msg, NULL, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
    UNREFERENCED_PARAMETER(lParam);
    switch (message){
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL){
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

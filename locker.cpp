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

#include <windows.h>
#include <iostream>
#include <string>

void terminateOutput(){
	MessageBoxW(nullptr,
	TEXT("Unable to start the program using this executable file. Instead, use 'MCBEML. exe'"),
	TEXT("Error"),
	MB_ICONERROR | MB_OK);
}

HWND getHandler() {
	HWND hWnd = FindWindowW(nullptr, L"Minecraft");
	if (hWnd == nullptr) {
		if (MessageBoxW(nullptr, L"Cannot find the Minecraft Window", L"Error", MB_ICONERROR | MB_RETRYCANCEL) == 4) {
			hWnd = getHandler();
		}
	}
	return hWnd;
}

int main(int argc,char* argv[]){
	std::cout << "Subprocess for MCBEML.exe V0.0.1 \nCopyright (c) 2023 orbitgw" << std::endl;
	if(argc == 2){
		std::string secure_launch_check(argv[1]);
		if(secure_launch_check == "--launchedbymcbemlexe" || secure_launch_check == "--debug"){


			HWND hcWnd = GetForegroundWindow();
			ShowWindow(hcWnd, SW_HIDE);


			HWND hWnd = getHandler();
			if (hWnd == nullptr) {
				exit(-1);
			}
			while(1){
				
				CURSORINFO pci;

				pci.cbSize=sizeof(CURSORINFO);
				GetCursorInfo(&pci);

				RECT rect;
				GetWindowRect(hWnd, &rect);

				int x = (rect.left + rect.right) / 2;
				int y = (rect.top + rect.bottom) / 2;

				if (pci.flags == 0) {
					RECT rect = {};
					rect.left = x;
					rect.top = y;
					rect.right = x;
					rect.bottom = y;
					
					ClipCursor(&rect);
				}
				else {
					ClipCursor(nullptr);
				}
			}
		} else {
			terminateOutput();
			return -1;
		}
	} else {
		terminateOutput();
		return -1;
	}
	return 0;
}

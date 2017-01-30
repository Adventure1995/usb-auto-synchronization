/**
 *@auther: anonymous
 */
#include<iostream>
#include<windows.h>
#include<string>
#include<tchar.h>
#include<Strsafe.h>
#include<io.h>  
#include<fcntl.h>
#include<list>
#include<vector>
#include<shlwapi.h>
#include <math.h>
#include <dbt.h>

#include "FileSynManager.h"
#include "InitManager.h"

#pragma comment(lib, "shlwapi.lib")

//WCHAR usbDeviceRoot;

using namespace std;


list<wstring> relativePathList;
vector<wstring> filterList;
int basePathSize;

LRESULT CALLBACK WndProc(HWND h, UINT msg, WPARAM wp, LPARAM lp)
{
    if (msg == WM_DEVICECHANGE) {
        if ((DWORD)wp == DBT_DEVICEARRIVAL) {
            DEV_BROADCAST_VOLUME* p = (DEV_BROADCAST_VOLUME*) lp;
            if (p->dbcv_devicetype == DBT_DEVTYP_VOLUME) {
                int l = (int)(log(double(p->dbcv_unitmask)) / log(double(2)));
				LPWSTR usbDeviceRoot = new WCHAR;
				*usbDeviceRoot = 'A' + l;
				*(usbDeviceRoot + 1) = L'\0';
				LPWSTR desPath = new WCHAR[MAX_PATH];

				InitManager init;
				init.getConfigFileContent();
				init.configAnalyze();

				FileSynManager& manager = FileSynManager::Instance();
				manager.setSrcPath(init.getSrcFolderPath());
				StringCchCopy(desPath, MAX_PATH, usbDeviceRoot);
				StringCchCat(desPath, MAX_PATH, L":\\");
				StringCchCat(desPath, MAX_PATH, init.getDesFolderPath());
				wcout << desPath << endl;
				manager.setDesPath(desPath);
				manager.setfilterList(init.getFilterList());
				manager.FileSyn();
                //printf("啊……%c盘插进来了\n", 'A' + l);
            }
        } else if ((DWORD)wp == DBT_DEVICEREMOVECOMPLETE) {
            DEV_BROADCAST_VOLUME* p = (DEV_BROADCAST_VOLUME*) lp;
            if (p->dbcv_devicetype == DBT_DEVTYP_VOLUME) {
                int l = (int)(log(double(p->dbcv_unitmask)) / log(double(2)));
				//usbDeviceRoot = 'A' + l;
                printf("啊……%c盘被拔掉了\n", 'A' + l);
            }
        }
        return TRUE;
    } else return DefWindowProc(h, msg, wp, lp);
}

void InitConsole()  
{  
    int nRet= 0;  
    FILE* fp;  
    AllocConsole();  
    nRet= _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);  
    fp = _fdopen(nRet, "w");  
    *stdout = *fp;  
    setvbuf(stdout, NULL, _IONBF, 0);  
}

int _stdcall WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd) {
	InitConsole();
	filterList.push_back(L".sln");
	filterList.push_back(L".java");

	/*FileSynManager& manager = FileSynManager::Instance();
	manager.setSrcPath(L"E:\\Xieyuan\\uestc\\git-repos\\SimpleMusicPlayer");
	manager.setDesPath(L"D:\TDDOWNLOAD");
	manager.setfilterList(filterList);
	manager.FileSyn();*/


	WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.lpszClassName = TEXT("myusbmsg");
    wc.lpfnWndProc = WndProc;
     
    RegisterClass(&wc);
    HWND h = CreateWindow(TEXT("myusbmsg"), TEXT(""), 0, 0, 0, 0, 0,
        0, 0, GetModuleHandle(0), 0);
    MSG msg;
    while( GetMessage(&msg, 0, 0, 0) > 0 ) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

	/*USBManager usbManager;
	MSG msg;
	while( GetMessage(&msg, 0, 0, 0) > 0 ) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
		wcout << L"啊……" << usbManager.GetDeviceRoot() << L"盘插进来了" << endl;
    }*/
	
	system("pause");
	return 0;
}
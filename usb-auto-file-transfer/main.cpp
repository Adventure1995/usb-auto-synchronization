/**
 *@auther: anonymous
 */
#include<iostream>
#include<windows.h>
#include<string>
#include <tchar.h>
#include<Strsafe.h> 
#include<io.h>  
#include<fcntl.h>
#include<list>
#include<vector>
#include<shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

#define MAX_PATH 1024

using namespace std;


list<wstring> relativePathList;
vector<wstring> filterList;
int basePathSize;

bool isSynFile(LPCTSTR path);
bool FileSyn(LPWSTR src, LPWSTR dis);
void InitConsole();
void findFile(LPWSTR path);

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

void findFile(LPWSTR path) {

	WIN32_FIND_DATA FindFileData;  
    HANDLE hFind=INVALID_HANDLE_VALUE;  
    wchar_t DirSpec[MAX_PATH];                  //定义要遍历的文件夹的目录  
    DWORD dwError;  
    StringCchCopy(DirSpec, MAX_PATH, path);  
    StringCchCat(DirSpec, MAX_PATH, TEXT("\\*"));   //定义要遍历的文件夹的完整路径\*  
	
	hFind=FindFirstFile(DirSpec, &FindFileData);          //找到文件夹中的第一个文件  
    if (hFind == INVALID_HANDLE_VALUE) {                               
		//如果hFind句柄创建失败，输出错误信息   
        FindClose(hFind);   
        return;    
    } else {  
        while (FindNextFile(hFind, &FindFileData) != 0) {                           
			//当文件或者文件夹存在时  
            if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0
				&& wcscmp(FindFileData.cFileName, L".") == 0
				|| wcscmp(FindFileData.cFileName, L"..") == 0) {      
				//判断是文件夹&&表示为"."||表示为"."  
				//排除. 和 ..
                 continue;  
            }  
            if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
				//判断如果是文件夹  
				LPWSTR DirAdd =  new WCHAR[MAX_PATH];  
                //StringCchCopy(DirAdd, MAX_PATH, path);
				StringCchCopy(DirAdd, MAX_PATH, path);
				StringCchCat(DirAdd, MAX_PATH, TEXT("\\"));  
                StringCchCat(DirAdd, MAX_PATH, FindFileData.cFileName);       //拼接得到此文件夹的完整路径
				findFile(DirAdd);                                  //实现递归调用  
            }  
            if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)    //如果不是文件夹  
            {  
                //wcout << path << "\\" << FindFileData.cFileName << endl;            //输出完整路径
				//relativePathList.assign(FindFileData.cFileName);
				WCHAR* name = FindFileData.cFileName;
				wstring ws_name(name);
				wstring ws_path(path);
				wstring ws_abslutePath;
				ws_abslutePath += ws_path + ws_name;
				//string s_abslutePath(ws_abslutePath.begin(), ws_abslutePath.end());
				if (!isSynFile(ws_abslutePath.c_str()))
					continue;
				wstring ws_relativePath = ws_abslutePath.substr(basePathSize, ws_abslutePath.size());
				relativePathList.push_back(ws_relativePath);
            }  
        }  
        FindClose(hFind);  
    }  
}

bool isSynFile(LPCTSTR path) {
	int i = 0;
	for (i = 0; i < filterList.size(); i++) {
		wstring tmp = filterList[i];
		LPTSTR tag = PathFindExtension(path);
		wstring ws_tag(tag);
		if (tmp == tag)
			return true;
	}
	return false;
}

bool FileSyn(LPWSTR src, LPWSTR dis) {
	/**
	 *参数：
	 *src 需要复制的文件夹路径
	 *dis 目标文件夹路径
	 */
	findFile(src);

	return false;
}

int _stdcall WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd) {
	InitConsole();
	filterList.push_back(L".sln");
	filterList.push_back(L".java");
	LPWSTR src = L"E:\\Xieyuan\\uestc\\git-repos\\SimpleMusicPlayer";
	wstring ws_src(src);
	basePathSize = ws_src.size();
	FileSyn(src, L"D:\TDDOWNLOAD");
//	cout << MoveFile(L"E:\\tmp\\html\\html\\admin_deviceOP.php", L"E:\\tmp\\test") << endl;
	if (!CopyFile(L"E:\\tmp\\html\\html\\admin_deviceOP.php", L"E:\\tmp\\test", FALSE)) {
		wcout << GetLastError() << endl;
	}
	while (!relativePathList.empty())
	{
		wcout << relativePathList.back() << endl;
		relativePathList.pop_back();
	}
	system("pause");
	return 0;
}
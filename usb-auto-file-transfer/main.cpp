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
    wchar_t DirSpec[MAX_PATH];                  //����Ҫ�������ļ��е�Ŀ¼  
    DWORD dwError;  
    StringCchCopy(DirSpec, MAX_PATH, path);  
    StringCchCat(DirSpec, MAX_PATH, TEXT("\\*"));   //����Ҫ�������ļ��е�����·��\*  
	
	hFind=FindFirstFile(DirSpec, &FindFileData);          //�ҵ��ļ����еĵ�һ���ļ�  
    if (hFind == INVALID_HANDLE_VALUE) {                               
		//���hFind�������ʧ�ܣ����������Ϣ   
        FindClose(hFind);   
        return;    
    } else {  
        while (FindNextFile(hFind, &FindFileData) != 0) {                           
			//���ļ������ļ��д���ʱ  
            if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0
				&& wcscmp(FindFileData.cFileName, L".") == 0
				|| wcscmp(FindFileData.cFileName, L"..") == 0) {      
				//�ж����ļ���&&��ʾΪ"."||��ʾΪ"."  
				//�ų�. �� ..
                 continue;  
            }  
            if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
				//�ж�������ļ���  
				LPWSTR DirAdd =  new WCHAR[MAX_PATH];  
                //StringCchCopy(DirAdd, MAX_PATH, path);
				StringCchCopy(DirAdd, MAX_PATH, path);
				StringCchCat(DirAdd, MAX_PATH, TEXT("\\"));  
                StringCchCat(DirAdd, MAX_PATH, FindFileData.cFileName);       //ƴ�ӵõ����ļ��е�����·��
				findFile(DirAdd);                                  //ʵ�ֵݹ����  
            }  
            if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)    //��������ļ���  
            {  
                //wcout << path << "\\" << FindFileData.cFileName << endl;            //�������·��
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
	 *������
	 *src ��Ҫ���Ƶ��ļ���·��
	 *dis Ŀ���ļ���·��
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
#include "FileSynManager.h"
#include<shlwapi.h>
#include<string>
#include<Strsafe.h>
#include<iostream>
#include<shlwapi.h>
#include<Shlobj.h>

#include "FileSynManager.h"

#pragma comment(lib, "shlwapi.lib")

FileSynManager& FileSynManager::Instance() {
	if (instance_ == NULL)
            {
                instance_ = new FileSynManager();
            }
            return *instance_;
}

FileSynManager::FileSynManager() {
	srcPath = NULL;
	desPath = NULL;
	absluteRootLength = 0;
}

FileSynManager::~FileSynManager() {
}



void FileSynManager::setfilterList(vector<wstring> filterList) {
	this->filterList = filterList;
}

bool FileSynManager::isSynFile(LPCTSTR path) {
	int i = 0;
	for (i = 0; i < filterList.size(); i++) {
		wstring tmp = filterList[i];
		LPTSTR tag = PathFindExtension(path);
		wstring ws_tag(tag);
		if (tmp == ws_tag)
			return true;
	}
	return false;
}

bool FileSynManager::FileSyn() {

	findFile(srcPath);

	while (!relativePathList.empty())
	{
		//wcout << relativePathList.back() << endl;
		//relativePathList.pop_back();
		LPWSTR abslutePath = new WCHAR[MAX_PATH];
		StringCchCopy(abslutePath, MAX_PATH, desPath);
		StringCchCat(abslutePath, MAX_PATH, L"\\");
		StringCchCat(abslutePath, MAX_PATH, relativePathList.back().c_str());
		relativePathList.pop_back();
		wstring name(PathFindFileName(srcAbslutePathList.back().c_str()));
		wstring ws_name(name);
		wstring ws_newFolder(abslutePath);
		ws_newFolder = ws_newFolder.substr(0, ws_newFolder.size() - name.size());
		SHCreateDirectoryEx(NULL, ws_newFolder.c_str(), NULL);
		if (!CopyFile(srcAbslutePathList.back().c_str(), abslutePath, false)) {
			cout << GetLastError() << endl;
		}
		srcAbslutePathList.pop_back();
	}

	return false;
}

void FileSynManager::findFile(LPWSTR path) {
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
				WCHAR* name = FindFileData.cFileName;
				wstring ws_name(name);
				wstring ws_path(path);
				wstring ws_abslutePath(ws_path + L"\\" + ws_name);//输出完整路径
				if (!isSynFile(ws_abslutePath.c_str()))
					continue;
				srcAbslutePathList.push_back(ws_abslutePath);
				wstring ws_relativePath = ws_abslutePath.substr(absluteRootLength + 1, ws_abslutePath.size());
				relativePathList.push_back(ws_relativePath);
            }  
        }  
        FindClose(hFind);  
    }  
}

void FileSynManager::setSrcPath(LPWSTR srcPath) {
	this->srcPath = srcPath;
	wstring tmp(srcPath);
	absluteRootLength = tmp.size();
}

void FileSynManager::setDesPath(LPWSTR desPath) {
	this->desPath = desPath;
}

FileSynManager* FileSynManager::instance_;
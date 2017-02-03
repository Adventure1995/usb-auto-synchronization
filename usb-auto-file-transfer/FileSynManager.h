#pragma once

#include<windows.h>
#include<list>
#include<vector>
#include<shlwapi.h>
#include<string>
#include<Strsafe.h>
#include<iostream>
#include<Shlobj.h>

#include "FileFilter.h"
#include "extentionFilter.h"

#define MAX_PATH 1024

using namespace std;

class FileSynManager
{
public:
	
	static FileSynManager& Instance();

	void addFilterList(FileFilter* filter);
	void setSrcPath(LPWSTR srcPath);
	void setDesPath(LPWSTR DesPath);
	
	bool FileSyn();

private:

	FileSynManager();
	~FileSynManager();

	void findFile(LPWSTR path);
	bool isSynFile(LPCTSTR path);

	static FileSynManager* instance_;

	LPWSTR srcPath;
	LPWSTR desPath;
	int absluteRootLength;
	//vector<wstring> filterList;
	list<wstring> relativePathList;   //相对路径列表
	//list<wstring> srcAbslutePathList; //原路径绝对路径列表
	//list<wstring> desAbslutePathList; //目标路径绝对路径列表

	list<wstring> failedSynFileList; //同步失败文件列表
	list<FileFilter*> filterList;
};
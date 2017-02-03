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
	list<wstring> relativePathList;   //���·���б�
	//list<wstring> srcAbslutePathList; //ԭ·������·���б�
	//list<wstring> desAbslutePathList; //Ŀ��·������·���б�

	list<wstring> failedSynFileList; //ͬ��ʧ���ļ��б�
	list<FileFilter*> filterList;
};
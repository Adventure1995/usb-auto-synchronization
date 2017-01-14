#pragma once

#include<windows.h>
#include<list>
#include<vector>

#define MAX_PATH 1024

using namespace std;

class FileSynManager
{
public:
	
	static FileSynManager& Instance();

	void setfilterList(vector<wstring> filterList);
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
	vector<wstring> filterList;
	list<wstring> relativePathList;   //相对路径列表
	list<wstring> srcAbslutePathList; //原路径绝对路径列表
	list<wstring> desAbslutePathList; //目标路径绝对路径列表

	list<wstring> failedSynFileList; //同步失败文件列表

};
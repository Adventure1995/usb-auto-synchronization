#include "FileFilter.h"
#include <vector>
#include <windows.h>
#include<shlwapi.h>

#ifndef EXTENTIONFILTER_H
#define EXTENTIONFILTER_H

#pragma comment(lib, "shlwapi.lib")

class ExtentionFilter :  public FileFilter {

public:
	 bool filter(wstring srcFile, wstring desFile);

	 ExtentionFilter();
	 ~ExtentionFilter();

private:
	vector<wstring> filterList;
};

#endif
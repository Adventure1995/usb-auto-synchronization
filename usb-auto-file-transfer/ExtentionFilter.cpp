#include "ExtentionFilter.h"

bool ExtentionFilter::filter(wstring srcFile, wstring desFile) {
	int i = 0;
	for (i = 0; i < filterList.size(); i++) {
		wstring tmp = filterList[i];
		LPTSTR tag = PathFindExtension(srcFile.c_str());
		wstring ws_tag(tag);
		if (tmp == ws_tag)
			return true;
	}
	return false;
};

ExtentionFilter::ExtentionFilter(): FileFilter() {
	filterList.push_back(L".java");
	filterList.push_back(L".docx");
	filterList.push_back(L".mp4");
};

ExtentionFilter::~ExtentionFilter() {
};
#include "FileSynManager.h"
#include "ConsoleManager.h"

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



/*void FileSynManager::setfilterList(vector<wstring> filterList) {
	this->filterList = filterList;
}*/

void FileSynManager:: addFilterList(FileFilter* filter) {
	filterList.push_back(filter);
}

/*bool FileSynManager::isSynFile(LPCTSTR path) {
	int i = 0;
	for (i = 0; i < filterList.size(); i++) {
		wstring tmp = filterList[i];
		LPTSTR tag = PathFindExtension(path);
		wstring ws_tag(tag);
		if (tmp == ws_tag)
			return true;
	}
	return false;
}*/

bool FileSynManager::FileSyn() {

	findFile(srcPath);

	ConsoleManager::consoleInit();

	map<wstring, wstring> locateMap;

	list<wstring> srcSynList;

	while (!relativePathList.empty()) {
		LPWSTR srcAbslutePath = new WCHAR[MAX_PATH];
		StringCchCopy(srcAbslutePath, MAX_PATH, srcPath);
		StringCchCat(srcAbslutePath, MAX_PATH, L"\\");
		StringCchCat(srcAbslutePath, MAX_PATH, relativePathList.back().c_str());
		LPWSTR desAbslutePath = new WCHAR[MAX_PATH];
		StringCchCopy(desAbslutePath, MAX_PATH, desPath);
		StringCchCat(desAbslutePath, MAX_PATH, L"\\");
		StringCchCat(desAbslutePath, MAX_PATH, relativePathList.back().c_str());
		relativePathList.pop_back();
		wstring ws_name(PathFindFileName(srcAbslutePath));
		//wstring ws_name(name);
		wstring ws_newFolder(desAbslutePath);
		ws_newFolder = ws_newFolder.substr(0, ws_newFolder.size() - ws_name.size());
		
		bool isSynFile = true;
		for (list<FileFilter*>::iterator iterator = filterList.begin(); iterator != filterList.end(); ++iterator) {
			if ((*iterator)->filter(wstring(srcAbslutePath), wstring(desAbslutePath)) == false) {
				isSynFile = false;
				break;
			}
		}
		if (isSynFile == false)
			continue;
		srcSynList.push_back(srcAbslutePath);
		locateMap.insert(std::pair<wstring, wstring>(srcAbslutePath, desAbslutePath));
	}

	ConsoleManager::printAllFile(srcSynList);

	/*while (!relativePathList.empty())
	{
		LPWSTR srcAbslutePath = new WCHAR[MAX_PATH];
		StringCchCopy(srcAbslutePath, MAX_PATH, srcPath);
		StringCchCat(srcAbslutePath, MAX_PATH, L"\\");
		StringCchCat(srcAbslutePath, MAX_PATH, relativePathList.back().c_str());
		LPWSTR desAbslutePath = new WCHAR[MAX_PATH];
		StringCchCopy(desAbslutePath, MAX_PATH, desPath);
		StringCchCat(desAbslutePath, MAX_PATH, L"\\");
		StringCchCat(desAbslutePath, MAX_PATH, relativePathList.back().c_str());
		relativePathList.pop_back();
		wstring ws_name(PathFindFileName(srcAbslutePath));
		//wstring ws_name(name);
		wstring ws_newFolder(desAbslutePath);
		ws_newFolder = ws_newFolder.substr(0, ws_newFolder.size() - ws_name.size());
		
		bool isSynFile = true;
		for (list<FileFilter*>::iterator iterator = filterList.begin(); iterator != filterList.end(); ++iterator) {
			if ((*iterator)->filter(wstring(srcAbslutePath), wstring(desAbslutePath)) == false) {
				isSynFile = false;
				break;
			}
		}
		if (isSynFile == false)
			continue;

		SHCreateDirectoryEx(NULL, ws_newFolder.c_str(), NULL);
		if (!CopyFile(srcAbslutePath, desAbslutePath, false)) {
			cout << GetLastError() << endl;
		}
		//srcAbslutePathList.pop_back();
	}*/
	int count = 1;
	int num = 1;
	int total = srcSynList.size();
	while (!srcSynList.empty() && num++ <= total)  {
		wstring srcAbslutePath = srcSynList.back();
		wstring ws_name(PathFindFileName(srcAbslutePath.c_str()));
		wstring desAbslutePath = locateMap[srcAbslutePath];
		wstring ws_newFolder(desAbslutePath);
		ws_newFolder = ws_newFolder.substr(0, ws_newFolder.size() - ws_name.size());
		SHCreateDirectoryEx(NULL, ws_newFolder.c_str(), NULL);
		if (CopyFile(srcAbslutePath.c_str(), desAbslutePath.c_str(), false)) {
			//cout << GetLastError() << endl;
			ConsoleManager::printCopyInfo(count, total, srcAbslutePath, desAbslutePath, 0);
			count++;
		} else {
			//ConsoleManager::printCopyInfo(locate++, total, srcAbslutePath, desAbslutePath, 1);
			cout << GetLastError() << endl;
			failedSynFileList.push_back(srcAbslutePath);
		}
		srcSynList.pop_back();
		
	}

	while (!srcSynList.empty()) {
		failedSynFileList.push_back(srcSynList.back());
		srcSynList.pop_back();
	}

	if (failedSynFileList.size() == 0) {
		ConsoleManager::consoleClose(true);
	} else {
		while (!failedSynFileList.empty()) {
			wstring file = failedSynFileList.back();
			ConsoleManager::printCopyInfo(count++, total, file, locateMap[file], 1);
			failedSynFileList.pop_back();
		}
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
				//if (!isSynFile(ws_abslutePath.c_str()))
				//	continue;
				//srcAbslutePathList.push_back(ws_abslutePath);
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
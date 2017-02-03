#include "FileSynManager.h"

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

	while (!relativePathList.empty())
	{
		//wcout << relativePathList.back() << endl;
		//relativePathList.pop_back();
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
	}

	return false;
}

void FileSynManager::findFile(LPWSTR path) {
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
				WCHAR* name = FindFileData.cFileName;
				wstring ws_name(name);
				wstring ws_path(path);
				wstring ws_abslutePath(ws_path + L"\\" + ws_name);//�������·��
				//if (!isSynFile(ws_abslutePath.c_str()))
				//	continue;
				//srcAbslutePathList.push_back(ws_abslutePath);
				wstring ws_relativePath = ws_abslutePath.substr(absluteRootLength + 1, ws_abslutePath.size());
				wcout << ws_relativePath << endl;
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
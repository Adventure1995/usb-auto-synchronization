#include<windows.h>
#include<iostream>
#include <fstream>  
#include <string>
#include<Strsafe.h>

#include "InitManager.h"

using namespace std;

std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

bool InitManager::getConfigFileContent() {

	ifstream configStream("main.conf");  
    string filename;  
    string line;  
  
    if(configStream) {
		// 有该文件  
        while (getline (configStream, line)) // line中不包括每行的换行符  
        {   
            cout << line << endl;
			configContent.push_back(line);
        }
		return true;
    }
    else {
		// 没有该文件  
        cout << "没有找到配置文件，使用默认配置" << endl;
		return false;
    }  

}

bool InitManager::configAnalyze() {

	vector<wstring> tmpFilterList;
	wstring tmpSrcFolderPath;
	wstring tmpDesFolderPath;

	bool ifCorrect = true;

	while(!configContent.empty()) {
		string config = configContent.back();
		configContent.pop_back();
		if (config.substr(0,1) == "#" || config == "") {
			continue;
		}else if (config.substr(0, 6) == "filter") {
			string filter = config.substr(6);
			if(!(filter.substr(0, 1) == ".")) {
				ifCorrect  = false;
				break;
			}else {
				filter = filter.substr(1);
				if (filter.substr(0, 9) == "extension") {
					filter = filter.substr(9);
					if (!(filter.substr(0,1) == "=")) {
						ifCorrect  = false;
						break;
					} else {
						filter = filter.substr(1);
						wstring tmp = s2ws(filter);
						tmpFilterList.push_back(tmp);
					}
				} else if (filter.substr(0, 4) == "time") {
				} else {
					ifCorrect = false;
					break;
				}
			}
		}else if (config.substr(0, 7) == "desPath") {
			string desPath = config.substr(7);
			string tmp = "=?:\\\\";
			if (!(desPath.substr(0, 5) == tmp)) {
				ifCorrect  = false;
				break;
			} else {
				desPath = desPath.substr(5);
				tmpDesFolderPath = s2ws(desPath);
			}
		}else if (config.substr(0, 7) == "srcPath") {
			string srcPath = config.substr(7);
			if (!(srcPath.substr(0, 1) == "=")) {
				ifCorrect  = false;
				break;
			} else {
				srcPath = srcPath.substr(1);
				tmpSrcFolderPath = s2ws(srcPath);
			}
		} else {
			ifCorrect  = false;
			break;
		}
	}

	if (ifCorrect) {
	//	filterList = tmpFilterList;
		//srcFolderPath = tmpSrcFolderPath;
		srcFolderPath = new WCHAR[tmpSrcFolderPath.size() + 1];
		StringCchCopy(srcFolderPath, tmpSrcFolderPath.size() + 1, tmpSrcFolderPath.c_str());
		desFolderPath = new WCHAR[tmpDesFolderPath.size() + 1];
		StringCchCopy(desFolderPath, tmpDesFolderPath.size() + 1, tmpDesFolderPath.c_str());
		filterList = tmpFilterList;
		return true;
	} else {
		cout << "配置文件语法解析错误，使用默认配置" << endl;
		return false;
	}

}

vector<wstring> InitManager::getFilterList() {
	return filterList;
}
LPWSTR InitManager::getSrcFolderPath() {
	return srcFolderPath;
}
LPWSTR InitManager::getDesFolderPath() {
	return desFolderPath;
}

InitManager::InitManager() {
	srcFolderPath = L"C:\\Users\\downloads";
	desFolderPath = L"cpy";
	filterList.push_back(L".docx");
	filterList.push_back(L".pdf");
}

InitManager::~InitManager() {
}
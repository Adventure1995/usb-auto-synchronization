#include<windows.h>
#include<string>
#include<list>
#include<vector>

using namespace std;

#define MAX_CONFIG_LENGTH 65536

class InitManager
{

public: 
	InitManager();
	~InitManager();
	
	bool getConfigFileContent();
	bool configAnalyze();

	vector<wstring> getFilterList();
	LPWSTR getSrcFolderPath();
	LPWSTR getDesFolderPath();

private:

	vector<wstring> filterList;
	LPWSTR srcFolderPath;
	LPWSTR desFolderPath;
	list<string> configContent;
};
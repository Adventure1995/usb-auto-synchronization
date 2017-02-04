#include "ConsoleManager.h"
#include "Util.h"

ConsoleManager::ConsoleManager(){};

ConsoleManager::~ConsoleManager(){};

void ConsoleManager::consoleInit() {
	int nRet= 0;  
    FILE* fp;  
    AllocConsole();  
    nRet= _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);  
    fp = _fdopen(nRet, "w");  
    *stdout = *fp;  
    setvbuf(stdout, NULL, _IONBF, 0);

	cout << "---------------------------" << endl;
	cout << "|        u盘自动同步       |" << endl;
	cout << "|        版本号：0.8       |" << endl;
	cout << "---------------------------" << endl << endl;

};

void ConsoleManager::printCopyInfo(int num, int total, wstring src, wstring des, int state) {

	switch (state)
	{
	case 0:
		//文件不存在
		cout << "(" << num << "/" << total << ")  " << WstringToString(src) << endl;
		cout << "     ->" << WstringToString(des) << endl;
		cout << "   [success]" << endl;
		break;
	case 1:
		//文件相同
		cout << "(" << num << "/" << total << ")  " << WstringToString(src) << endl;
		cout << "     ->" << WstringToString(des) << endl;
		cout << "   [failed]" << endl;
		break;
	default:
		break;
	}
};

void ConsoleManager::printAllFile(list<wstring> fileList) {

	cout << "------即将开始复制文件------" << endl;
	cout << "--------所需同步文件--------" << endl << endl;
	while (!fileList.empty()) {
		string tmp;
		tmp = WstringToString(fileList.back());
		cout << tmp << endl;
		fileList.pop_back();
	}
	cout << "------开始复制,请等待------" << endl;

}

void ConsoleManager::consoleClose(bool flag){
	if (flag == true)
		FreeConsole();
	else
		return;
};
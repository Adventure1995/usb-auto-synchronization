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
	cout << "|        u���Զ�ͬ��       |" << endl;
	cout << "|        �汾�ţ�0.8       |" << endl;
	cout << "---------------------------" << endl << endl;

};

void ConsoleManager::printCopyInfo(int num, int total, wstring src, wstring des, int state) {

	switch (state)
	{
	case 0:
		//�ļ�������
		cout << "(" << num << "/" << total << ")  " << WstringToString(src) << endl;
		cout << "     ->" << WstringToString(des) << endl;
		cout << "   [success]" << endl;
		break;
	case 1:
		//�ļ���ͬ
		cout << "(" << num << "/" << total << ")  " << WstringToString(src) << endl;
		cout << "     ->" << WstringToString(des) << endl;
		cout << "   [failed]" << endl;
		break;
	default:
		break;
	}
};

void ConsoleManager::printAllFile(list<wstring> fileList) {

	cout << "------������ʼ�����ļ�------" << endl;
	cout << "--------����ͬ���ļ�--------" << endl << endl;
	while (!fileList.empty()) {
		string tmp;
		tmp = WstringToString(fileList.back());
		cout << tmp << endl;
		fileList.pop_back();
	}
	cout << "------��ʼ����,��ȴ�------" << endl;

}

void ConsoleManager::consoleClose(bool flag){
	if (flag == true)
		FreeConsole();
	else
		return;
};
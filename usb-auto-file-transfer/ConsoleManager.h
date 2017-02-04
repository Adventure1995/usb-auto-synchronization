#include<string>
#include<windows.h>
#include<io.h>
#include<iostream>
#include<fcntl.h>
#include<list>

using namespace std;

class ConsoleManager {

public:
	ConsoleManager();
	~ConsoleManager();

	static void printCopyInfo(int num, int total, wstring src, wstring des, int level);
	static void printAllFile(list<wstring> path);
	static void consoleInit();
	static void consoleClose(bool flag);
};
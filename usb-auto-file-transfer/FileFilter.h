#include<string>
#include<shlwapi.h>


#ifndef FILEFILTER_H
#define FILEFILTER_H
using namespace std;

class FileFilter {

	public:
      virtual bool filter(wstring srcFile, wstring desFile);
	  //virtual void extentionFilter();

	  FileFilter();
	  ~FileFilter();

};

#endif
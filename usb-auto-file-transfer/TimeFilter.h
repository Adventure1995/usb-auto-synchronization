#include "FileFilter.h"

#ifndef TIMEFILTER_H
#define TIMEFILTER_H

class TimeFilter : private FileFilter {

public:

	TimeFilter();
	~TimeFilter();

	bool filter(wstring file, wstring desFile);

};

#endif
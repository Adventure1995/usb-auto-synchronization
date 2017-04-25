#include "FileFilter.h"

#ifndef TIMEFILTER_H
#define TIMEFILTER_H

class TimeFilter : private FileFilter {

public:

	TimeFilter();
	~TimeFilter();

	list<wstring> filter(list<wstring> src);

};

#endif
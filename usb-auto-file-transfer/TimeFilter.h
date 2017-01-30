#include "FileFilter.h"

class TimeFilter : private FileFilter {

public:

	TimeFilter();
	~TimeFilter();

	bool filter(wstring file);

};
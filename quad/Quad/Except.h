#ifndef __EXCEPT__
#define __EXCEPT__

#include <exception>

class StopCond: public std::exception {
	public:
		StopCond(int ti): ti_(ti) {}
		int ti_;
};


#endif

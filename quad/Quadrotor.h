#ifndef __QUADROTOR__
#define __QUADROTOR__

//#include <Brain.h>
//#include <Telem.h>
//#include <Plant.h>

class Telem;
class Plant;
class Brain;

void product(int choices, int repeat, int*& arr, int level = 0);


class Quadrotor {
	public:
		Quadrotor(double dt, int N);
		void	run();
		void	write();
		

		double	dt_;
		int	N_;
		double*	t_;
		
		Telem*		telem_;
		Plant*		plant_;
		Brain*		brain_;

};


#endif






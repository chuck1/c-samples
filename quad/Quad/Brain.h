#ifndef __BRAIN__
#define __BRAIN__

#include <deque>

#include <math/quat.h>
#include <math/vec4.h>

#include <Except.h>

class Position;
class Attitude;
class Quadrotor;

namespace Command {
	class Base;
}

class Brain {
	public:
		class EmptyQueue: public StopCond {
			public:
				EmptyQueue(int ti): StopCond(ti) {}
		};


	public:
		Brain(Quadrotor*);

		void		reset();

		void		process_force_reference(math::vec3 f_R, int ti, math::quat& qn, double& thrust);
		
		void		control_law_position(int ti, int ti_0);
		void		control_law_3(int ti, int ti_0);
		
		void		step(int ti);
		
		void		set_motor_speed(int ti, double thrust);

	public:
		int		ti_0_;
		
		Quadrotor*	quad_;

		Position*	pos_;
		Attitude*	att_;

		double		heading_;

		std::deque<Command::Base*>	objs_;

		Command::Base*	obj_;
};



#endif

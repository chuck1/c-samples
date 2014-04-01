#ifndef __BRAIN__
#define __BRAIN__

#include <deque>

#include <math/quat.h>
#include <math/vec4.h>

class Position;
class Attitude;
class Quadrotor;

namespace Command {
	class Base;
}

class Brain {
	public:
		Brain(Quadrotor*);
		void		process_force_reference(math::vec3 f_R, int ti, math::quat& qn, double& thrust);
		void		process_force_reference2(math::vec3 f_R, int ti, double& thrust);

		math::vec4	control_law_2(int ti, int ti_0);
		math::vec4	control_law_3(int ti, int ti_0);
		
		void		step(int ti);

		int		ti_0_;

		Quadrotor*	quad_;

		Position*	pos_;
		Attitude*	att_;

		std::deque<Command::Base*>	objs_;

		Command::Base*	obj_;
};



#endif

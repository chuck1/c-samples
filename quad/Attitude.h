#ifndef __ATTITUDE__
#define __ATTITUDE__

#include <math/vec3.h>
#include <math/quat.h>
#include <math/mat33.h>

class Quadrotor;

namespace Command {
	class Orient;
}

class Attitude {
	public:
		Attitude(Quadrotor* quad);

		void		set_q_reference(int ti, math::quat q);
		void		set_obj(int ti1, Command::Orient* att);
		void		step(int ti, int ti_0);
		math::vec3	get_tau_RB(int ti, int ti_0);
		void		write();

		Quadrotor*	quad_;
		

		math::mat33	C1_;
		math::mat33	C2_;
		
		math::quat*	e1_;
		
		math::quat*	q_ref_;
		math::vec3*	q_ref_d_;
		math::vec3*	q_ref_dd_;
		
		double*		e1_mag_d_;

		math::vec3*	tau_RB_;

		Command::Orient*	att_;

};

#endif

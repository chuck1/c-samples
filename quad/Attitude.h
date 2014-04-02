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
		void		reset();
		void		set_q_reference(int ti, math::quat q);
		void		set_obj(int ti1, Command::Orient* att);
		void		step(int ti, int ti_0);
		void		get_tau_RB(int ti, int ti_0);
		void		write(int n = 0);
		void		write_param();
		void		read_param();

		Quadrotor*	quad_;
		

		math::mat33	C1_;
		math::mat33	C2_;
		
		math::quat*	e1_;
		math::vec3*	e2_;
		
		
		math::quat*	q_ref_;
		
		struct {
			math::vec3*	q_ref_d_;
			math::vec3*	q_ref_dd_;
		} filt, unfilt;
		

		double*		e1_mag_;
		double*		e1_mag_d_;

		math::vec3*	tau_RB_;

		Command::Orient*	att_;

};

#endif

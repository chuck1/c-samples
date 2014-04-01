#ifndef __POSITION__
#define __POSITION__

#include <math/mat33.h>

#include <Brain.h>
#include <Command.h>
#include <Quadrotor.h>
#include <Telem.h>
#include <Plant.h>

class Position {
	public:
		Position(Quadrotor*);

		void		fill_xref(int ti1, math::vec3 x);
		void		fill_xref_parametric(int ti1, math::vec3 (*f)(double));

		void		step(int ti, int ti_0);

		void		set_obj(int ti, Command::Position* pos);

		math::vec3	get_force_rotor(int ti, int ti_0);

		void		write();

		Quadrotor*	quad_;

		Command::Position*	pos_;

		math::mat33	C1_;
		math::mat33	C2_;
		math::mat33	C3_;

		math::vec3*	e1_;

		math::vec3*	chi_;

		double*		e1_mag_d_;


		math::vec3*	x_ref_;
		math::vec3*	x_ref_d_;
		math::vec3*	x_ref_dd_;

		math::vec3*	f_R_;

		unsigned int	flag_;
};

#endif



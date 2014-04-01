#ifndef __TELEM__
#define __TELEM__

#include <stdio.h>

#include <math/vec3.h>
#include <math/vec4.h>
#include <math/quat.h>
#include <math/mat33.h>
#include <math/mat44.h>

class Quadrotor;

class Telem {
	public:
		Telem(Quadrotor*, double* t, int N);

		void		step(int ti);

	public:
		Quadrotor*	quad_;

		double*		t_;

		// physical constants
		double		m_, L_, R_, Asw_, rho_, CD_, A_;
		double		Kv_, Kt_, Ktau_;
		double		k_, b_;

		math::mat33	I_;
		math::mat33	Iinv_;

		math::vec3	gravity_;

		int		N_;

		// state variables
		math::quat*	q_;
		math::vec3*	o_;

		math::vec3*	x_;
		math::vec3*	v_;

		math::mat44	A4_;
		math::mat44	A4inv_;


};


#endif




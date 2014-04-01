#ifndef __PLANT__
#define __PLANT__

#include <math/vec3.h>
#include <math/vec4.h>
#include <math/quat.h>
#include <math/mat33.h>
#include <math/mat44.h>

class Quadrotor;

class Plant {
	public:
		Plant(Quadrotor* quad, double* t, int N);

		math::vec3	get_tau_body(int ti);
		math::vec3	get_tau_rotor_body(int ti);

		math::vec3	get_force_rotor_body(int ti);

		math::vec3	get_force_drag_body(int ti);
		math::vec3	get_force_drag(int ti);

		math::vec3	get_force(int ti);

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
		//math::quat*	q_;
		//math::vec3*	o_;
		math::vec3*	od_;

		//math::vec3*	x_;
		//math::vec3*	v_;
		math::vec3*	a_;

		math::vec4*	gamma_;

		math::mat44	A4_;
		math::mat44	A4inv_;


};




#endif

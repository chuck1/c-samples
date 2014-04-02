/*
#include <math/vec3.h>
#include <math/vec4.h>
#include <math/quat.h>
#include <math/mat33.h>
#include <math/mat44.h>
*/

#include <stdio.h>

#include <Quadrotor.h>
#include <Telem.h>
#include <Plant.h>

Plant::Plant(Quadrotor* quad):
	quad_(quad)
{
	int n = quad_->N_;
		
	od_ = new math::vec3[n];
	a_ = new math::vec3[n];
	
	// constants

	// motor speed
	gamma_ = new math::vec4[n];

	
}
math::vec3 Plant::get_tau_body(int ti) {
	math::vec3 tau = get_tau_rotor_body(ti);
	return tau;
}
math::vec3 Plant::get_tau_rotor_body(int ti) {
	math::vec4 gamma = gamma_[ti];
	
	//if (gamma.isNan()) throw;
	//raise ValueError('gamma nan')
	
	math::vec4 temp = quad_->A4_ * gamma;
	math::vec3 tau(temp.x, temp.y, temp.z);
	
	if(tau.isNan()) {
		printf("A4\n");
		quad_->A4_.print();
		printf("gamma\n");
		gamma.print();
		throw;
	}

	return tau;
}
math::vec3 Plant::get_force_rotor_body(int ti) {
	math::vec4 temp = quad_->A4_ * gamma_[ti];
	math::vec3 T(0.0, 0.0, temp.w);
	return T;
}
math::vec3 Plant::get_force_drag_body(int ti) {
	return math::vec3();
}
math::vec3 Plant::get_force_drag(int ti) {
	return quad_->telem_->q_[ti].getConjugate().rotate(get_force_drag_body(ti));
}
math::vec3 Plant::get_force(int ti) {
	
	math::vec3 f_B = get_force_rotor_body(ti) + get_force_drag_body(ti);

	//if (f_B.isNan()) raise ValueError("f_B nan");

	math::vec3 f = quad_->gravity_ + quad_->telem_->q_[ti].getConjugate().rotate(f_B);

		/*
		   ver = False
		   if ver:	
		   print 'A6 ',A6
		   print 'f_g',f_g
		   print 'f_B',f_B
		   print 'f  ',f
		   */
	return f;
}
void Plant::step(int ti) {
	//double dt = t_[ti] - t_[ti-1];

	// rotation
	math::vec3 tau = get_tau_rotor_body(ti-1);

	od_[ti] = quad_->Iinv_ * (tau - quad_->telem_->o_[ti-1].cross(quad_->I_ * quad_->telem_->o_[ti-1]));
	
	// translation
	math::vec3 f = get_force(ti-1);
	
	a_[ti] = f / quad_->m_;
}



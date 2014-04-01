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

Plant::Plant(Quadrotor* quad, double* t, int N):
	quad_(quad),
	t_(t),
	N_(N)
{

	// physical constants
	m_	= 1.0;		// mass (kg)
	L_	= 0.25;		// arm length (m)
	R_	= 0.05;		// prop radius (m)
	Asw_	= M_PI * R_ * R_;
	
	rho_ 	= 1.28;	// (kg/m3) air density
	CD_ 	= 1.0;	// dimensionless const
	A_	= 0.05 * 0.01;	// prop cross-sectional area (m2)


	Kv_	= 1450;	// back EMF (RPM / V)
	Kv_ = 1.0 / Kv_ * 0.1047;

	Kt_	= Kv_;
	Ktau_	= 0.5;

	k_ = Kv_ * Ktau_ * sqrt(rho_ * Asw_);
	b_ = 0.5 * pow(R_,3) * rho_ * CD_ * A_;
	
	//I_ = ;
	Iinv_ = I_.GetInverse();
	
	
	gravity_ = math::vec3(0,0,-9.81);
	
	// state variables
	//q_ = new math::quat[N_];
	//q[0] = math.quat();
	
	//o_ = new math::vec3[N_];
	od_ = new math::vec3[N_];
	
	//x_ = new math::vec3[N_];
	//v_ = new math::vec3[N_];
	a_ = new math::vec3[N_];
	
	// constants

	// motor speed
	gamma_ = new math::vec4[N_];

	// matrices
	A4_ = math::mat44(
			L_ * k_,	0,		-L_ * k_,	0,
			0,		L_ * k_,	0,		-L_ * k_,
			b_,		-b_,		b_,		-b_,
			k_,		k_,		k_,		k_);
	
	A4inv_ = A4_.GetInverse();
	
}
math::vec3 Plant::get_tau_body(int ti) {
	math::vec3 tau = get_tau_rotor_body(ti);
	return tau;
}
math::vec3 Plant::get_tau_rotor_body(int ti) {
	math::vec4 gamma = gamma_[ti];
	
	//if (gamma.isNan()) throw;
	//raise ValueError('gamma nan')
	
	math::vec4 temp = A4_ * gamma;
	math::vec3 tau(temp.x, temp.y, temp.z);
	
	if(tau.isNan()) {
		printf("A4\n");
		A4_.print();
		printf("gamma\n");
		gamma.print();
		throw;
	}

	return tau;
}
math::vec3 Plant::get_force_rotor_body(int ti) {
	math::vec4 temp = A4_ * gamma_[ti];
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

	math::vec3 f = gravity_ + quad_->telem_->q_[ti].getConjugate().rotate(f_B);

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

	math::vec3 tau = get_tau_rotor_body(ti-1);
	
	od_[ti] = Iinv_ * (tau - quad_->telem_->o_[ti-1].cross(I_ * quad_->telem_->o_[ti-1]));
	
	math::vec3 f = get_force(ti-1);
	
	a_[ti] = f / m_;
}




#include <stdio.h>

#include <math/vec3.h>
#include <math/vec4.h>
#include <math/quat.h>
#include <math/mat33.h>
#include <math/mat44.h>

#include <Telem.h>
#include <Plant.h>
#include <Quadrotor.h>


Telem::Telem(Quadrotor* quad, double* t, int N):
	quad_(quad), t_(t), N_(N)
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
	q_ = new math::quat[N_];
	o_ = new math::vec3[N_];
	
	x_ = new math::vec3[N_];
	v_ = new math::vec3[N_];
	
	// constants


}

void Telem::step(int ti) {
	double dt = t_[ti] - t_[ti-1];

	o_[ti] = o_[ti-1] + quad_->plant_->od_[ti] * dt;
	
	if(o_[ti].isNan()) {
		
		printf("o  "); o_[ti].print();
		throw;
	}
	
	double o_magn = o_[ti].magnitude();
	
	math::quat r;
	
	if (o_magn == 0.0) {
		r = math::quat();
	} else {
		math::vec3 o_hat = o_[ti] / o_magn;
		r = math::quat(o_magn * dt, o_hat);
	}
	
	if(!r.isSane()) {
		printf("o_ "); o_[ti].print();
		printf("o_magn %f\n", o_magn);
		throw;
	}
	
	q_[ti] = r * q_[ti-1];
	
	// position
	v_[ti] = v_[ti-1] + quad_->plant_->a_[ti] * dt;
	x_[ti] = x_[ti-1] + v_[ti] * dt;
	
}





#include <math/vec3.h>
#include <math/vec4.h>
#include <math/quat.h>
#include <math/mat33.h>
#include <math/mat44.h>

class Quadrotor {
	public:
		Quadrotor(double* t, int N);
		math::vec3	get_tau_body(int ti);
		math::vec3	get_tau_rotor_body(int ti);
		
		math::vec3	get_force_rotor_body(int ti);

		math::vec3	get_force_drag_body(int ti);
		math::vec3	get_force_drag(int ti);

		math::vec3	get_force(int ti);

		void		step(int ti);




	public:
		double*	t_;

	// physical constants
	double	m_, L_, R_, Asw_, rho_, CD_, A_;
	double	Kv_, Kt_, Ktau_;
	double	k_, b_;

	math::mat33	I_;
	math::mat33	Iinv_;

	math::vec3	gravity_;


	int		N_;

	// state variables
	math::quat*	q_;
	math::vec3*	o_;

	math::vec3*	x_;
	math::vec3*	v_;
	math::vec3*	a_;
	
	math::vec4*	gamma_;

	math::mat44	A4_;
	math::mat44	A4inv_;


};

Quadrotor::Quadrotor(double* t, int N): t_(t), N_(N) {

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
	


	// reference MOVE TO ATTITUDE CONTROLLER
	



	// state variables
	q_ = new math::quat[N_];
	//q[0] = math.quat();
	
	o_ = new math::vec3[N_];
	
	x_ = new math::vec3[N_];
	v_ = new math::vec3[N_];
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
math::vec3 Quadrotor::get_tau_body(int ti) {
	math::vec3 tau = get_tau_rotor_body(ti);
	return tau;
}
math::vec3 Quadrotor::get_tau_rotor_body(int ti) {
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
math::vec3 Quadrotor::get_force_rotor_body(int ti) {
	math::vec4 temp = A4_ * gamma_[ti];
	math::vec3 T(0.0, 0.0, temp.w);
	return T;
}
math::vec3 Quadrotor::get_force_drag_body(int ti) {
	return math::vec3();
}
math::vec3 Quadrotor::get_force_drag(int ti) {
	return q_[ti].getConjugate().rotate(get_force_drag_body(ti));
}
math::vec3 Quadrotor::get_force(int ti) {
	
	math::vec3 f_B = get_force_rotor_body(ti) + get_force_drag_body(ti);

	//if (f_B.isNan()) raise ValueError("f_B nan");

	math::vec3 f = gravity_ + q_[ti].getConjugate().rotate(f_B);

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
void Quadrotor::step(int ti) {
	double dt = t_[ti] - t_[ti-1];

	math::vec3 tau = get_tau_rotor_body(ti-1);
	
	math::vec3 omegad = Iinv_ * ( tau - o_[ti-1].cross( I_ * o_[ti-1] ) );
	
	//theta  = theta[ti-1]
	//thetap = get_thetad(ti-1)
	
	o_[ti] = o_[ti-1] + omegad * dt;
	
	if(o_[ti].isNan()) {
		printf("tau"); tau.print();
		
		printf("o  "); o_[ti].print();
		printf("od "); omegad.print();
		throw;
	}
	
	//theta[ti] = theta + thetap * dt
	
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
	
	
	//ver = False;
	//if (ver) {
		//print 'tau    ',tau
		//	print 'omegad ',omegad
		//	print 'omega_n',omega_n
		//	print 'r      ',r.s,r.v
	//}
	
	// position
	math::vec3 f = get_force(ti-1);
	
	//if (any(np.isnan(f))) throw;//raise ValueError('f nan');
	
	a_[ti] = f / m_;
	
	v_[ti] = v_[ti-1] + a_[ti] * dt;
	x_[ti] = x_[ti-1] + v_[ti] * dt;
	
}




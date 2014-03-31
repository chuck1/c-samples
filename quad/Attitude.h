#include <math/vec3.h>

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

Attitude::Attitude(Quadrotor* quad):
	quad_(quad)
{

	double C1 = 1.6;

	double C2 = 0.8;

	C1_ = math::mat33(
		C1,0.0,0.0,
		0.0,C1,0.0,
		0.0,0.0,C1);

	C2_ = math::mat33(
		C2,0.0,0.0,
		0.0,C2,0.0,
		0.0,0.0,C2);


	int N = quad_->N_;

	e1_ = new math::quat[N];
	
	e1_mag_d_ = new double[N];
	
	q_ref_ = new math::quat[N];
	q_ref_d_ = new math::vec3[N];
	q_ref_dd_ = new math::vec3[N];
	
	
	tau_RB_ = new math::vec3[N];

	att_ = NULL;
}
void Attitude::set_q_reference(int ti, math::quat q) {
	q_ref_[ti] = q;
}
void Attitude::set_obj(int ti1, Command::Orient* att) {
	att_ = att;
	
	for (int ti = ti1; ti < quad_->N_; ti++) q_ref_[ti] = att_->q_;
}	
void Attitude::step(int ti, int ti_0) {
	double dt = quad_->t_[ti] - quad_->t_[ti-1];
	
	// q = c.q[ti];
	// q_ref = q_ref[ti];
	
	e1_[ti] = q_ref_[ti] * quad_->q_[ti].getConjugate();
	
	// q_ref_0 = q_ref[ti-1];
	// q_ref_1 = q_ref[ti-0];
	
	// q_refd
	if (ti_0 > 1) {
		math::quat r = q_ref_[ti] * q_ref_[ti-1].getConjugate();
		q_ref_d_[ti] = r.getOmega(dt);

		if(q_ref_d_[ti].isNan()) {
			printf("dt %f\n",dt);
			printf("r\n");
			r.print();
			printf("q_ref_d\n");
			q_ref_d_[ti].print();
			
			throw;
		}

	} else {
		q_ref_d_[ti] = math::vec3();
	}


	
	// clamp
	//q_refd_1 = vec.clamparr(q_refd_1, -1.0, 1.0);

	// q_refdd
	if (ti_0 > 2) {
		q_ref_dd_[ti] = (q_ref_d_[ti] - q_ref_d_[ti-1]) / dt;
	} else {
		q_ref_dd_[ti] = math::vec3();
	}


	// omega ref
	// omega_ref[ti] = self.q_refd[ti];

	// omega error
	//e2[ti] = self.omega_ref[ti] - self.c.omega[ti];

	// e1 mag d	
	if (ti_0 > 0) {
		double e1_mag_1 = e1_[ti-0].getImaginaryPart().magnitude();
		double e1_mag_0 = e1_[ti-1].getImaginaryPart().magnitude();
		
		e1_mag_d_[ti] = (e1_mag_1 - e1_mag_0) / dt;
	}

	// check objective
	if (ti_0 > 0) {
		if (att_) {
			if (att_->mode_ == Command::Base::Mode::NORMAL) {
				if ((e1_mag_d_[ti] < 0.0) && (e1_mag_d_[ti] > -0.001)) {
					if (e1_[ti].getAngle() < att_->thresh_) {
						att_->flag_ |= Command::Base::Flag::COMPLETE;
					}
				}
			}
		}
	}


	// extras
	/*
	   Attitude::prin():
	   print 'q_ref_1 ',q_ref_1.v
	   print 'q_ref_0 ',q_ref_0.v
	   print 'r       ',r.v
	   print 'q_refd_n',q_refd_1
	 */

}
math::vec3 Attitude::get_tau_RB(int ti, int ti_0) {
	// require error values
	step(ti, ti_0);

	math::quat q = quad_->q_[ti];
	math::vec3 o = quad_->o_[ti];
	
	math::mat33 I = quad_->I_;
	
	math::vec3 od = C1_ * e1_[ti].getImaginaryPart() + C2_ * (q_ref_d_[ti] - o) + q_ref_dd_[ti];
	
	if(od.isNan()) {
		printf("od\n");
		od.print();
		printf("q_ref_d\n");
		q_ref_d_[ti].print();
		printf("q_ref_dd\n");
		q_ref_dd_[ti].print();
		printf("e1\n");
		e1_[ti].getImaginaryPart().print();
		throw;
	}
	
	//tau_RB = omegad
	tau_RB_[ti] = I * od + o.cross(I * o);

	if(tau_RB_[ti].isNan()) {
		printf("od\n");
		od.print();
		printf("o\n");
		o.print();
		printf("I\n");
		I.print();
		throw;
	}

	return tau_RB_[ti];
}
void Attitude::write() {
	FILE* file = fopen("att.txt","w");
	
	int N = quad_->N_;
	
	math::vec3* e1 = new math::vec3[N];
	math::vec3* q = new math::vec3[N];
	math::vec3* q_ref = new math::vec3[N];
	for(int ti = 0; ti < N; ti++) {
		e1[ti] = e1_[ti].getImaginaryPart();
		q[ti] = quad_->q_[ti].getImaginaryPart();
		q_ref[ti] = q_ref_[ti].getImaginaryPart();
	}
	
	
	fwrite(e1,		sizeof(math::vec3), quad_->N_, file);
	fwrite(q,		sizeof(math::vec3), quad_->N_, file);
	fwrite(q_ref,		sizeof(math::vec3), quad_->N_, file);
	fwrite(q_ref_d_,	sizeof(math::vec3), quad_->N_, file);
	fwrite(q_ref_dd_,	sizeof(math::vec3), quad_->N_, file);
	fwrite(tau_RB_,		sizeof(math::vec3), quad_->N_, file);


}



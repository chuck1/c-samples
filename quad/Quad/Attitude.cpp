

#include <math/vec3.h>
#include <math/quat.h>
#include <math/mat33.h>

#include <FDA.h>
#include <Quadrotor.h>
#include <Telem.h>
#include <Command.h>
#include <Plant.h>
#include <Attitude.h>

Attitude::Attitude(Quadrotor* quad):
	quad_(quad)
{

	double C1 = 15.88;
	double C2 =  5.2;
	
	C1_ = math::mat33(
		C1,0.0,0.0,
		0.0,C1,0.0,
		0.0,0.0,C1);

	C2_ = math::mat33(
		C2,0.0,0.0,
		0.0,C2,0.0,
		0.0,0.0,C2);

	//read_param();

	int N = quad_->N_;

	e1_ = new math::quat[N];
	e2_ = new math::vec3[N];

	e1_mag_ = new double[N];
	e1_mag_d_ = new double[N];
	
	q_ref_ = new math::quat[N];

	unfilt.q_ref_d_ = new math::vec3[N];
	unfilt.q_ref_dd_ = new math::vec3[N];
	filt.q_ref_d_ = new math::vec3[N];
	filt.q_ref_dd_ = new math::vec3[N];

	
	tau_RB_ = new math::vec3[N];

	att_ = NULL;
}
void Attitude::reset() {
	
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

	// low-pass filter for derivatives	
	double tc = 0.0 * dt;

	// reference derivatives
	forward_quavec(q_ref_, unfilt.q_ref_d_,  dt, ti, ti_0, 0);
	
	low_pass(unfilt.q_ref_d_, filt.q_ref_d_, ti, dt, tc);
	
	forward(filt.q_ref_d_, unfilt.q_ref_dd_, dt, ti, ti_0, 1);
	
	low_pass(unfilt.q_ref_dd_, filt.q_ref_dd_, ti, dt, tc);
	

	// errors and their magnitudes	
	e1_[ti] = q_ref_[ti] * quad_->telem_->q_[ti].getConjugate();
	
	e1_mag_[ti] = e1_[ti].getImaginaryPart().magnitude();
	
	e2_[ti] = filt.q_ref_d_[ti] - quad_->telem_->o_[ti];
	
	// magnitude derivatives
	forward(e1_mag_,  e1_mag_d_, dt, ti, ti_0, 0);

	// check
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

}
void Attitude::get_tau_RB(int ti, int ti_0) {
	// require error values
	step(ti, ti_0);

	math::vec3 o = quad_->telem_->o_[ti];

	// reference acceleration
	math::vec3 od = 
		C1_ * e1_[ti].getImaginaryPart() 
		+ C2_ * e2_[ti] 
		+ filt.q_ref_dd_[ti];
	
	//printf("od\n");
	//od.print();
	
	if(od.isNan()) {
		printf("od\n");
		od.print();
		printf("q_ref_d\n");
		filt.q_ref_d_[ti].print();
		printf("q_ref_dd\n");
		filt.q_ref_dd_[ti].print();
		printf("e1\n");
		e1_[ti].getImaginaryPart().print();
		throw;
	}
	
	// reference torque
	tau_RB_[ti] = quad_->I_ * od + o.cross(quad_->I_ * o);

	if(tau_RB_[ti].isNan()) {
		printf("od\n");
		od.print();
		printf("o\n");
		o.print();
		printf("I\n");
		quad_->I_.print();
		throw;
	}

	//return tau_RB_[ti];
}
void Attitude::write(int n) {
	FILE* file = fopen("att.txt","w");

	n = (n > 0) ? (n) : (quad_->N_);

	math::vec3* e1 = new math::vec3[n];
	math::vec3* q = new math::vec3[n];
	math::vec3* q_ref = new math::vec3[n];

	for(int ti = 0; ti < n; ti++) {
		e1[ti] = e1_[ti].getImaginaryPart();
		q[ti] = quad_->telem_->q_[ti].getImaginaryPart();
		q_ref[ti] = q_ref_[ti].getImaginaryPart();
	}


	fwrite(e1,			sizeof(math::vec3), n, file);
	fwrite(q,			sizeof(math::vec3), n, file);
	fwrite(quad_->telem_->o_,	sizeof(math::vec3), n, file);
	fwrite(q_ref,			sizeof(math::vec3), n, file);
	fwrite(filt.q_ref_d_,		sizeof(math::vec3), n, file);
	fwrite(filt.q_ref_dd_,		sizeof(math::vec3), n, file);
	fwrite(tau_RB_,			sizeof(math::vec3), n, file);

	fclose(file);
}
void Attitude::write_param() {
	FILE* file = fopen("att_param.txt","w");
	fprintf(file, "%f,%f,%f,%f,%f,%f",
			C1_.v[0],
			C1_.v[4],
			C1_.v[8],
			C2_.v[0],
			C2_.v[4],
			C2_.v[8]);
	fclose(file);
}
void Attitude::read_param() {
	FILE* file = fopen("att_param.txt","r");
	if(file != NULL) {
	fscanf(file, "%f,%f,%f,%f,%f,%f",
			C1_.v+0,
			C1_.v+4,
			C1_.v+8,
			C2_.v+0,
			C2_.v+4,
			C2_.v+8);
	}
	fclose(file);
}




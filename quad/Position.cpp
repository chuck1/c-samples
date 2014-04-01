

#include <math/mat33.h>

#include <Brain.h>
#include <Command.h>
#include <Quadrotor.h>
#include <Telem.h>
#include <Plant.h>
#include <Position.h>

Position::Position(Quadrotor* quad):
	quad_(quad),
	flag_(0)
{

	double C1_11 = 0.2;
	double C1_22 = 0.2;
	double C1_33 = 0.2;

	double C2_11 = 0.4;
	double C2_22 = 0.4;
	double C2_33 = 0.4;

	double C3_11 = 0.0;
	double C3_22 = 0.0;
	double C3_33 = 0.0;


	C1_ = math::mat33(
			C1_11,0,0,
			0,C1_22,0,
			0,0,C1_33);
	C2_ = math::mat33(
			C2_11,0,0,
			0,C2_22,0,
			0,0,C2_33);
	C3_ = math::mat33(
			C3_11,0,0,
			0,C3_22,0,
			0,0,C3_33);

	int N = quad_->N_;

	chi_ = new math::vec3[N];

	e1_ = new math::vec3[N];

	e1_mag_d_ = new double[N];

	x_ref_ = new math::vec3[N];
	x_ref_d_ = new math::vec3[N];
	x_ref_dd_ = new math::vec3[N];

	f_R_ = new math::vec3[N];

}
void Position::fill_xref(int ti1, math::vec3 x) {
	for (int ti = ti1; ti < quad_->N_; ti++) x_ref_[ti] = x;
}
void Position::fill_xref_parametric(int ti1, math::vec3 (*f)(double)) {
	for(int ti = ti1; ti < quad_->N_; ti++) {
		double t = quad_->t_[ti];
		x_ref_[ti] = f(t);
	}
}
void Position::step(int ti, int ti_0) {
	double dt = quad_->t_[ti] - quad_->t_[ti-1];

	e1_[ti] = x_ref_[ti] - quad_->telem_->x_[ti];
	
	// reference
	if (ti_0 > 0) {
		x_ref_d_[ti] = (x_ref_[ti] - x_ref_[ti-1]) / dt;
		
		chi_[ti] = chi_[ti-1] + e1_[ti] * dt;
	
		e1_mag_d_[ti] = (e1_[ti].magnitude() - e1_[ti-1].magnitude()) / dt;
	}
	
	if (ti_0 > 1) {
		x_ref_dd_[ti] = (x_ref_d_[ti] - x_ref_d_[ti-1]) / dt;
	}

	// step position error
	


	//x_ref_[ti].print();
	//printf("e1_mag %f\n",e1_mag);


	
	
	if (pos_) {
		//printf("pos\n");

		if (pos_->mode_ == Command::Position::Mode::NORMAL) {
			//printf("normal\n");

			bool close = e1_[ti].abs_less(pos_->thresh_);
			
			//if(close) printf("close\n");

			if (ti_0 > 1) {
				if (e1_mag_d_[ti] > -0.01) {
					if (e1_mag_d_[ti] < 0.0) {
						if (close) {

							//print 'e5 ',e5[ti]
							//print 'x ',c.x[ti]
							//print 'x_ref',x_ref[ti]
							((Command::Move*)pos_)->settle(quad_->t_[ti]);
							//pos_->flag_ |= Command::Position::Flag::COMPLETE;
						}
					}
				}
			}
		} else {
			//printf("mode %i\n",pos_->mode_);
		}
	}
}
void Position::set_obj(int ti, Command::Position* pos) {
	pos_ = pos;

	// reset
	flag_ &= ~Command::Position::Flag::COMPLETE;

	Command::Move* move;
	Command::Path* path;

	if (pos == NULL) throw;

	switch(pos_->type_) {
		case Command::Base::Type::MOVE:
			move = (Command::Move*)pos_;
			fill_xref(ti, move->x2_);
			break;
		case Command::Base::Type::PATH:
			path = (Command::Path*)pos_;

			printf("ti %i path %p\n",ti,path);

			fill_xref_parametric(ti, path->f_);
			break;
	}

}
math::vec3 Position::get_force_rotor(int ti, int ti_0) {

	double m = quad_->plant_->m_;
	math::vec3 g = quad_->plant_->gravity_;
	
	math::vec3 f_D = quad_->plant_->get_force_drag(ti);

	f_R_[ti] = C1_ * (x_ref_[ti] - quad_->telem_->x_[ti]) * m;
	f_R_[ti] += C2_ * (x_ref_d_[ti] - quad_->telem_->v_[ti]) * m;
	f_R_[ti] += C3_ * chi_[ti] * m;
	f_R_[ti] += -g * m - f_D;
	
	
	if(f_R_[ti].isNan()) {
		printf("f_R is nan\n");
		x_ref_[ti].print();
		x_ref_d_[ti].print();
		x_ref_dd_[ti].print();
		throw;
	}

	return f_R_[ti];
}

void Position::write() {
	FILE* file = fopen("pos.txt","w");

	printf("sizeof vec3 %i\n",(int)sizeof(math::vec3));

	int N = quad_->N_;

	fwrite(e1_,			sizeof(math::vec3), N, file);
	fwrite(quad_->telem_->x_,	sizeof(math::vec3), N, file);
	fwrite(x_ref_,			sizeof(math::vec3), N, file);
	fwrite(x_ref_d_,		sizeof(math::vec3), N, file);
	fwrite(x_ref_dd_,		sizeof(math::vec3), N, file);
	fwrite(f_R_,			sizeof(math::vec3), N, file);

	//for(int ti = 0; ti < quad_->N_; ti++) {
	// fwrite(&e1_[ti].x, sizeof(float), 1, file);
	//}

}








#include <deque>

#include <Position.h>
#include <Attitude.h>
#include <Command.h>

Brain::Brain(Quadrotor* quad):
	quad_(quad)
{
	pos_ = new Position(quad);
	att_ = new Attitude(quad);


	obj_ = NULL;
}
void Brain::reset() {
	objs_.clear();
	obj_ = NULL;

	pos_->reset();
	att_->reset();
}
void Brain::process_force_reference(math::vec3 f_R, int ti, math::quat& qn, double& thrust) {
	math::vec3 z(0,0,1);
	math::quat q = quad_->telem_->q_[ti];
	
	// ignore all but z-component
	//f_R.x = 0;
	//f_R.y = 0;

	if(!q.isSane()) printf("insane\n");
	//printf("q"); q.print();
	if(f_R.isNan()) {
		printf("f_R is nan");
		throw;
	}

	// transform desired rotor force from inertial to body frame
	math::vec3 f_RB = q.rotate(f_R);
	math::vec3 z_I = q.rotate(z);
	
	// match inertial z-component
	thrust = f_RB.dot(z_I) / z.dot(z_I);
	
	if (f_RB.dot(z) < 0.0) f_RB = -f_RB;
	
	math::quat r(f_RB,z);
	
	qn = r * q;
	
	// eliminate z-axis rotation
	qn.z = 0.0;
	qn.normalize();
	
	if(!q.isSane()) {
		printf("qn insane\n");
		throw;
	}

	//if (r.getAngle() > (M_PI / 4.0)) {
	//	thrust = 0.0;
	//}
}	


void Brain::control_law_2(int ti, int ti_0) {
	// position control
	
	// require position error
	pos_->step(ti, ti_0);
	
	pos_->get_force_rotor(ti, ti_0);
	

	math::quat q;
	double thrust;
	

	process_force_reference(pos_->f_R_[ti], ti, q, thrust);

	//printf("f_R\n");
	//pos_->f_R_[ti].print();
	
	//printf("q\n");
	//q.print();

	if(!q.isSane()) {
		q.print();
		throw;
	}
	

	att_->set_q_reference(ti, q);

	// get body torque
	att_->get_tau_RB(ti, ti_0);
	
	set_motor_speed(ti, thrust);
}
void Brain::set_motor_speed(int ti, double thrust) {

	math::vec4 temp(
			att_->tau_RB_[ti].x,
			att_->tau_RB_[ti].y,
			att_->tau_RB_[ti].z,
			thrust);

	quad_->plant_->gamma_[ti] = quad_->A4inv_ * temp;
	
}
void Brain::control_law_3(int ti, int ti_0) {
	// require position error
	pos_->step(ti, ti_0);

	/*math::vec3 f_R =*/ pos_->get_force_rotor(ti, ti_0);

	double thrust;
	math::quat q; // not used

	process_force_reference(pos_->f_R_[ti], ti, q, thrust);

	// get body torque
	att_->get_tau_RB(ti, ti_0);


	set_motor_speed(ti, thrust);
	
}	
void Brain::step(int ti) {

	if ((obj_ == NULL) || (obj_->flag_ & Command::Base::Flag::COMPLETE)) {
		
		if(objs_.empty()) {
			throw Brain::EmptyQueue(ti);
		}

		//print 'new move'
		obj_ = objs_.front();
		objs_.pop_front();
		ti_0_ = 0;
		
		switch(obj_->type_) {
			case Command::Base::Type::MOVE:
				pos_->set_obj(ti, (Command::Position*)obj_);
				break;
			case Command::Base::Type::PATH:
				pos_->set_obj(ti, (Command::Position*)obj_);
				break;
			case Command::Base::Type::ORIENT:
				// set reference altitude to current altitude
				Command::Move* move = new Command::Move(quad_->telem_->x_[ti]);
				pos_->set_obj(ti, move);

				att_->set_obj(ti, (Command::Orient*)obj_);

				
				break;
		}
	}

	switch(obj_->type_) {
		case Command::Base::Type::MOVE:
			control_law_2(ti, ti_0_);
			break;
		case Command::Base::Type::PATH:
			control_law_2(ti, ti_0_);
			break;
		case Command::Base::Type::ORIENT:
			control_law_3(ti, ti_0_);
			break;
	}	

	ti_0_++;
}





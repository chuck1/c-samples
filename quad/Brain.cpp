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

void Brain::process_force_reference(math::vec3 f_R, int ti, math::quat& qn, double& thrust) {
	// input:
	// target force in inertial frame

	// output:
	// target quaternion orientation
	// target rotor thrust

	math::vec3 z(0,0,1);

	math::quat q = quad_->telem_->q_[ti];
	
	if(!q.isSane()) printf("insane\n");
	
	//printf("q"); q.print();
	
	if(f_R.isNan()) {
		printf("f_R is nan");
		throw;
	}

	// transform desired rotor force from inertial to body frame
	math::vec3 f_RB = q.rotate(f_R);

	math::quat r;
	if (f_RB.dot(z) < 0.0) {
		// force is down
		printf("force is down\n");
		f_RB.print();
	} else {
		r = math::quat(f_RB,z);
	}

	if(!r.isSane()) {
		printf("r is insane\n");
		r.print();
		throw;
	}
	
	qn = r * q;

	// eliminate z-axis rotation
	qn.z = 0.0;
	qn.normalize();

	math::vec3 z_I = q.rotate(z);

	// match inertial z-component
	thrust = f_RB.dot(z_I) / z.dot(z_I);
		
	// body z-component
	//thrust = f_RB[2]

	//if qn.isnan() {
	//print 'q',q.s,q.v
	//print 'r',r.s,r.v
	//	throw //raise ValueError('qn nan')

}	
void Brain::process_force_reference2(math::vec3 f_R, int ti, double& thrust) {
	// input:
	// target force in inertial frame

	// output:
	// target quaternion orientation
	// target rotor thrust

	math::vec3 z(0,0,1);

	math::quat q = quad_->telem_->q_[ti];

	// transform desired rotor force from inertial to body frame

	// ignore all but z-component
	f_R.x = 0;
	f_R.y = 0;
	
	math::vec3 f_RB = q.rotate(f_R);

	math::vec3 z_I = q.rotate(z);


	
	// match inertial z-component	
	thrust = f_RB.dot(z_I) / z.dot(z_I);
	
	if (f_RB.dot(z) < 0.0) f_RB = -f_RB;
	
	
	math::quat r(f_RB, z);

	if (r.getAngle() > (M_PI / 4.0)) {
		thrust = 0.0;
	}

}

math::vec4 Brain::control_law_2(int ti, int ti_0) {
	// position control
	
	// require position error
	pos_->step(ti, ti_0);
	
	math::vec3 f_R = pos_->get_force_rotor(ti, ti_0);
	
	math::quat q;
	double thrust;
	
	process_force_reference(f_R, ti, q, thrust);
	
	if(!q.isSane()) {
		q.print();
		throw;
	}
	
	// debug	
	//theta = np.array([math.pi/2.0, 0.0, 0.0])

	// set attitude reference
	att_->set_q_reference(ti, q);

	// get body torque
	math::vec3 tau_RB = att_->get_tau_RB(ti, ti_0);

	if(tau_RB.isNan()) {
		printf("tau_RB\n");
		tau_RB.print();
		throw;
	}
	
	// calculate motor speed
	
	math::vec4 temp(tau_RB.x, tau_RB.y, tau_RB.z, thrust);
	
	math::vec4 gamma = quad_->plant_->A4inv_ * temp;

	if(gamma.isNan()) {
		printf("tau_RB\n");
		tau_RB.print();
		printf("thrust %f\n", thrust);
		printf("gamma\n");
		gamma.print();
		throw;
	}

	return gamma;
}	
math::vec4 Brain::control_law_3(int ti, int ti_0) {
	// require position error
	pos_->step(ti, ti_0);

	math::vec3 f_R = pos_->get_force_rotor(ti, ti_0);

	double thrust;

	process_force_reference2(f_R, ti, thrust);

	// get body torque
	math::vec3 tau_RB = att_->get_tau_RB(ti, ti_0);

	// calculate motor speed
	math::vec4 temp(tau_RB.x, tau_RB.y, tau_RB.z, thrust);

	math::vec4 gamma = quad_->plant_->A4inv_ * temp;

	return gamma;
}	
void Brain::step(int ti) {

	if ((obj_ == NULL) || (obj_->flag_ & Command::Base::Flag::COMPLETE)) {
		
		if(objs_.empty()) {
			Command::EmptyQueue e;
			throw e;
		}

		//print 'new move'
		obj_ = objs_.front();
		objs_.pop_front();
		
		
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

				ti_0_ = 0;
				break;
		}
	}

	math::vec4 gamma;

	switch(obj_->type_) {
		case Command::Base::Type::MOVE:
			gamma = control_law_2(ti, ti_0_);
			break;
		case Command::Base::Type::PATH:
			gamma = control_law_2(ti, ti_0_);
			break;
		case Command::Base::Type::ORIENT:
			gamma = control_law_3(ti, ti_0_);
			break;
	}	

	quad_->plant_->gamma_[ti] = gamma;

	ti_0_++;
}







enum ObjMode {
	normal = 0,
	hold = 1
};

class Objective {
	Objective() {
		flag_complete = False;
	}
};

class Move: public Objective {
	Move(x2, thresh = None, mode = ObjMode.normal) {
		Objective.__init__(self)
		
		self.x2 = x2
		self.thresh = thresh		
		self.mode = mode

class Path: public Objective {
	Path(self, f) {
		Objective.__init__(self)
		
		self.f = f

		self.mode = ObjMode.hold
	}
}

class Orient(Objective) {
	def __init__(self, q, thresh = None, mode = ObjMode.normal) {
		Objective.__init__(self)

		self.q = q
		self.thresh = thresh
		self.mode = mode
	}
}	

class Brain:
	__init__(self, c) {
		self.c = c

		self.ctrl_position = position.Position1(c)
		self.ctrl_attitude = attitude.Attitude3(c)

		self.obj = None
	}	
	control_law_1(self, ti) {
		gamma = np.zeros(4)
		
		return gamma
	}
	process_force_reference(self, f_R, ti) {
		// input:
		// target force in inertial frame
		
		// output:
		// target quaternion orientation
		// target rotor thrust
		
		q = self.c.q[ti]
		
		// transform desired rotor force from inertial to body frame
		f_RB = q.rotate(f_R)
		
		if np.dot(f_RB, vec.e2) < 0:
			// force is down
			r = qt.Quat()
		else:
			r = qt.Quat(v1 = f_RB, v2 = qt.e2)
			
		
		qn = r * q
		
		
		z_I = q.rotate(vec.e2)
		
		// match inertial z-component
		thrust = np.dot(f_RB, z_I) / np.dot(vec.e2, z_I)
		
		// body z-component
		//thrust = f_RB[2]

		if qn.isnan() {
			//print 'q',q.s,q.v
			//print 'r',r.s,r.v
			throw //raise ValueError('qn nan')

		return qn, thrust
		}	
	process_force_reference2(self, f_R, ti) {
		// input:
		// target force in inertial frame
		
		// output:
		// target quaternion orientation
		// target rotor thrust
		
		q = self.c.q[ti]
		
		// ignore all but z-component
		f_R = np.multiply(f_R, vec.e2)
		
		// transform desired rotor force from inertial to body frame
		f_RB = q.rotate(f_R)
		
				
		
		z_I = q.rotate(vec.e2)
		
		
		
		// match inertial z-component	
		thrust = np.dot(f_RB, z_I) / np.dot(vec.e2, z_I)
		
		if np.dot(f_RB, vec.e2) < 0:
			f_RB = -f_RB
		
		
		r = qt.Quat(v1 = f_RB, v2 = qt.e2)
		
		
		
		if (2.0 * math.asin(vec.mag(r.v))) > (math.pi / 4.0) {
			thrust = 0.0
		
		return thrust
	}
	control_law_2(self, ti, ti_0) {
		// require position error
		self.ctrl_position.step(ti, ti_0)
		
		f_R = self.ctrl_position.get_force_rotor(ti, ti_0)
		
		f_R_mag = vec.mag(f_R)
	
		q, thrust = self.process_force_reference(f_R, ti)
		
		// debug	
		//theta = np.array([math.pi/2.0, 0.0, 0.0])
		
		// set attitude reference
		self.ctrl_attitude.set_q_reference(ti, q)
		
		// get body torque
		tau_RB = self.ctrl_attitude.get_tau_RB(ti, ti_0)
		
		// calculate motor speed
		gamma = np.dot(self.c.A4inv, np.append(tau_RB, thrust))
	
		ver = False
		if (ver) {
			//print 'f_R',f_R
			//print 'theta',theta
			//print self.c.A4inv
			//print tau_RB
			//print np.append(tau_RB, fz_RB)
		}
		return gamma
	}	
	control_law_3(int ti, int ti_0, double gamma[4]) {
		// require position error
		self.ctrl_position.step(ti, ti_0)
		
		f_R = self.ctrl_position.get_force_rotor(ti, ti_0)
		
		f_R_mag = vec.mag(f_R)
		
		thrust = self.process_force_reference2(f_R, ti)
		
		// get body torque
		tau_RB = self.ctrl_attitude.get_tau_RB(ti, ti_0)
		
		// calculate motor speed
		gamma = np.dot(self.c.A4inv, np.append(tau_RB, thrust))
	
		ver = False
		if ver:
			pass
		
		return gamma
	}	
	void step(self, ti) {
		
		
		if (self.obj is None) or self.obj.flag_complete:
			//print 'new move'
			self.obj = self.objs.pop(0)
			
			if isinstance(self.obj, Move) {
				self.ctrl_position.set_obj(ti, self.obj)

			if isinstance(self.obj, Path) {
				self.ctrl_position.set_obj(ti, self.obj)
			
			elif isinstance(self.obj, Orient) {
				// set reference altitude to current altitude
				move = Move(self.c.x[ti], mode = ObjMode.hold)
				self.ctrl_position.set_obj(ti, move)
				
				self.ctrl_attitude.set_obj(ti, self.obj)
			
			self.ti_0 = 0
		
		
		
		
		if isinstance(self.obj, Move) {
			gamma = self.control_law_2(ti, self.ti_0)
		}
		else if isinstance(self.obj, Orient) {
			gamma = self.control_law_3(ti, self.ti_0)
		}
		else if isinstance(self.obj, Path) {
			gamma = self.control_law_2(ti, self.ti_0)
		}

		self.c.gamma[ti] = gamma
		
		self.ti_0 += 1
	}	
	void plot(self) {
		//self.ctrl_x.plot()
		//self.ctrl_y.plot()
		//self.ctrl_z.plot()
		//self.ctrl_rot[0].plot()
		//self.ctrl_tilt.plot()
		pass

	}
}

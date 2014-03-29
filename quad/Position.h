
#include <math/mat33.h>

class Position {
	Position(Quadrotor*);

	Quadrotor*	quad_;

	

};


	Position::Position(Quadrotor* quad) {
		
		double C1_11 = 0.7;
		double C1_22 = 0.7;
		double C1_33 = 0.3;
		
		double C2_11 =  3.5;
		double C2_22 =  3.5;
		double C2_33 = 10.0;
		
		double C3_11 = 0.0;
		double C3_22 = 0.0;
		double C3_33 = 0.0;
		
		
		C1_ = math::mat33(
				C1_11,0,0,
				0,C1_22,0,
				0,0,C1_33)
		C2_ = math::mat33(
				C2_11,0,0,
				0,C2_22,0,
				0,0,C2_33)
		C3_ = math::mat33(
				C3_11,0,0,
				0,C3_22,0,
				0,0,C3_33)
		
		chi = new math::quat[


		
		e5_mag_d = np.zeros(c.N)




		x_ref = np.zeros((c.N, 3))
		x_refd = np.zeros((c.N, 3))
		x_refdd = np.zeros((c.N, 3))

		v_ref = np.zeros((c.N, 3))

		f_R = np.zeros((c.N, 3))

		flag_converged = False
		
	Position::fill_xref( ti1, x):
		for ti in range(ti1, c.N):
			x_ref[ti] = np.array(x)

	Position::fill_xref_parametric( ti1, f):
		for ti in range(ti1, c.N):
			t = c.t[ti]
			x_ref[ti] = np.array(f(t))
		
		//pl.plot(c.t, x_ref)
		
	Position::get_f6( e5, e6):
		return -np.dot(C6,e6) - e5
	Position::step( ti, ti_0):
		dt = c.t[ti] - c.t[ti-1]
		
		// reference
		x_refd[ti] = (x_ref[ti] - x_ref[ti-1]) / dt
		
		if ti_0 > 1:
			x_refdd[ti] = (x_refd[ti] - x_refd[ti-1]) / dt
		
		// step position error
		e5[ti] = x_ref[ti] - c.x[ti]
		
		if ti_0 > 0:
			chi5[ti] = chi5[ti-1] + e5[ti] * dt
		
		if ti_0 > 0:
			e5_mag_d[ti] = (vec.mag(e5[ti]) - vec.mag(e5[ti-1])) / dt
		
		// step v_ref before stepping e6
		v_ref[ti] = np.dot(C5, e5[ti]) + x_refd[ti] + np.dot(L5, chi5[ti])
		
		// step velocity error
		e6[ti] = v_ref[ti] - c.v[ti]
		
		e5_mag = vec.mag(e5[ti])
		
		if obj:
			if obj.mode == control.ObjMode.normal:
				close = all(np.absolute(e5[ti]) < obj.thresh)
				if ti_0 > 1:
					if e5_mag_d[ti] < 0.0:
						if e5_mag_d[ti-1] > 0.0:
							// local maximum error
							if e5_mag < e5_local_max:
								// converging
								if close:
									// converged
									obj.flag_complete = True
							
							e5_local_max = e5_mag
					
					if e5_mag_d[ti] > -0.01:
						if e5_mag_d[ti] < 0.0:
							if close:
								//print 'e5   ',e5[ti]
								//print 'x    ',c.x[ti]
								//print 'x_ref',x_ref[ti]
								obj.flag_complete = True
			
	Position::set_obj( ti, obj):
		obj = obj
		
		// reset
		e5_local_max = 0.0
		flag_converged = False
		
		if isinstance(obj, control.Move):
			fill_xref(ti, obj.x2)
		elif isinstance(obj, control.Path):
			fill_xref_parametric(ti, obj.f) 
		
		
	Position::get_force_rotor( ti, ti_0) {
		
		f6 = get_f6(e5,e6)
		
		m = quad_->m_;
		g = quad_->gravity_;
		
		f_D = quad_->get_force_drag(ti)
		
		// old equation
		//temp1 = np.dot(C5, e6 - np.dot(C5, e5) - np.dot(L5, chi5))
		
		//temp2 = np.dot(L5, e5)

		//f_R = m * (-f6 + temp1 + x_refdd + temp2 - g) - f_D
		
	
		// new equation
		f_R[ti] = m * (
				C1_ * (x_ref_[ti] - x[ti]) +
				C2_ * (x_ref_d_[ti] - v[ti]) +
				C3_ * chi -
				g) - F_D
		
		
		//ver = True
		//ver = False
		//if ver:
			//print 'f6   ' ,f6
			//print 'temp1' ,temp1
			//print 'xrefdd',xrefdd
			//print 'temp2 ',temp2
			//print 'g     ',g
			//print 'f_D   ',f_D
			//print 'f_R   ',f_R

		return f_R;
	}
	//Position::plot(:
		/*
		t = c.t
		
		// f
		fig = pl.figure()
		
		ax = fig.add_subplot(111)
		ax.set_ylabel('f_R')
		
		ax.plot(t, f_R)

		// x
		fig = pl.figure()
		
		x = c.x[:,0]
		y = c.x[:,1]
		z = c.x[:,2]
		xr = x_ref[:,0]
		yr = x_ref[:,1]
		zr = x_ref[:,2]
		
		ax = fig.add_subplot(111)
		ax.set_xlabel('t')
		ax.set_ylabel('x')
		
		ax.plot(t,x,'b-')
		ax.plot(t,y,'g-')
		ax.plot(t,z,'r-')
		ax.plot(t,xr,'b--')
		ax.plot(t,yr,'g--')
		ax.plot(t,zr,'r--')
		
		ax.legend(['x','y','z','xr','yr','zr'])
		
		// v
		fig = pl.figure()
		
		x = c.v[:,0]
		y = c.v[:,1]
		z = c.v[:,2]
		xr = v_ref[:,0]
		yr = v_ref[:,1]
		zr = v_ref[:,2]
		
		ax = fig.add_subplot(111)
		ax.set_xlabel('t')
		ax.set_ylabel('v')
		
		ax.plot(t,x,'b-')
		ax.plot(t,y,'g-')
		ax.plot(t,z,'r-')
		ax.plot(t,xr,'b--')
		ax.plot(t,yr,'g--')
		ax.plot(t,zr,'r--')
		
		ax.legend(['x','y','z','xr','yr','zr'])
		
		// e	
		fig = pl.figure()
		
		ax = fig.add_subplot(221)
		ax.set_ylabel('e_5')
		ax.plot(t, e5)
	
		ax = fig.add_subplot(222)
		ax.set_ylabel('e_6')
		ax.plot(t, e6)
		
		// e5_mag_d
		fig = pl.figure()
		
		ax = fig.add_subplot(111)
		ax.set_ylabel('e5_mag_d')
		ax.plot(t, e5_mag_d)
		
	*/


	

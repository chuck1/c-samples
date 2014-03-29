#include <math/vec3.h>

Attitude::__init__(self, c):
	self.c = c

	self.q_ref = np.empty(c.N, dtype=object);

	C1 = 3.0;

	float* temp = new float[3][3];
	/*
	   {
	   {C1,0.0,0.0},
	   {0.0,C1,0.0},
	   {0.0,0.0,C1}};

	   C1_ = temp;

	   C2 = 2.0;

	   temp = {
	   {C2,0.0,0.0},
	   {0.0,C2,0.0},
	   {0.0,0.0,C2}};

	   C2 = temp
	   */
	e1 = new quat*[N];
	e2 = new vec3*[N];

	e1_mag_d = np.zeros(c.N);

	q_refd = np.zeros((c.N,3));
	q_refdd = np.zeros((c.N,3));
	omega_ref = np.zeros((c.N,3));

	tau_RB = np.zeros((c.N,3));

	obj = None;
	}
Attitude::set_q_reference( ti, q) {
	q_ref[ti] = q
}
Attitude::set_obj( ti1, obj) {
	obj_ = obj

		for (int ti = ti1; it < c.N; it++) q_ref[ti] = obj.q
}	
Attitude::step( ti, ti_0) {
	dt = c.t[ti] - self.c.t[ti-1]

		q = c.q[ti]
		q_ref = q_ref[ti]

		e1[ti] = q_ref * q.conj()

		q_ref_0 = q_ref[ti-1]
		q_ref_1 = q_ref[ti-0]

		// q_refd
		if ti_0 > 1:
			r = q_ref_1 * q_ref_0.conj()
				q_refd_1 = r.to_omega(dt)
				//print 'r',r.s,r.v
		else:
			q_refd_1 = np.zeros(3)		

				// clamp
				q_refd_1 = vec.clamparr(q_refd_1, -1.0, 1.0)
				q_refd[ti] = q_refd_1

				// q_refdd
				if (ti_0 > 2) {
					q_refdd_1 = (q_refd_1 - q_refd[ti-1]) / dt
				} else {
					q_refdd_1 = np.zeros(3)
				}

	q_refdd[ti] = q_refdd_1

		// omega ref
		omega_ref[ti] = self.q_refd[ti]

		// omega error
		e2[ti] = self.omega_ref[ti] - self.c.omega[ti]

		// e1 mag d	
		if (ti_0 > 0) {
			e1_mag_d[ti] = (vec.mag(self.e1[ti].v) - vec.mag(self.e1[ti-1].v)) / dt
		}

	// check objective
	if (ti_0 > 0) {
		if (obj) {
			if (obj.mode == control.ObjMode.normal) {
				if (e1_mag_d[ti] < 0.0) and (self.e1_mag_d[ti] > -0.001) {
					if ((2.0 * math.asin(vec.mag(e1[ti].v))) < self.obj.thresh) {
						obj.flag_complete = True
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

	if np.any(q_refd_1 > 1.0) {
		//prin()
	}
	ver = False
		//ver = True
		if (ver) {
			//prin()
		}
}
Attitude::get_tau_RB( ti, ti_0):
	// require error values
	step(ti, ti_0)

	q_ref = q_ref[ti]

	q = c.q[ti]

	e1 = e1[ti]

	omega = c.omega[ti]
	I = c.I

omegad = np.dot(C1, e1.v) + np.dot(self.C2, self.e2[ti])
	//tau_RB = omegad
tau_RB = np.dot(I, omegad) + np.cross(omega, np.dot(I, omega))

	tau_RB[ti] = tau_RB

	if any(np.isnan(tau_RB)):
		print q_ref.s,q_ref.v
		print q.s,q.v
		print e1.s, e1.v


		raise ValueError('tau_RB nan')

		return tau_RB
		}
Attitude::plot(:
		plot_q()
		plot_omega()
		plot_qrefdd()
		}
		Attitude::plot_q(:
			fig = pl.figure()

			t = c.t

			ax = fig.add_subplot(111)
			ax.plot(c.t, self.tau_RB)
			ax.set_xlabel('t')
			ax.set_ylabel('tau_RB')
			ax.legend(['x','y','z'])

			// orientation
			N = c.N
			q = np.zeros((N,4))
			q_ref = np.zeros((N,4))

			print np.shape(q)

			for i in range(N):
			q[i,1:4] = c.q[i].v
			q[i,0] = c.q[i].s

			if q_ref[i]:
			q_ref[i,1:4] = q_ref[i].v
			q_ref[i,0] = q_ref[i].s

	fig = pl.figure()
ax = fig.add_subplot(111)

print np.shape(q[:,0])

	ax.plot(c.t, q[:,0],'b-')
	ax.plot(c.t, q[:,1],'g-')
	ax.plot(c.t, q[:,2],'r-')
	ax.plot(c.t, q[:,3],'c-')

	ax.plot(c.t, q_ref[:,0],'b--')
	ax.plot(c.t, q_ref[:,1],'g--')
	ax.plot(c.t, q_ref[:,2],'r--')
	ax.plot(c.t, q_ref[:,3],'c--')

	ax.set_xlabel('t')
	ax.set_ylabel('q')
	ax.legend(['a','b','c','d','a','b','c','d'])
	}
Attitude::plot_omega(:
		t = c.t

		fig = pl.figure()
		ax = fig.add_subplot(111)

		//print np.shape(q[:,0])
		omega = c.omega
		q_refd = q_refd

		ax.plot(t, omega[:,0],'b-')
		ax.plot(t, omega[:,1],'g-')
		ax.plot(t, omega[:,2],'r-')

		ax.plot(t, q_refd[:,0],'b--')
		ax.plot(t, q_refd[:,1],'g--')
		ax.plot(t, q_refd[:,2],'r--')

		ax.set_xlabel('t')
		ax.set_ylabel('omega')

ax.legend(['x','y','z','x_q_refd','y_q_refd','z_q_refd'])
}
Attitude::plot_qrefdd() {
	t = c.t

		fig = pl.figure()
		ax = fig.add_subplot(111)

		q_refdd = q_refdd

		ax.plot(t, q_refdd[:,0],'b--')
		ax.plot(t, q_refdd[:,1],'g--')
		ax.plot(t, q_refdd[:,2],'r--')

		ax.set_xlabel('t')
		ax.set_ylabel('q_refdd')

		ax.legend(['x','y','z'])
}




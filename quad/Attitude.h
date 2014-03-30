#include <math/vec3.h>

class Attitude {
	public:
		Attitude(Quadrotor* quad);


		Quadrotor*	quad_;
};

Attitude::Attitude(Quadrotor* quad):
	quad_(quad)
{

	double C1 = 3.0;

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

	math::quat q = quad_->q_[ti]	
	math::vec3 o = quad_->o_[ti]
	
	I = quad_->I_;
	
	math::vec3 od = C1_ * e1_[ti].v + C2_ * e2_[ti] + q_ref_dd_[ti];
	
	//tau_RB = omegad
	tau_RB_[ti] = I * od + o.cross(I * o)
	
	
	//if any(np.isnan(tau_RB)):
		//print q_ref.s,q_ref.v
		//print q.s,q.v
		//print e1.s, e1.v
		//raise ValueError('tau_RB nan')

	return tau_RB;
}


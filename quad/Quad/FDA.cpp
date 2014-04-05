
#include <math/vec3.h>
#include <math/quat.h>

#include <FDA.h>

void zero(math::vec3& a) {
	a.LoadZero();
}
void zero(double& a) {
	a = 0.0;
}

math::quat diff(math::quat const & a, math::quat const & b) {
	return (a * b.getConjugate());
}



void forward_quavec(math::quat* q, math::vec3* qd, double h, int ti, int ti_0, int pre, double tc/* = 0.0*/) {
	
	if (ti_0 > (pre + 1)) {
		math::quat r = diff(q[ti], q[ti-1]);
		
		//qd[ti] = r.getOmega(h) * 2.0 - qd[ti-1];
		qd[ti] = r.getOmega(h);
	} else if(ti_0 > (pre + 0)) {
		math::quat r = diff(q[ti], q[ti-1]);
		
		qd[ti] = r.getOmega(h);
	}
}





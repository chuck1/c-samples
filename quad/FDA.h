#ifndef __FDA__
#define __FDA__

#include <math/vec3.h>
#include <math/quat.h>

void zero(math::vec3& a);
void zero(double& a);
math::quat diff(math::quat const & a, math::quat const & b);


template<typename T> void low_pass(T* x, T* y, int ti, double dt, double tc) {
	if(ti > 3) {
		//v[ti] = v[ti-2] * 0.25 + v[ti-1] * 0.5 + v[ti] * 0.25;
		
		y[ti] =  (y[ti-1] * tc + x[ti] * dt) / (tc + dt);
	} else if(ti > 0) {
		y[ti] =  (y[ti-1] * tc + x[ti] * dt) / (tc + dt);
	}
}


template<typename T> void forward(T* v, T* vd, double h, int ti, int ti_0, int pre) {
	if(ti_0 > (pre + 1)) {
		//vd[ti] = (v[ti] - v[ti-1]) * 2.0 / h - vd[ti-1];
		
		vd[ti] = ((v[ti] - v[ti-1]) / h + vd[ti-1]) / 2.0;

		//vd[ti] = vd[ti-1] + (v[ti] - v[ti-1] * 2.0 + v[ti-2]) / (2.0 * h);
		
		//vd[ti] = (v[ti] - v[ti-1]) / h;
	} else if(ti_0 > (pre + 0)) {
		vd[ti] = (v[ti] - v[ti-1]) / h;
	} else {
		zero(vd[ti]);
	}
}



void forward_quavec(math::quat* v, math::vec3* vd, double h, int ti, int ti_0, int pre, double tc = 0.0);

#endif

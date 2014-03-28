#include <stdio.h>

#define _DEBUG 0

#include <math/vec3.h>


#include <Quadrotor.h>
#include <Brain.h>


int main() {

	double dt = 0.01;
	int N = 2000;

	float* t = new float[N];

	for(int ti = 0; ti < N; ti++) t[ti] = ti * dt;
	
	Quadcopter* c = new Quadcopter(t);
	Brain* b = new Brain(c);
	
	//b.ctrl_position.fill_xref([1.0, 1.0, 0.0])
	/*
	   """
	   b.objs = [
	   control.Move([1.0,0.0,0.0],[0.01,0.01,0.01]),
	   control.Move([1.0,1.0,0.0],[0.01,0.01,0.01]),
	   control.Move([0.0,1.0,0.0],[0.01,0.01,0.01]),
	   control.Move([0.0,0.0,0.0],[0.01,0.01,0.01]),
	   control.Move([0.0,0.0,1.0],[0.01,0.01,0.01]),
	   control.Move([1.0,0.0,1.0],[0.01,0.01,0.01]),
	   control.Move([1.0,1.0,1.0],[0.01,0.01,0.01]),
	   control.Move([0.0,1.0,1.0],[0.01,0.01,0.01]),
	   control.Move([0.0,0.0,1.0],[0.01,0.01,0.01])]
	   """
	   """
	   b.objs = [
	   control.Orient(qt.Quat(theta = math.pi, v = [1.0, 0.0, 0.0]), mode = control.ObjMode.hold)]
	   """
	   */

	b.objs.push_back(control.Path(lambda t: np.array([math.sin(t * 4.0 / math.pi),t,0.0])));
	
	
	for(int ti = 1; it < N) {

		if ((ti % (N / 100)) == 0) {
			printf("%i\n",ti);
		}
	}

	b.step(ti-1);
	c.step(ti);

	c.plot();

	b.ctrl_position.plot();
	b.ctrl_attitude.plot();


	b.plot();

	c.plot3();

	pl.show();

}


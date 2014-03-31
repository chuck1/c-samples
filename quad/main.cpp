#include <stdio.h>
#include <cstdlib>

#define _DEBUG 0

#include <math/vec3.h>


#include <Quadrotor.h>
#include <Brain.h>
#include <Simulation.h>

math::vec3 sinewave(double t) {
	return math::vec3(sin(t * 4.0 / M_PI),t,0.0);
}

int main(int argc, const char ** argv) {
	
	if(argc != 2) printf("usage: %s <time_steps>\n",argv[0]);
	
	double dt = 0.01;
	int N = atoi(argv[1]);
	
	
	Simulation* s = new Simulation(dt, N);
	
	s->b_->objs_.push_back(
			new Command::Move(math::vec3(1,0,0))
			);


	s->run();

	//b->pos_->write();
	//b->att_->write();
}




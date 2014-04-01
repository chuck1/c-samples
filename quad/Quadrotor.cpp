#include <Attitude.h>
#include <Brain.h>
#include <Telem.h>
#include <Plant.h>
#include <Quadrotor.h>
#include <Position.h>


Quadrotor::Quadrotor(double dt, int N):
	dt_(dt),
	N_(N)
{
	t_ = new double[N_];
	for(int ti = 0; ti < N_; ti++) t_[ti] = dt * (float)ti;

	telem_ = new Telem(this, t_, N_);

	plant_ = new Plant(this, t_, N_);

	brain_ = new Brain(this);
}

void Quadrotor::run() {

	for(int ti = 1; ti < N_; ti++) {

		if ((ti % (N_ / 100)) == 0) {
			//printf("%i %f\n",ti,t[ti]);
		}
		
		brain_->step(ti-1);
		
		plant_->step(ti);
		
		//math::vec3 a	= plant_->a_[ti];
		//math::vec3 od	= plant_->od_[ti];
		
		telem_->step(ti);
	}

}

void Quadrotor::write() {
	brain_->pos_->write();
	brain_->att_->write();
}


void product(int choices, int repeat, int*& arr, int level) {
	
	int len = pow(choices, repeat);
	
	if(level == 0) {
		arr = new int[len * repeat];
	}
	
	int mod = pow(choices, level);
	
	for(int a = 0; a < len; a++) {
		int b = (a - (a % mod))/mod % choices;
		//printf("%i ", b);
		arr[a*repeat + level] = b;
	}
	//printf("\n");
	
	if(level < (repeat-1)) {
		product(choices, repeat, arr, level + 1);
	}
}




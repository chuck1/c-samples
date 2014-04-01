#include <stdio.h>
#include <cstdlib>

#define _DEBUG 0

#include <math/vec3.h>

#include <Attitude.h>
#include <Position.h>
#include <Quadrotor.h>
#include <Brain.h>

math::vec3 sinewave(double t) {
	return math::vec3(sin(t * 4.0 / M_PI),t,0.0);
}

void map() {
	int* arr;

	int choices = 2;
	int repeat = 4;
	int len = pow(choices, repeat);

	product(choices, repeat, arr);
	
	/*
	double coeff[] = {
		1.0, 2.0, 3.0, 4.0,
		1.0, 2.0, 3.0, 4.0,
		1.0, 2.0, 3.0, 4.0,
		1.0, 2.0, 3.0, 4.0};
	*/
	


	double coeff[] = {
		1.0, 2.0,
		1.0, 2.0,
		1.0, 2.0,
		1.0, 2.0,};
	
	
	
	double dt = 0.01;
	int N = 1000;

	float C1, C2, C3, C4;

	for(int a = 0; a < len; a++) {
		
		C1 = coeff[0*choices + arr[a*repeat + 0]];
		C2 = coeff[1*choices + arr[a*repeat + 1]];
		C3 = coeff[2*choices + arr[a*repeat + 2]];
		C4 = coeff[3*choices + arr[a*repeat + 3]];
		
		//printf("%i %f %f %f %f\n",a,C1,C2,C3,C4);
		
		Quadrotor* r = new Quadrotor(dt, N);
		
		r->brain_->pos_->C1_.v[3*0 + 0] = C1;
		r->brain_->pos_->C1_.v[3*1 + 1] = C1;
		r->brain_->pos_->C1_.v[3*2 + 2] = C1;

		r->brain_->pos_->C2_.v[3*0 + 0] = C2;
		r->brain_->pos_->C2_.v[3*1 + 1] = C2;
		r->brain_->pos_->C2_.v[3*2 + 2] = C2;
		
		r->brain_->att_->C1_.v[3*0 + 0] = C3;
		r->brain_->att_->C1_.v[3*1 + 1] = C3;
		r->brain_->att_->C1_.v[3*2 + 2] = C3;

		r->brain_->att_->C2_.v[3*0 + 0] = C4;
		r->brain_->att_->C2_.v[3*1 + 1] = C4;
		r->brain_->att_->C2_.v[3*2 + 2] = C4;
		
		
		r->brain_->objs_.push_back(
				new Command::Move(math::vec3(1,0,0), math::vec3(0.01,0.01,0.01))
				);


		try {
			r->run();
		} catch(Command::EmptyQueue& e) {
			//printf("caught\n");
		} catch(...) {
			//printf("unknwon error\n");
		}

		Command::Move* move = (Command::Move*)(r->brain_->pos_->pos_);

		printf("ts %f\n",move->ts_);


	}
}

void normal(int N) {

	double dt = 0.01;

	Quadrotor* r = new Quadrotor(dt, N);

	r->brain_->objs_.push_back(
			new Command::Move(math::vec3(1,0,0), math::vec3(0.01,0.01,0.01))
			);

	r->run();

	r->write();
}

int main(int argc, const char ** argv) {

	//int N = atoi(argv[1]);

	//if(argc != 2) {
	//	printf("usage: %s <time_steps>\n",argv[0]);
	//	exit(0);
	//}


	//normal(10000);
	map();

	//b->att_->write();
}




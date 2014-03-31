
class Simulation {
	public:
		Simulation(double dt, int N);
		void	run();

		double	dt_;
		int	N_;
		double*	t_;

		Quadrotor*	r_;
		Brain*		b_;

};



Simulation::Simulation(double dt, int N):
	dt_(dt),
	N_(N)
{
	t_ = new double[N_];
	for(int ti = 0; ti < N_; ti++) t_[ti] = dt * (float)ti;

	r_ = new Quadrotor(t_, N_);

	b_ = new Brain(r_);
}

void Simulation::run() {

	for(int ti = 1; ti < N_; ti++) {

		if ((ti % (N_ / 100)) == 0) {
			//printf("%i %f\n",ti,t[ti]);
		}

		b_->step(ti-1);
		r_->step(ti);
	}

}



#include <math/mat33.h>

#include <Brain.h>
#include <Objective.h>

class Position {
public:
	Position(Quadrotor*);

	void		fill_xref(int ti1, math::vec3 x);
	void		fill_xref_parametric(int ti1, math::vec3 (*f)(double));

	void		step(int ti, int ti_0);

	void		set_obj(int ti, Command::Position* pos);
	
	math::vec3	get_force_rotor(int ti, int ti_0);

	
	Quadrotor*	quad_;

	Command::Position*	pos_;

	math::mat33	C1_;
	math::mat33	C2_;
	math::mat33	C3_;

	math::vec3*	e1_;

	math::vec3*	chi_;

	double*		e1_mag_d_;


	math::vec3*	x_ref_;
	math::vec3*	x_ref_d_;
	math::vec3*	x_ref_dd_;

	math::vec3*	f_R_;

	unsigned int	flag_;
};


Position::Position(Quadrotor* quad):
	flag_(0)
{

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
			0,0,C1_33);
	C2_ = math::mat33(
			C2_11,0,0,
			0,C2_22,0,
			0,0,C2_33);
	C3_ = math::mat33(
			C3_11,0,0,
			0,C3_22,0,
			0,0,C3_33);

	int N = quad_->N_;

	chi_ = new math::vec3[N];

	e1_ = new math::vec3[N];

	e1_mag_d_ = new double[N];

	x_ref_ = new math::vec3[N];
	x_ref_d_ = new math::vec3[N];
	x_ref_dd_ = new math::vec3[N];

	f_R_ = new math::vec3[N];

}
void Position::fill_xref(int ti1, math::vec3 x) {
	for (int ti = ti1; ti < quad_->N_; ti++) x_ref_[ti] = x;
}
void Position::fill_xref_parametric(int ti1, math::vec3 (*f)(double)) {
	for(int ti = 0; ti < quad_->N_; ti++) {
		double t = quad_->t_[ti];
		x_ref_[ti] = f(t);
	}
}
//pl.plot(c.t, x_ref)

void Position::step(int ti, int ti_0) {
	double dt = quad_->t_[ti] - quad_->t_[ti-1];
	
	// reference
	x_ref_d_[ti] = (x_ref_[ti] - x_ref_[ti-1]) / dt;
	
	if (ti_0 > 1) x_ref_dd_[ti] = (x_ref_d_[ti] - x_ref_d_[ti-1]) / dt;
	
	// step position error
	e1_[ti] = x_ref_[ti] - quad_->x_[ti];
	
	if (ti_0 > 0) chi_[ti] = chi_[ti-1] + e1_[ti] * dt;
	
	if (ti_0 > 0) e1_mag_d_[ti] = (e1_[ti].magnitude() - e1_[ti-1].magnitude()) / dt;
	
	
	double e1_mag = e1_[ti].magnitude();
	
	
	
	
	bool close = e1_[ti].abs_less(pos_->thresh_);
	
	if (pos_) {
		if (pos_->mode_ & Command::Position::Mode::NORMAL) {
			if (ti_0 > 1) {
				if (e1_mag_d_[ti] > -0.01) {
					if (e1_mag_d_[ti] < 0.0) {
						if (close) {
							//print 'e5   ',e5[ti]
							//print 'x    ',c.x[ti]
							//print 'x_ref',x_ref[ti]
							pos_->flag_ |= Command::Position::Flag::COMPLETE;
						}
					}
				}
			}
		}
	}
}
void Position::set_obj(int ti, Command::Position* pos) {
	pos_ = pos;
	
	// reset
	flag_ &= ~Command::Position::Flag::COMPLETE;
	
	Command::Move* move;
	Command::Path* path;

	switch(pos_->type_) {
		case Command::Position::Type::MOVE:
			move = (Command::Move*)pos_;
			fill_xref(ti, move->x2_);
		case Command::Position::Type::PATH:
			path = (Command::Path*)pos_;
			fill_xref_parametric(ti, path->f_);
	}

}
math::vec3 Position::get_force_rotor(int ti, int ti_0) {

	double m = quad_->m_;
	math::vec3 g = quad_->gravity_;

	math::vec3 f_D = quad_->get_force_drag(ti);

	// old equation
	//temp1 = np.dot(C5, e6 - np.dot(C5, e5) - np.dot(L5, chi5))

	//temp2 = np.dot(L5, e5)

	//f_R = m * (-f6 + temp1 + x_refdd + temp2 - g) - f_D


	// new equation
	f_R_[ti] = m * (
			C1_ * (x_ref_[ti] - quad_->x_[ti]) +
			C2_ * (x_ref_d_[ti] - quad_->v_[ti]) +
			C3_ * chi_[ti] -
			g) - f_D;


		//ver = True
		//ver = False
		//if ver) {
		//print 'f6   ' ,f6
		//print 'temp1' ,temp1
		//print 'xrefdd',xrefdd
		//print 'temp2 ',temp2
		//print 'g     ',g
		//print 'f_D   ',f_D
		//print 'f_R   ',f_R

		return f_R_[ti];
}
//Position) {:plot(:
/*
   t = c.t

// f
fig = pl.figure()

ax = fig.add_subplot(111)
ax.set_ylabel('f_R')

ax.plot(t, f_R)

// x
fig = pl.figure()

x = c.x[) {,0]
y = c.x[) {,1]
z = c.x[) {,2]
xr = x_ref[) {,0]
yr = x_ref[) {,1]
zr = x_ref[) {,2]

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

x = c.v[) {,0]
y = c.v[) {,1]
z = c.v[) {,2]
xr = v_ref[) {,0]
yr = v_ref[) {,1]
zr = v_ref[) {,2]

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




#ifndef __ATTITUDE__
#define __ATTITUDE__

#include <math/vec3.h>
#include <math/quat.h>
#include <math/mat33.h>

#include <Quad/Array.h>

class Quadrotor;

namespace Command {
	class Orient;
}

class Attitude {
	public:
		Attitude(Quadrotor* quad);
		void		reset();
		void		set_q_reference(int ti, math::quat q);
		void		set_obj(int ti1, Command::Orient* att);
		void		step(int ti, int ti_0);
		void		get_tau_RB(int ti, int ti_0);
		void		write(int n = 0);
		void		write_param();
		void		read_param();


		Quadrotor*			quad_;
		Command::Orient*		att_;	
		
		math::mat33			C1_;
		math::mat33			C2_;
		
		Array<math::quat>		e1_;
		Array<math::vec3>		e2_;
		
		
		Array<math::quat>		q_ref_;
		
		struct {
			Array<math::vec3>	q_ref_d_;
			Array<math::vec3>	q_ref_dd_;
		} filt, unfilt;
		

		Array<double>			e1_mag_;
		Array<double>			e1_mag_d_;

		Array<math::vec3>		tau_RB_;



};

#endif

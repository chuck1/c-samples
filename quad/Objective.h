#ifndef __OBJECTIVE__
#define __OBJECTIVE__




namespace Command {
	class Base {
		public:
			struct Flag {
				enum e {
					RISE = 1 << 0,
					SETTLE = 1 << 1,
					COMPLETE = 1 << 2,
				};
			};

			struct Mode {
				enum e {
					NORMAL,
					HOLD
				};
			};

			Base(unsigned int);

			unsigned int	flag_;
			unsigned int	mode_;
	};

	class Position: public Base {
		public:
			Position(Command::Position::Mode::e);
			Position(Command::Position::Mode::e, math::vec3 thresh_);
			
			math::vec3	thresh_;
	};


	class Move: public Command::Position {
		public:
			math::vec3	x2_;


			Move(math::vec3 x2, math::vec3 thresh):
				Command::Position(Command::Position::Mode::NORMAL, thresh),
				x2_(x2)
		{
		}
			Move(math::vec3 x2):
				Command::Position(Command::Position::Mode::HOLD),
				x2_(x2)
		{
		}
	};

	class Path: public Position {
		public:
			math::vec3 (*f_)(double);

			Path(math::vec3 (*f)(double)):
				Position(Position::Mode::HOLD),
				f_(f)
		{
		}
	};

	class Orient: public Base {
		public:
			math::quat	q_;
			double		thresh_;

			Orient(math::quat q, double thresh):
				Base(Base::Mode::NORMAL),
				q_(q), thresh_(thresh)
		{
		}
			Orient(math::quat q):
				Base(Base::Mode::HOLD)
		{
		}
	};	

}



#endif

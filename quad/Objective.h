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

			struct Type {
				enum e {
					MOVE,
					PATH,
					ORIENT
				};
			};

			Base(Type::e, Mode::e);

			unsigned int	flag_;
			unsigned int	mode_;
			unsigned int	type_;
	};

	class Position: public Base {
		public:
			Position(Base::Type::e type, Base::Mode::e);
			Position(Base::Type::e type, Base::Mode::e, math::vec3 thresh_);
			
			math::vec3	thresh_;
	};


	class Move: public Command::Position {
		public:
			math::vec3	x2_;


			Move(math::vec3 x2, math::vec3 thresh);
			Move(math::vec3 x2);
	};

	class Path: public Position {
		public:
			Path(math::vec3 (*f)(double));

			math::vec3 (*f_)(double);
	};
	
	
	class Orient: public Base {
		public:
			math::quat	q_;
			double		thresh_;

			Orient(math::quat q, double thresh);
			Orient(math::quat q);
	};	

}

Command::Base::Base(Command::Base::Type::e type, Command::Base::Mode::e mode):
	flag_(0),
	type_(type),
	mode_(mode)
{
}

Command::Position::Position(Command::Base::Type::e type, Command::Base::Mode::e mode):
	Command::Base(type, mode)
{
}

Command::Position::Position(Command::Base::Type::e type, Command::Base::Mode::e mode, math::vec3 thresh):
	Command::Base(type, mode),
	thresh_(thresh)
{
}

Command::Move::Move(math::vec3 x2, math::vec3 thresh):
	Command::Position(Command::Base::Type::MOVE, Command::Position::Mode::NORMAL, thresh),
	x2_(x2)
{
}

Command::Move::Move(math::vec3 x2):
	Command::Position(Command::Base::Type::MOVE, Command::Position::Mode::HOLD),
	x2_(x2)
{
}

Command::Path::Path(math::vec3 (*f)(double)):
	Position(Command::Base::Type::PATH, Command::Base::Mode::HOLD),
	f_(f)
{
	printf("path %p\n",this);
	printf("f %p\n",f_);
	
	if (f_ == NULL) throw;
}

Command::Orient::Orient(math::quat q, double thresh):
	Base(Base::Type::ORIENT, Base::Mode::NORMAL),
	q_(q),
	thresh_(thresh)
{
}

Command::Orient::Orient(math::quat q):
	Base(Base::Type::ORIENT, Base::Mode::HOLD)
{
}


#endif

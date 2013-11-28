#ifndef __PLANE_H__
#define __PLANE_H__

#include <math/mat44.h>
#include <math/plane.h>

class plane
{
	public:
		plane():
			pose_(),
			plane_(math::vec3(0,1,0),0)
	{}

		void draw();

		int useTexture;


		math::mat44 pose_;
		math::plane plane_;
};

#endif

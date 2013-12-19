#include <stdio.h>

#include <math/geo/polyhedron.h>

#include <glutpp/master.h>
#include <glutpp/window.h>
#include <glutpp/sphere.h>

int	main()
{
	int sl = 15;
	
	math::geo::sphere s(0.5,sl,sl);
	
	glutpp::window w( 600, 600, 200, 100, "window");
	
	w.set(glutpp::window::SHADER);
	w.set(glutpp::window::LIGHTING);

	w.camera_.eye_ = math::vec4(0,0,5,0);

	printf("w = %p\n",&w);

	w.init();
	
	// light
	glutpp::light l;
	
	l.camera_.eye_ = math::vec4(8.0,8.0,0.0,1.0);
	
	w.add_light(&l);
	
	// object
	glutpp::object o;
	
	o.construct(&s);
	
	//o.load("cube.obj");
	
	w.add_object(&o);

	
	w.StartSpinning();
	
	glutpp::__master.CallGlutMainLoop();
	
	return 0;
}

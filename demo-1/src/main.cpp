#include <stdio.h>

#include <glutpp/master.h>
#include <glutpp/window.h>
#include <glutpp/sphere.h>

class window:
	public glutpp::window
{
public:
	window( int w, int h, int x, int y, const char * title):
		glutpp::window(w,h,x,y,title)
	{
		printf("%s\n",__PRETTY_FUNCTION__);

		camera_.eye_ = math::vec4(0,5,5,0);
		
		set( PLANAR_REFLECTION );
		set( SHADOW | SHADOW_TEXTURE );
		
		// light
		glutpp::light* light = new glutpp::light(this, GL_LIGHT0);
		light->camera_.eye_ = math::vec4(1.0,7.0,0.0,1.0);
		
		// sphere
		glutpp::sphere* sphere = new glutpp::sphere(this);

		// plane
		glutpp::plane* plane = new glutpp::plane(this);
		plane->plane_.n = math::vec3(0.0,1.0,0.0);
		
		lights_.push_back(light);
		
		objects_.push_back(sphere);
		objects_.push_back(plane);
	}
};

int	main()
{
	
	window w( 600, 600, 200, 100, "window");





	
	// enable idle function
	w.StartSpinning();
	
	glutpp::__master.CallGlutMainLoop();

	return 0;
}


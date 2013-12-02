#include <stdio.h>

#include <glutpp/master.h>
#include <glutpp/window.h>

class window:
	public glutpp::window
{
public:
	window( int w, int h, int x, int y, const char * title):
		glutpp::window(w,h,x,y,title)
	{
		camera_.eye_ = math::vec4(0,5,-5,0);
		
		light_.camera_.eye_ = math::vec4(0,5,0,0);
	}
	void	Display()
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, math::white);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, math::blue);
        	glMaterialfv(GL_FRONT, GL_SPECULAR, math::white);
	        glMaterialf(GL_FRONT, GL_SHININESS, 16.0f);
		
		glutSolidSphere(0.5,20,20);

		glTranslatef(2,0,0);
		
		glutSolidCube(1.0);
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


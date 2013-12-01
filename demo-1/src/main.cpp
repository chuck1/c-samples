#include <glutpp/master.h>
#include <glutpp/window.h>

class window:
	public glutpp::window
{
	window( int w, int h, int x, int y, const char * title):
		glutpp::window(w,h,x,y,title)
	{
	}
	/*	void GRU::DemoWindow::CallBackDisplayFunc(void){

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glColor4f(1.0, 0.0, 0.0, 1.0);
		glutWireSphere(50, 10, 10);

		glutSwapBuffers();
		}*/
	void	Display()
	{
		glutSolidSphere(1.0,10,10);
	}
};

int	main()
{

	glutpp::window w0( 200, 200, 200, 100, "First Window");
	
	glutpp::window w1( 200, 200, 200, 100, "First Window");
	
	// enable idle function
	w0.StartSpinning();
	
	glutpp::__master.CallGlutMainLoop();

	return 0;
}


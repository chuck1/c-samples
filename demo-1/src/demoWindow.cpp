

#include <GRU/demoWindow.h>

GRU::DemoWindow::DemoWindow( GRU::Master * glutMaster,
		int setWidth, int setHeight,
		int setInitPositionX, int setInitPositionY,
		const char * title)
{

	width  = setWidth;               
	height = setHeight;

	initPositionX = setInitPositionX;
	initPositionY = setInitPositionY;

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(initPositionX, initPositionY);
	glViewport(0, 0, width, height);   // This may have to be moved to after the next line on some platforms

	glutMaster->CallGlutCreateWindow( (char *)title, this );

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glOrtho(-80.0, 80.0, -80.0, 80.0, -500.0, 500.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(60, 1, 1, 1);
	glColor4f(1.0, 0.0, 0.0, 1.0);
}
GRU::DemoWindow::~DemoWindow()
{
	glutDestroyWindow(windowID);
}
void GRU::DemoWindow::CallBackDisplayFunc(void){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor4f(1.0, 0.0, 0.0, 1.0);
	glutWireSphere(50, 10, 10);

	glutSwapBuffers();
}
void GRU::DemoWindow::CallBackReshapeFunc(int w, int h){

	width = w;
	height= h;

	glViewport(0, 0, width, height); 
	CallBackDisplayFunc();
}
void GRU::DemoWindow::CallBackIdleFunc(void)
{
	glRotatef(0.25, 1, 1, 2);
	CallBackDisplayFunc();
}
void GRU::DemoWindow::StartSpinning( GRU::Master * glutMaster )
{

	glutMaster->SetIdleToCurrentWindow();
	glutMaster->EnableIdleFunction();
}









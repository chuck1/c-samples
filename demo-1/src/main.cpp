
#include <GRU/Master.h>
#include <GRU/Window.h>
#include <GRU/demoWindow.h>

GRU::DemoWindow     * firstWindow       = 0;
GRU::DemoWindow     * secondWindow      = 0;

int	main(void)
{

	firstWindow  = new GRU::DemoWindow( &GRU::master,
			200, 200,		// height, width
			200, 100,		// initPosition (x,y)
			"First Window");	// title

	secondWindow = new GRU::DemoWindow( &GRU::master,
			500, 500,		// height, width
			200, 400,		// initPosition (x,y)
			"Second Window");	// title
	
	// enable idle function
	secondWindow->StartSpinning( &GRU::master );
	
	GRU::master.CallGlutMainLoop();
	
	return 0;
}




#include <GRU/Master.h>
#include <GRU/Window.h>
#include <GRU/demoWindow.h>

#include <NEB/Physics.h>
#include <NEB/Scene.h>
#include <NEB/Actor/Rigid_Dynamic_Box.h>
#include <NEB/View.h>

int	main()
{
	
	// NEB
	NEB::physics.Init();
	
	NEB::Scene * scene = new NEB::Scene();
	
	NEB::Actor::Rigid_Dynamic_Box * box = scene->Create_Rigid_Dynamic_Box();

	box = box;
	
	NEB::View * view = new NEB::View;
	
	// GRU
	
	GRU::Window * window  = new GRU::Window(
			&GRU::master,
			200, 200,		// height, width
			200, 100,		// initPosition (x,y)
			"First Window" );	// title

	
	window->StartSpinning( &GRU::master );
	
	// main
	
	view->SetWindow( window );
	view->scene_ = scene;
	
	GRU::master.CallGlutMainLoop();
	
	return 0;
}



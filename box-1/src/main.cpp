/* TODO
 * - cleanly destroy actors and scenes (so that scene can be reset in-game)
 * - automate connection of camera (and other objects) to devices (via JSL::Master)
 * - cleanly exit (JSL read threads, glut windows)
 * - create terminal (via GUL)
 * - make command line revolve around objects (ex. command to change camera mode or target would be directed to the camera)
 *   - help command to print list of objects listening to terminal
 *   - another idea: have program listen to actual terminal (/dev/ttyX)
 * - focus tracking for windows to suspend signals when appropriate
 */

#include <functional>

//#include <JSL/Master.h>
//#include <JSL/Event.h>

#include <glutpp/master.h>
#include <glutpp/window.h>

#include <neb/physics.h>
#include <neb/scene.h>
#include <neb/actor/Rigid_Dynamic.h>
#include <neb/view.h>
#include <neb/camera.h>


int	main(int argc, char const ** argv)
{

	// JSL
	JSL::master.Init();


	// NEB
	NEB::physics.Init();

	TiXmlDocument document("scene.xml");
	if ( !document.LoadFile() )
	{
		printf ("XML file not found\n");
		return 0;
	}
	
	TiXmlElement* el_scene = document.FirstChildElement("scene");
	
	std::shared_ptr<NEB::Scene> scene = NEB::physics.Create_Scene(el_scene);
	
	NEB::View * view = new NEB::View;
	NEB::Camera * camera = new NEB::Camera;

	// GRU

	GRU::Window * window = new GRU::Window(
			&GRU::master,
			600, 600,		// height, width
			200, 100,		// initPosition (x,y)
			"First Window" );	// title

	// main

	camera->SetWindow(window);
	camera->view_ = view;

	camera->Connect();

	
	view->scene_ = scene;



	window->StartSpinning();




	// Run

	JSL::master.Launch();

	GRU::master.CallGlutMainLoop();

	JSL::master.Join();

	return 0;
}




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
//#include <neb/camera.h>


int	main(int argc, char const ** argv)
{

	// JSL
	//JSL::master.Init();


	// NEB
	neb::__physics.Init();

	TiXmlDocument document("scene.xml");
	if ( !document.LoadFile() )
	{
		printf ("XML file not found\n");
		return 0;
	}
	
	TiXmlElement* el_scene = document.FirstChildElement("scene");
	
	neb::scene* scene = neb::__physics.Create_Scene(el_scene);
	
	neb::view * view = new neb::view;
	//neb::camera * camera = new NEB::Camera;

	// GRU

	glutpp::window w(600, 600, 200, 100, "First Window");

	// main

	//camera->SetWindow(window);
	//camera->view_ = view;

	//camera->Connect();

	
	view->scene_ = scene;



	w.StartSpinning();




	// Run

	//JSL::master.Launch();

	glutpp::__master.CallGlutMainLoop();

	//JSL::master.Join();

	return 0;
}




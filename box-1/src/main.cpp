/* TODO
 * - create a JSL::Master class. On init, finds available devices and makes a list.
 * - scene setup using xml
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

#include <GRU/Master.h>
#include <GRU/Window.h>

#include <NEB/Physics.h>
#include <NEB/Scene.h>
#include <NEB/Actor/Rigid_Dynamic_Box.h>
#include <NEB/View.h>
#include <NEB/Camera.h>

#include <JSL/Event.h>

int	main(int argc, char const ** argv)
{



	// NEB
	NEB::physics.Init();

	NEB::Scene* scene = NEB::physics.Create_Scene();

	NEB::View * view = new NEB::View;
	NEB::Camera * camera = new NEB::Camera;

	// Actors

	/*	NEB::Actor::Rigid_Dynamic_Box* box =		*/scene->Create_Light();


	NEB::Actor::Rigid_Dynamic_Box* box =		scene->Create_Rigid_Dynamic_Box();
	box->pose_ = physx::PxTransform( physx::PxVec3(2.0f, 0.0f, 0.0f) );


	NEB::Actor::Rigid_Static_Plane* plane =		scene->Create_Rigid_Static_Plane();
	plane->pose_ = physx::PxTransform( physx::PxVec3(0.0f, -5.0f, 0.0f) );




	// GRU

	GRU::Window * window = new GRU::Window(
			&GRU::master,
			600, 600,		// height, width
			200, 100,		// initPosition (x,y)
			"First Window" );	// title

	// main

	camera->SetWindow(window);
	camera->view_ = view;




	view->scene_ = scene;



	window->StartSpinning();



	// JSL
	JSL::Event ev_mouse, ev_keyboard;

	ev_mouse.Open("/dev/input/event6");
	ev_keyboard.Open("/dev/input/event7");

	//ev_mouse.Set( JSL::Event::flag::PRINT );

	ev_mouse.map_sig_rel_[REL_X].connect(	std::bind( &NEB::Camera::FirstOrderDeltaYawRel,		camera, std::placeholders::_1 ) );
	ev_mouse.map_sig_rel_[REL_Y].connect(	std::bind( &NEB::Camera::FirstOrderDeltaPitchRel,	camera, std::placeholders::_1 ) );
	ev_keyboard.sig_key_.connect( 		std::bind( &NEB::Camera::HandleKey,			camera, std::placeholders::_1, std::placeholders::_2 ) );

	ev_mouse.Launch();
	ev_keyboard.Launch();


	// Run

	GRU::master.CallGlutMainLoop();

	ev_mouse.Join();
	ev_keyboard.Join();

	return 0;
}




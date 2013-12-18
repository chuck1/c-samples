/* 
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
#include <glutpp/renderable.h>
#include <glutpp/scene.h>

#include <neb/app.h>
#include <neb/user.h>
#include <neb/physics.h>
#include <neb/scene.h>
#include <neb/actor/Rigid_Body.h>
#include <neb/actor/Rigid_Dynamic.h>
#include <neb/view.h>
#include <neb/camera.h>

enum box
{
	SCENE_0,
	LAYOUT_0
};



int	main(int argc, char const ** argv)
{
	neb::__physics.Init();
	
	std::shared_ptr<neb::app> app(new neb::app);
	app->init();
	app->load_scene(box::SCENE_0, "scene.xml");
	
	//app->activate_scene(box::SCENE_0);
	
	
	
	std::shared_ptr<neb::user> user(new neb::user);
	
	auto actor = std::dynamic_pointer_cast<neb::actor::Base>(app->scenes_[box::SCENE_0]->actors_.at(0));
	
	
	user->set_actor(actor, neb::camera_type::e::RIDEALONG);
	user->connect(app->window_);
	
	
	
	
	printf("loop\n");
	app->window_->prepare();
	app->window_->loop();
	
	
	return 0;
}




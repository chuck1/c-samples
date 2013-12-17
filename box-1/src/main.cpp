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





int	main(int argc, char const ** argv)
{
	//JSL::master.Init();

	// NEB
	neb::__physics.Init();

/*	TiXmlDocument document("scene.xml");
	if ( !document.LoadFile() )
	{
		printf ("XML file not found\n");
		return 0;
	}

	TiXmlElement* el_scene = document.FirstChildElement("scene");
*/
	
	std::shared_ptr<neb::app> app(new neb::app);
	app->init();
	app->load("scene.xml");


	
	std::shared_ptr<glutpp::light> l0(new glutpp::light);
	std::shared_ptr<glutpp::light> l1(new glutpp::light);

	l0->camera_.eye_ = math::vec4( 3.0,0.0,0.0,1.0);
	l1->camera_.eye_ = math::vec4(-3.0,0.0,0.0,1.0);

	l0->diffuse_ = math::color(0.6, 0.6, 0.6, 1.0);
	l1->diffuse_ = math::color(0.6, 0.6, 0.6, 1.0);
	l0->atten_linear_ = 1.0;
	l1->atten_linear_ = 1.0;

	
	app->window_->renderable_->scene_->add_light(l0);
	app->window_->renderable_->scene_->add_light(l1);

	
	
	
	std::shared_ptr<neb::camera_ridealong> ride(new neb::camera_ridealong);
	
	std::shared_ptr<neb::user> user(new neb::user);
	user->actor_ = app->scene_->actors_.at(0);
	user->camera_control_ = ride;
	
	ride->actor_ = user->actor_;
	
	
	user->connect(app->window_);
	
	
	

	printf("control\n");
	app->window_->renderable_->scene_->camera_.control_ = user->camera_control_;

	printf("loop\n");
	app->window_->loop();


	return 0;
}




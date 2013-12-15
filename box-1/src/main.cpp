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

#include <neb/user.h>
#include <neb/physics.h>
#include <neb/scene.h>
#include <neb/actor/Rigid_Body.h>
#include <neb/actor/Rigid_Dynamic.h>
#include <neb/view.h>
#include <neb/camera.h>

class box_window;
class box_app
{
public:
		box_app();
		void		step(double);

		std::shared_ptr<box_window>	window_;

		std::shared_ptr<neb::view>	view_;
		std::shared_ptr<neb::scene>	scene_;
};
box_app::box_app()
{
}
void	box_app::step(double time)
{
	printf("%s\n", __PRETTY_FUNCTION__);

	assert(scene_);

	scene_->step(time);
}


class box_window: public glutpp::window
{
public:
		box_window(int w, int h, int x, int y, char const * title);
		void		step(double);
		
		std::weak_ptr<box_app>	app_;
};
box_window::box_window(int w, int h, int x, int y, char const * title):
	glutpp::window(w,h,x,y,title)
{
}
void	box_window::step(double time)
{
	printf("%s\n", __PRETTY_FUNCTION__);

	app_.lock()->step(time);
}





int	main(int argc, char const ** argv)
{
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
	
	std::shared_ptr<box_app> app(new box_app);
	std::shared_ptr<box_window> w(new box_window(600, 600, 200, 100, "First Window"));
	w->app_ = app;
	app->window_ = w;
		
	app->scene_ = neb::__physics.Create_Scene(el_scene);
	app->view_.reset(new neb::view);

	app->view_->scene_ = app->scene_;
	app->scene_->view_ = app->view_;
	

	app->window_->init();

	app->window_->renderable_->scene_->set(glutpp::scene::SHADER);
	app->window_->renderable_->scene_->set(glutpp::scene::LIGHTING);
	
	app->window_->renderable_->scene_->shaders();
	app->window_->renderable_->scene_->uniforms();

	
	//w.set(glutpp::scene::SHADOW);
	
	app->view_->set_window(app->window_);
	
	//camera->SetWindow(window);
	//camera->view_ = view;
	//camera->Connect();

	std::shared_ptr<glutpp::light> l0(new glutpp::light);
	std::shared_ptr<glutpp::light> l1(new glutpp::light);

	l0->camera_.eye_ = math::vec4( 3.0,0.0,0.0,1.0);
	l1->camera_.eye_ = math::vec4(-3.0,0.0,0.0,1.0);

	l0->diffuse_ = math::color(0.6, 0.6, 0.6, 1.0);
	l1->diffuse_ = math::color(0.6, 0.6, 0.6, 1.0);
	
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




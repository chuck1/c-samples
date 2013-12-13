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

#include <neb/physics.h>
#include <neb/scene.h>
#include <neb/actor/Rigid_Body.h>
#include <neb/actor/Rigid_Dynamic.h>
#include <neb/view.h>
#include <neb/camera.h>

class window: public glutpp::window
{
	public:
		window(int,int,int,int,char const *);
		void		step(double);

		neb::view*	view_;
		neb::scene*	scene_;
};
window::window(int w, int h, int x, int y, char const * title):
	glutpp::window(w,h,x,y,title),
	view_(NULL),
	scene_(NULL)

{

}
void	window::step(double time)
{
	printf("%s\n", __PRETTY_FUNCTION__);

	assert(scene_);

	scene_->step(time);
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



	printf("window\n");

	window w(600, 600, 200, 100, "First Window");


	w.scene_ = neb::__physics.Create_Scene(el_scene);
	w.view_ = new neb::view;
	w.view_->scene_ = w.scene_;



	w.set(glutpp::window::SHADER);
	w.set(glutpp::window::LIGHTING);


	w.init();



	w.view_->set_window(&w);

	//camera->SetWindow(window);
	//camera->view_ = view;
	//camera->Connect();

	glutpp::light l;

	l.camera_.eye_ = math::vec4(8.0,8.0,0.0,1.0);

	w.add_light(&l);

	// select our lead actor
	neb::actor::Rigid_Dynamic* actor = (neb::actor::Rigid_Dynamic*)w.scene_->actors_.at(0);
	
	std::shared_ptr<neb::camera_ridealong> ride(new neb::camera_ridealong);
	ride->actor_ = actor;
	
	w.map_sig_key_[GLFW_KEY_SPACE].connect(std::bind(
		&neb::actor::Rigid_Body::key_force,
		actor,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3,
		math::vec3(0.0, 1.0, 0.0)));
	w.map_sig_key_[GLFW_KEY_W].connect(std::bind(
		&neb::actor::Rigid_Body::key_force,
		actor,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3,
		math::vec3(0.0, 0.0,-1.0)));
	w.map_sig_key_[GLFW_KEY_S].connect(std::bind(
		&neb::actor::Rigid_Body::key_force,
		actor,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3,
		math::vec3(0.0, 0.0, 1.0)));
	w.map_sig_key_[GLFW_KEY_D].connect(std::bind(
		&neb::actor::Rigid_Body::key_force,
		actor,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3,
		math::vec3( 1.0, 0.0, 0.0)));
	w.map_sig_key_[GLFW_KEY_A].connect(std::bind(
		&neb::actor::Rigid_Body::key_force,
		actor,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3,
		math::vec3(-1.0, 0.0, 0.0)));



	w.map_sig_key_[GLFW_KEY_I].connect(std::bind(
		&neb::actor::Rigid_Body::key_torque,
		actor,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3,
		math::vec3(-1.0, 0.0, 0.0)));
	w.map_sig_key_[GLFW_KEY_K].connect(std::bind(
		&neb::actor::Rigid_Body::key_torque,
		actor,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3,
		math::vec3( 1.0, 0.0, 0.0)));
	w.map_sig_key_[GLFW_KEY_L].connect(std::bind(
		&neb::actor::Rigid_Body::key_torque,
		actor,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3,
		math::vec3( 0.0,-1.0, 0.0)));
	w.map_sig_key_[GLFW_KEY_J].connect(std::bind(
		&neb::actor::Rigid_Body::key_torque,
		actor,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3,
		math::vec3( 0.0, 1.0, 0.0)));


	printf("control\n");
	w.camera_.control_ = ride;

	printf("loop\n");
	w.loop();


	return 0;
}




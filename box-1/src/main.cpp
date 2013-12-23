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

#include <neb/network/server.h>
#include <neb/network/client.h>

#include <glutpp/master.h>
#include <glutpp/window.h>
#include <glutpp/renderable.h>
#include <glutpp/scene.h>
#include <glutpp/gui/object/object_factory.h>
#include <glutpp/gui/object/textview.h>

#include <neb/app.h>
#include <neb/user.h>
#include <neb/physics.h>
#include <neb/scene.h>
#include <neb/simulation_callback.h>
#include <neb/shape.h>
#include <neb/actor/Rigid_Body.h>
#include <neb/actor/Rigid_Dynamic.h>
#include <neb/view.h>
#include <neb/camera.h>



namespace box
{
	enum box
	{
		SCENE_0,
		LAYOUT_HOME,
		LAYOUT_GAME,
		WINDOW_0,
	};

	namespace layouts
	{
		namespace home
		{
			class local_game: public glutpp::gui::object::textview
			{
				public:
					virtual int	mouse_button_fun(int button, int action, int mods)
					{
						return 1;
					}
			};
			class network_game: public glutpp::gui::object::textview
			{
				public:
					virtual int	mouse_button_fun(int button, int action, int mods)
					{
						return 1;
					}
			};
		}
		namespace game
		{
			class exit: public glutpp::gui::object::textview
			{
				public:
					virtual int	mouse_button_fun(int button, int action, int mods)
					{
						return 1;
					}
			};
		}
	}
	class object_factory: public glutpp::gui::object::object_factory
	{
		public:
			virtual std::shared_ptr<glutpp::gui::object::object>	create(tinyxml2::XMLElement* element) {

				assert(element);

				std::shared_ptr<glutpp::gui::object::object> object;

				char const * buf = element->Attribute("type");
				assert(buf);

				if(strcmp(buf, "home_local_game") == 0)
				{
					object.reset(new layouts::home::local_game);
				}
				else if(strcmp(buf, "home_network_game") == 0)
				{
					object.reset(new layouts::home::network_game);
				}
				else if(strcmp(buf, "game_exit") == 0)
				{
					object.reset(new layouts::game::exit);
				}
				else
				{
					object = glutpp::gui::object::object_factory::create(element);
				}

				object->load_xml(element);

				return object;
			}
	};
}
neb::actor::desc	get_desc() {

	neb::actor::desc desc;
	desc.type = neb::actor::RIGID_DYNAMIC;
	
	desc.pose.p.from_math(math::vec3(0.0, 0.0, 20.0));
	desc.pose.q.from_math(math::quat(0.0, math::vec3(1.0, 0.0, 0.0)));

	neb::shape shape;
	shape.box(math::vec3(0.5, 0.5, 0.5));

	desc.density = 1000.0;
	
	desc.shape = shape;
	
	desc.filter_group = neb::simulation_callback::filter_group::NORMAL;
	desc.filter_mask = neb::simulation_callback::filter_group::PROJECTILE;
	
	return desc;
}


std::shared_ptr<neb::app> app;

int	client_main(char const * addr, short unsigned int port) {
	
	app->client_.reset(new neb::network::client(addr, port));

	return 0;	
}
int	server_main(short unsigned int port) {
	
	app->server_.reset(new neb::network::server(port, 10));
	
	app->load_scene(box::SCENE_0, "scene.xml");
	app->load_layout(box::LAYOUT_HOME, "layout_home.xml");
	app->load_layout(box::LAYOUT_GAME, "layout_game.xml");
	
	
	app->create_window(box::WINDOW_0, 600, 600, 200, 100, "box");
	
	
	app->activate_scene(box::WINDOW_0, box::SCENE_0);
	//app->activate_layout(box::LAYOUT_GAME);
	
	std::shared_ptr<neb::user> user(new neb::user);
	
	auto actor = app->scenes_[box::SCENE_0]->Create_Rigid_Dynamic(get_desc());
	
	user->set_actor(actor, neb::camera_type::e::RIDEALONG);
	user->connect(app->windows_[box::WINDOW_0]);
	
	
	
	printf("loop\n");
	
	app->windows_->prepare();
	app->windows_->loop();
	
	
	return 0;
}
int	main(int argc, char const ** argv)
{
	if(argc < 3)
	{
		printf("usage: %s <mode> <port>\n", argv[0]);
		return 1;
	}

	neb::__physics.Init();

	glutpp::__master.object_factory_.reset(new box::object_factory);
	
	app.reset(new neb::app);
	app->init();


	
	if(strcmp(argv[1], "s") == 0)
	{
		return server_main(atoi(argv[2]));
	}

	return 0;
}




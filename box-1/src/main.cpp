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
#include <glutpp/scene/scene.h>
#include <glutpp/gui/object/object_factory.h>
#include <glutpp/gui/object/textview.h>

#include <neb/app.h>
#include <neb/user.h>
#include <neb/physics.h>
#include <neb/scene/scene.h>
#include <neb/simulation_callback.h>
#include <neb/shape.h>
#include <neb/actor/Rigid_Body.h>
#include <neb/actor/Rigid_Dynamic.h>
#include <neb/camera.h>
#include <neb/packet/packet.h>


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
glutpp::actor::desc* get_desc() {

	glutpp::actor::desc* desc = new glutpp::actor::desc;
	
	desc->raw_.type_ = glutpp::actor::RIGID_DYNAMIC;

	desc->raw_.pose_.p.from_math(math::vec3(0.0, 0.0, 0.0));
	desc->raw_.pose_.q.from_math(math::quat(0.0, math::vec3(1.0, 0.0, 0.0)));

	// shape
	glutpp::shape::desc* sd = new glutpp::shape::desc;
	sd->raw_.box(math::vec3(0.5, 0.5, 0.5));

	sd->raw_.front_.diffuse_.from_math(math::red);
	sd->raw_.front_.emission_.from_math(math::black);

	// light
	glutpp::light::desc* ld = new glutpp::light::desc;
	
	ld->raw_.pos_.from_math(math::vec4(0.0, 0.0, 0.0, 1.0));
	ld->raw_.spot_direction_.from_math(math::vec3(0.0f, 0.0f, 1.0f));
	ld->raw_.spot_cutoff_ = M_PI/10.0;
	ld->raw_.ambient_.from_math(math::black);
	ld->raw_.atten_linear_ = 0.0;
	
	sd->lights_.push_back(ld);

	desc->shapes_.push_back(sd);

	// density
	desc->raw_.density_ = 500.0;

	desc->raw_.filter_data_.simulation_.word0 = neb::filter::type::DYNAMIC;
	desc->raw_.filter_data_.simulation_.word1 = neb::filter::RIGID_AGAINST;

	return desc;
}


std::shared_ptr<neb::app> app;

int	client_main(char const * addr, short unsigned int port) {

	app->client_.reset(new neb::network::client(addr, port));
	app->client_->app_ = app;

	app->loop();


	return 0;	
}
int	server_main(short unsigned int port) {

	app->server_.reset(new neb::network::server(port, 10));
	app->server_->app_ = app;


	neb::scene::desc* sd = new neb::scene::desc;
	sd->load("scene.xml");


	app->load_scene(box::SCENE_0, sd);
	app->load_layout(box::LAYOUT_HOME, "layout_home.xml");
	app->load_layout(box::LAYOUT_GAME, "layout_game.xml");


	app->create_window(box::WINDOW_0, 600, 600, 200, 100, "box");


	app->activate_scene(box::WINDOW_0, box::SCENE_0);
	//app->activate_layout(box::WINDOW_0, box::LAYOUT_GAME);

	std::shared_ptr<neb::user> user(new neb::user);

	glutpp::actor::desc* desc = get_desc();

	auto actor = app->scenes_[box::SCENE_0]->create_actor(desc);

	//std::shared_ptr<neb::actor::Rigid_Dynamic> actor(new neb::actor::Rigid_Dynamic(desc, ));
	//->Create_Rigid_Dynamic(desc);

	user->set_actor(actor, neb::camera_type::e::RIDEALONG);
	user->connect(app->windows_[box::WINDOW_0]);


	// vehicle
	//app->scenes_[box::SCENE_0]->create_vehicle();



	printf("loop\n");

	app->prepare();
	app->loop();


	return 0;
}
int	main(int argc, char const ** argv)
{
	if(argc < 2)
	{
		printf("usage:\n");
		printf("    %s <mode> <port>\n", argv[0]);
		printf("    %s h\n", argv[0]);
		return 1;
	}

	if(strcmp(argv[1], "h") == 0)
	{
		printf("size\n");
		printf("glutpp::material_desc %i\n", (int)sizeof(glutpp::material_desc));
		printf("glutpp::shape_desc    %i\n", (int)sizeof(glutpp::shape::desc));
		printf("neb::actor::desc      %i\n", (int)sizeof(glutpp::actor::desc));
		printf("neb::packet::packet   %i\n", (int)sizeof(neb::packet::packet));
		return 0;
	}

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
	else if(strcmp(argv[1], "c") == 0)
	{
		return client_main("127.0.0.1", atoi(argv[2]));
	}


	return 0;
}




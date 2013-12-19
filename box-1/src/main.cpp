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
#include <glutpp/gui/object/object_factory.h>
#include <glutpp/gui/object/textview.h>

#include <neb/app.h>
#include <neb/user.h>
#include <neb/physics.h>
#include <neb/scene.h>
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
		LAYOUT_GAME
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

int	main(int argc, char const ** argv)
{
	neb::__physics.Init();

	glutpp::__master.object_factory_.reset(new box::object_factory);
	
	std::shared_ptr<neb::app> app(new neb::app);
	app->init();
	
	app->load_scene(box::SCENE_0, "scene.xml");
	app->load_layout(box::LAYOUT_HOME, "layout_home.xml");
	app->load_layout(box::LAYOUT_GAME, "layout_game.xml");
	
	app->activate_scene(box::SCENE_0);
	app->activate_layout(box::LAYOUT_GAME);
	
	
	
	std::shared_ptr<neb::user> user(new neb::user);

	auto actor = std::dynamic_pointer_cast<neb::actor::Base>(app->scenes_[box::SCENE_0]->actors_.at(0));


	user->set_actor(actor, neb::camera_type::e::RIDEALONG);
	user->connect(app->window_);




	printf("loop\n");
	app->window_->prepare();
	app->window_->loop();


	return 0;
}




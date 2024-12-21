/*
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include <string>
#include <iostream>

#include <gtkmm.h>

#include "gui_functions.h"
#include "client_functions.h"

using namespace std;

int main(int argc, char* argv[])
{
	string server, path;
	int pid = getInformation(server, path, argc, argv);

	if (pid == 0)
		pid = setupServer(server, path);

	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("com.github_dchansen06_music_player");
	Gtk::Window* window = new Window(pid);

	app->signal_activate().connect([&](){
		app->add_window(*window);
		window->show();
	});

	int status = app->run();
	delete window;
	return status;
}

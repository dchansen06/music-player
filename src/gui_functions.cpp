/*
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>

#include <gtkmm.h>

#include "gui_functions.h"
#include "client_functions.h"

using namespace std;

Window::Window(int pid) : Gtk::Window(), togglePlaypause(true), serverpid(pid), box(Gtk::Orientation::VERTICAL, 3), playpause_button("⏯︎"), skip_button("⏭︎"), exit_button("🗙")
{
	set_title("Music Player");
	set_child(box);
	box.set_margin(5);
	scrolledWindow.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
	scrolledWindow.set_expand();
	box.append(scrolledWindow);

	grid.set_row_spacing(1);
	grid.set_column_spacing(3);
	scrolledWindow.set_child(grid);

	playpause_button.set_margin(5);
	playpause_button.signal_clicked().connect(sigc::mem_fun(*this, &Window::playpause_button_clicked));
	grid.attach(playpause_button, 0, 0);

	skip_button.set_margin(5);
	skip_button.signal_clicked().connect(sigc::mem_fun(*this, &Window::skip_button_clicked));
	grid.attach(skip_button, 1, 0);

	exit_button.set_margin(5);
	exit_button.signal_clicked().connect(sigc::mem_fun(*this, &Window::exit_button_clicked));
	grid.attach(exit_button, 2, 0);

	playpause_button.grab_focus();
}

int Window::setServerpid(int pid)
{
	int temp = getServerpid();
	serverpid = pid;
	return temp;
}

int Window::getServerpid()
{
	return serverpid;
}

void Window::playpause_button_clicked()
{
	if (togglePlaypause) {
		sendSignal(getServerpid(), PAUSE);
		togglePlaypause = !togglePlaypause;
	} else {
		sendSignal(getServerpid(), RESUME);
		togglePlaypause = !togglePlaypause;
	}
}

void Window::skip_button_clicked()
{
	sendSignal(getServerpid(), SKIP);
}

void Window::exit_button_clicked()
{
	sendSignal(getServerpid(), EXIT);
	this->hide();
}

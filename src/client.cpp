/*
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <cstdlib>
#include <string>

#include "client_functions.h"
#include "signals.h"

using namespace std;

int main(int argc, char* argv[])
{
	string path, directory;

	if (!getInformation(path, directory, argc, argv)) {
		cerr << "Failed to get information\n";
		exit(-1);
	}

	int server = fork();

	if (server == 0)
		execl(path.c_str(), path.c_str(), directory.c_str(), nullptr);

	controlServer(server);

	return 0;
}

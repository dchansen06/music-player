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

// Reads information from the command line and gathers any missing information
// Precondition: A valid path, music directory, and command line arguments
// Postcondition: Returns the PID of the server to use or fills in the path and directory
int getInformation(string& path, string& dir, int argc, char* argv[])
{
	if (argc > 2 && ((string)argv[1]).find("-p") != string::npos) {	// Must be in the form "command -p PID"
			return stoi(argv[2]);
	}

	switch(argc) {	// Gathers path and directory as provided, leaves off tail end
		case 3:
			path = argv[1];
			dir = argv[2];
			break;
		case 2:
			path = argv[1];
			cout << "Enter directory: ";
			cin >> dir;
			break;
		default:
			cout << "Enter path: ";
			cin >> path;
			cout << "Enter directory: ";
			cin >> dir;
	}

	return 0;
}

// Starts a new server as a detatched process and returns its PID to signal it
// Precondition: A valid path and directory for the server to use
// Postcondition: Returns the PID to signal the server on
int setupServer(string path, string directory)
{
	pid_t server = fork();

	if (server == 0) {
		pid_t sid = setsid();

		if (sid < 0) {
			cerr << "Failed to start server! Could not setsid\n";
			exit(-1);
		}

		execl(path.c_str(), path.c_str(), directory.c_str(), nullptr);
	} else if (server < 0) {
		cerr << "Failed to start server! Could not fork\n";
		exit(-1);
	}

	return server;
}

// Loop to control the server with signals and keyboard instructions, can close server or leave it running and stop controlling it
// Precondition: A valid PID of the server
// Postcondition: Kills the server or leaves it running as this exits on user input
void controlServer(int server)
{
	char input;
	cout << "Gained control of server " << server << "\nEnter instructions: [R]esume, [P]ause, re[W]ind, [S]kip, [E]xit (or Ctrl+C to close)\n";
	cin >> input;	// Reads one char at a time

	while (tolower(input) != 'e') {
		switch(tolower(input)) {
			case 'w':
				kill(server, REWIND);
				break;
			case 'p':
				kill(server, PAUSE);
				break;
			case 'r':
				kill(server, RESUME);
				break;
			case 's':
				kill(server, SKIP);
				break;
			default:
				cout << "Misunderstood input, enter R, P, S, or E only:\n";
		}

		usleep(50000);	// Allows inputting multiple instructions on the same line with minimal risk as long as >>fadeMS

		cin >> input;
	}

	kill(server, EXIT);
}

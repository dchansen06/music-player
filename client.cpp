/*
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <csignal>
#include <string>

#include "signals.h"

using namespace std;

int main(int argc, char* argv[])
{
	int server;

	if (argc == 2 && !kill(stoi(argv[1]), RESUME)) {
		server = stoi(argv[1]);
	} else {
		cout << "Enter PID of server: ";
		cin >> server;
	}

	char input;
	cout << "Gained control of server\nEnter instructions: [R]esume, [P]ause, [S]kip, [E]xit (or Ctrl+C to close)\n";
	cin >> input;

	while (tolower(input) != 'e') {
		switch(tolower(input)) {
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
				cout << "Misunderstood input, enter R, P, S, or E only";
		}

		cin >> input;
	}

	kill(server, EXIT);

	return 0;
}

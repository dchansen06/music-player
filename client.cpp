#include <iostream>
#include <csignal>
#include <string>

#include "music_player.h"

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

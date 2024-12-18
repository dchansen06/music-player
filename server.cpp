#include <iostream>
#include <vector>

#include <unistd.h>

#include "signals.h"
#include "server_functions.h"

using namespace std;

int main(int argc, char* argv[])
{
	cout << "PID: " << getpid() << endl;

	fspath path;

	if (argc > 1)
	{
		path = argv[1];
	} else {
		cout << "Enter directory ";
		cin >> path;
	}

	music_list validmusic = findMusicFiles(path);
	shuffleMusic(validmusic);
	playMusic(validmusic, 250);
}

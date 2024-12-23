/*
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>

#include "server_functions.h"

using namespace std;

int main(int argc, char* argv[])
{
	if (argc != 2) {	// Requires directory path
		cerr << "Invalid option\n";
		exit(-1);
	}

	music_list validmusic = findMusicFiles(argv[1]);	// Find music
	shuffleMusic(validmusic);	// Shuffle
	playMusic(validmusic, 250);	// Play music with 250 ms fade in/out between songs
}

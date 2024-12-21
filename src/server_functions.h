/*
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include <filesystem>
#include <string>
#include <vector>
#include <SDL2/SDL_mixer.h>

#pragma once

typedef std::vector<std::filesystem::path> music_list;
typedef std::filesystem::path fspath;

struct MusicInfo {
	std::string title;
	std::string artist;
	std::string album;
	std::string copyright;
	std::string type;
	int numTracks;
};

void shuffleMusic(music_list& files);
MusicInfo getInformation(const Mix_Music* m);
music_list findMusicFiles(fspath path);
void playMusic(music_list path, int fadeMS);

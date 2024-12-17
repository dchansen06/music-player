#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

using namespace std;

#include "functions.h"

vector<filesystem::path> findValidFiles()
{
	filesystem::path path;
	cout << "Enter library path: ";
	cin >> path;

	if (path.empty())
		exit(-1);

	if (path.string().substr(0, 1) == "~")
		path = getenv("HOME") + path.string().substr(1);

	cerr << "Root path " << path.root_path() << endl;
	cerr << "Root name " << path.root_name() << endl;
	cerr << "Root directory " << path.root_directory() << endl;
	cerr << "Relative path " << path.relative_path() << endl;
	cerr << "Parent path " << path.parent_path() << endl;
	cerr << "Filename " << path.filename() << endl;
	cerr << "Stem " << path.stem() << endl;
	cerr << "Extension " << path.extension() << endl;

	vector<filesystem::path> validfiles;

	filesystem::recursive_directory_iterator rdi_itr = filesystem::recursive_directory_iterator(path, filesystem::directory_options::skip_permission_denied);
	filesystem::recursive_directory_iterator it_rdi = begin(rdi_itr);
	while (it_rdi != end(rdi_itr)) {
		if (it_rdi->path().extension() == ".ogg" || it_rdi->path().extension() == ".mp3")
			validfiles.push_back(it_rdi->path());

		it_rdi++;
	}

	return validfiles;
}

string toLower(string s)
{
	string result = "";

	for(char c : s)
		result += tolower(c);

	return result;
}

void playMusic(filesystem::path path)
{
	int flags;
	string ext = toLower(path.extension());

	if (ext ==  ".flac")
		flags = MIX_INIT_FLAC;
	else if (ext == ".mod")
		flags = MIX_INIT_MOD;
	else if (ext == ".mp3")
		flags = MIX_INIT_MP3;
	else if (ext == ".ogg")
		flags = MIX_INIT_OGG;
	else if (ext == ".mid")
		flags = MIX_INIT_MID;
	else if (ext == ".opus")
		flags = MIX_INIT_OPUS;
	else
		flags = MIX_INIT_MP3;

	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		cerr << "Failed to init SDL: " << SDL_GetError() << endl;
		exit(1);
	}

	if (flags != Mix_Init(flags)) {
		cerr << "Could not initialize mixer, Mix_Init: " << Mix_GetError() << endl;
		exit(1);
	}

	if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640) < 0) {
		cerr << "Could not open audio: " << SDL_GetError() << endl;
		exit(1);
	}

	Mix_Music *music = Mix_LoadMUS(path.c_str());
	Mix_PlayMusic(music, 1);

	while (!SDL_QuitRequested())
		SDL_Delay(250);

	Mix_FreeMusic(music);
	SDL_Quit();
}

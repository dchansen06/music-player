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

	vector<filesystem::path> validfiles, nomediadir;

	filesystem::recursive_directory_iterator rdi_itr = filesystem::recursive_directory_iterator(path, filesystem::directory_options::skip_permission_denied);
	filesystem::recursive_directory_iterator it_rdi = begin(rdi_itr);
	while (it_rdi != end(rdi_itr)) {
		if (it_rdi->path().filename() == ".nomedia")
			nomediadir.push_back(it_rdi->path().parent_path());

		it_rdi++;
	}

	rdi_itr = filesystem::recursive_directory_iterator(path, filesystem::directory_options::skip_permission_denied);
	it_rdi = begin(rdi_itr);
	while (it_rdi != end(rdi_itr)) {
		bool valid = true;
		for (filesystem::path badpath : nomediadir) {
			if (it_rdi->path().string().find(badpath) == string::npos) {
				valid = false;
				break;
			}
		}

		if (valid) {
			cout << "Looking at " << it_rdi->path() << endl;

			if (it_rdi->path().extension() == ".ogg" || it_rdi->path().extension() == ".mp3" || it_rdi->path().extension() == ".wav") {
				validfiles.push_back(it_rdi->path());
				cout << "Added " << validfiles[validfiles.size() - 1] << endl;
			}
		}

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

void playMusic(vector<filesystem::path> paths)
{
	int flags;
	Mix_Music *music;

	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		cerr << "Failed to init SDL: " << SDL_GetError() << endl;
		exit(1);
	}

	if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640) < 0) {
		cerr << "Could not open audio: " << SDL_GetError() << endl;
		exit(1);
	}

	for (filesystem::path path : paths) {
		cout << "Starting ... " << path;

		music = Mix_LoadMUS(path.c_str());

		if (!Mix_FadeInMusic(music, 1, 250)) {
			cerr << "Could not start music: " << SDL_GetError() << endl;
			exit(1);
		}

		cout << " ... Started" << endl;

		while (!SDL_QuitRequested() && (Mix_PlayingMusic() || Mix_PausedMusic()))
			SDL_Delay(250);

		Mix_FreeMusic(music);
	}

	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
}

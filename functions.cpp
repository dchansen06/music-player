#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <random>
#include <algorithm>

#include <cstdlib>
#include <csignal>

#include "functions.h"

using namespace std;

sig_atomic_t signalStatus;
void signal_handler(int signal)
{
	signalStatus = signal;
}

void shuffleMusic(vector<filesystem::path>& files)
{
	random_device rand_dev;
	default_random_engine rand_eng(rand_dev());
	shuffle(files.begin(), files.end(), rand_eng);
}

vector<filesystem::path> findMusicFiles()
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

void playMusic(vector<filesystem::path> paths, int fadeMS)
{
	Mix_Music *music;

	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		cerr << "Failed to init SDL: " << SDL_GetError() << endl;
		exit(1);
	}

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) < 0) {
		cerr << "Could not open audio: " << SDL_GetError() << endl;
		exit(1);
	}

	int audio_rate, audio_channels;
	SDL_AudioFormat audio_format;
	Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
	cout << "Opened audio at " << audio_rate << " Hz, "
		<< (audio_format&0xFF) << " bit" << (SDL_AUDIO_ISFLOAT(audio_format) ? " (float), " : ", ")
		<< ((audio_channels > 2) ? "surround" : (audio_channels > 1) ? "stereo" : "mono") << endl;

	for (filesystem::path path : paths) {
		cout << "Starting ... " << path << " ";

		music = Mix_LoadMUS(path.c_str());

		if (Mix_FadeInMusic(music, 1, fadeMS) < 0) {
			cerr << "Could not play music: " << SDL_GetError() << endl;
			exit(1);
		}

		cout << "... Started" << endl;

		signal(SIGINT, signal_handler);

		while (signalStatus != SIGINT && (Mix_MusicDuration(music) - Mix_GetMusicPosition(music)) * 1000 > fadeMS)
			SDL_Delay(fadeMS);

		if (signalStatus == SIGINT)
			signalStatus = SIGUSR1;

		Mix_FadeOutMusic(fadeMS);
		Mix_FreeMusic(music);
	}

	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
}

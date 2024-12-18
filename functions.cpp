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

void shuffleMusic(music_list& songs)
{
	random_device rand_dev;
	default_random_engine rand_eng(rand_dev());
	shuffle(songs.begin(), songs.end(), rand_eng);
}

music_list findMusicFiles()
{
	filesystem::path path;
	cout << "Enter library path: ";
	cin >> path;

	if (path.empty())
		exit(-1);

	if (path.string().substr(0, 1) == "~")
		path = getenv("HOME") + path.string().substr(1);

	vector<filesystem::path> validfiles, nomediadir;

	filesystem::recursive_directory_iterator rdi_itr = filesystem::recursive_directory_iterator(path, filesystem::directory_options::skip_permission_denied);
	filesystem::recursive_directory_iterator it_rdi = begin(rdi_itr);
	while (it_rdi != end(rdi_itr)) {
		if (it_rdi->path().filename() == ".nomedia")
			nomediadir.push_back(it_rdi->path().parent_path());

		it_rdi++;
	}

	vector<string> validExtensionTypes = {".ogg", ".mp3", ".wav", ".m4a", ".flac"};
	for (size_t i = 0; i < (validExtensionTypes.size() + 1) / 2; i++) {
		string temp = "";
		for (char c : validExtensionTypes[i])
			temp += toupper(c);
		validExtensionTypes.push_back(temp);
	}

	rdi_itr = filesystem::recursive_directory_iterator(path, filesystem::directory_options::skip_permission_denied);
	it_rdi = begin(rdi_itr);
	while (it_rdi != end(rdi_itr)) {
		bool valid = true;
		for (filesystem::path badpath : nomediadir) {
			if (it_rdi->path().string().find(badpath.string()) != string::npos) {
				valid = false;
				break;
			}
		}

		if (valid && find(begin(validExtensionTypes), end(validExtensionTypes), it_rdi->path().extension()) != end(validExtensionTypes))
			validfiles.push_back(it_rdi->path());

		it_rdi++;
	}

	return validfiles;
}

void playMusic(music_list songs, int fadeMS)
{
	if (songs.size() == 0) {
		cerr << "Nothing to play\n";
		exit(1);
	}

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

	for (filesystem::path song : songs) {
		cout << "Starting ... " << song << " ";

		music = Mix_LoadMUS(song.c_str());

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

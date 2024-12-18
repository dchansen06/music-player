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

#include "music_player.h"
#include "server_functions.h"

using namespace std;

sig_atomic_t signalStatus = RESUME;
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

vector<fspath> findNoMedia(fspath path, string pattern)
{
	vector<fspath> nomediadir;

	filesystem::recursive_directory_iterator rdi_itr = filesystem::recursive_directory_iterator(path, filesystem::directory_options::skip_permission_denied);
	filesystem::recursive_directory_iterator it_rdi = begin(rdi_itr);

	while (it_rdi != end(rdi_itr)) {
		if (it_rdi->path().filename() == pattern)
			nomediadir.push_back(it_rdi->path().parent_path());

		it_rdi++;
	}

	return nomediadir;
}

vector<string> validExtensionTypes()
{
	vector<string> validExtensionTypes = {".ogg", ".mp3", ".wav", ".flac"};

	for (size_t i = 0; i < (validExtensionTypes.size() + 1) / 2; i++) {
		string temp = "";
		for (char c : validExtensionTypes[i])
			temp += toupper(c);
		validExtensionTypes.push_back(temp);
	}

	return validExtensionTypes;
}

void checkAndCorrectPath(fspath path)
{
	if (path.empty())
		exit(-1);

	if (path.string()[0] == '~')
		path = getenv("HOME") + path.string().substr(1);
}

music_list findMusicFiles(fspath path)
{
	checkAndCorrectPath(path);

	music_list validfiles;
	vector<fspath> nomediadirs = findNoMedia(path, ".nomedia");
	vector<string> validExtensions = validExtensionTypes();

	filesystem::recursive_directory_iterator rdi_itr = filesystem::recursive_directory_iterator(path, filesystem::directory_options::skip_permission_denied);
	filesystem::recursive_directory_iterator it_rdi = begin(rdi_itr);
	while (it_rdi != end(rdi_itr)) {
		bool valid = true;
		for (fspath nomediadir : nomediadirs) {
			if (it_rdi->path().string().find(nomediadir.string()) != string::npos) {
				valid = false;
				break;
			}
		}

		if (valid && find(begin(validExtensions), end(validExtensions), it_rdi->path().extension()) != end(validExtensions))
			validfiles.push_back(it_rdi->path());

		it_rdi++;
	}

	return validfiles;
}

void playSong(fspath song, int fadeMS)
{
	Mix_Music *music = Mix_LoadMUS(song.c_str());

	if (Mix_FadeInMusic(music, 1, fadeMS) < 0) {
		cerr << "Could not play " << song << ": " << SDL_GetError() << endl;
		exit(1);
	}

	signal(SKIP, signal_handler);
	signal(PAUSE, signal_handler);
	signal(RESUME, signal_handler);
	signal(EXIT, signal_handler);

	while (signalStatus != SKIP && signalStatus != EXIT && (Mix_MusicDuration(music) - Mix_GetMusicPosition(music)) * 1000 > fadeMS) {
		if (signalStatus == PAUSE) {
			Mix_PauseMusic();
		} else if (signalStatus == RESUME) {
			Mix_ResumeMusic();
		}

		SDL_Delay(fadeMS);
	}

	if (signalStatus == SKIP) {
		signalStatus = RESUME;
	}

	Mix_FadeOutMusic(fadeMS);
	Mix_FreeMusic(music);
}

void playMusic(music_list songs, int fadeMS)
{
	if (songs.size() == 0) {
		return;
	}

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

	for (fspath song : songs) {
		if (signalStatus == EXIT)
			break;

		playSong(song, fadeMS);
	}

	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
}

/*
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

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

#include "signals.h"
#include "server_functions.h"

using namespace std;

sig_atomic_t signalStatus = RESUME;	// Holds the signal that was most recently sent

// Catches signals it is set up to catch and alters signalStatus accordingly
// Precondition: A valid signal
// Postcondition: Changes the signalStatus to match the signal
void signal_handler(int signal)
{
	signalStatus = signal;
}

// Shuffles the music_list provided randomly
// Precondition: A valid music_list passed by reference
// Postcondition: The music_list is shuffled
void shuffleMusic(music_list& songs)
{
	random_device rand_dev;
	default_random_engine rand_eng(rand_dev());
	shuffle(songs.begin(), songs.end(), rand_eng);
}

// Provides useful information about the currently playing song
// Precondition: A valid Mix_Music pointer
// Postcondition: Returns a filled out MusicInfo struct
MusicInfo getInformation(const Mix_Music* m)
{
	MusicInfo info;

	info.title = Mix_GetMusicTitle(m);
	info.artist = Mix_GetMusicArtistTag(m);
	info.album = Mix_GetMusicAlbumTag(m);
	info.copyright = Mix_GetMusicCopyrightTag(m);
	info.type = Mix_GetMusicType(m);

	return info;
}

// Looks through the given path recursively for any directory containing a given file pattern and adds the directory containing it to a list
// Precondition: A valid path and pattern
// Postcondition: Returns a vector<fspath> of locations to not look through
vector<fspath> findNoMedia(fspath path, string pattern)
{
	vector<fspath> nomediadir;

	filesystem::recursive_directory_iterator rdi_itr = filesystem::recursive_directory_iterator(path, filesystem::directory_options::skip_permission_denied);
	filesystem::recursive_directory_iterator it_rdi = begin(rdi_itr);

	while (it_rdi != end(rdi_itr)) {	// Loops through until a given file is found
		if (it_rdi->path().filename() == pattern)
			nomediadir.push_back(it_rdi->path().parent_path());	// Adds parent to list

		it_rdi++;
	}

	return nomediadir;
}

// Configures the valid extensions of music to include in a music_list
// Precondition: None
// Postcondition: Returns a vector<string> containing the given valid extensions
vector<string> validExtensionTypes()
{
	vector<string> validExtensionTypes = {".ogg", ".mp3", ".wav", ".flac"};

	/* Adds the capitalized version of any above extension */
	for (size_t i = 0; i < (validExtensionTypes.size() + 1) / 2; i++) {
		string temp = "";
		for (char c : validExtensionTypes[i])
			temp += toupper(c);
		validExtensionTypes.push_back(temp);
	}

	return validExtensionTypes;
}

// Notes an empty path and configures $HOME correctly
// Precondition: A given fspath that can be changed
// Postcondition: A path with $HOME resolved or an error
void checkAndCorrectPath(fspath path)
{
	if (path.empty()) {
		cerr << "Bad path!\n";
		exit(-1);
	}

	if (path.string()[0] == '~')
		path = getenv("HOME") + path.string().substr(1);
}

// Recursively looks through a given path, adds all valid music files that do not mix with the findNoMedia pattern
// Precondition: A valid path
// Postcondition: A music_list containing all found songs
music_list findMusicFiles(fspath path)
{
	checkAndCorrectPath(path);	// Configures $HOME

	music_list validfiles;
	vector<fspath> nomediadirs = findNoMedia(path, ".nomedia");	// Any directory containing a file named ".nomedia" will not have files inside of it added
	vector<string> validExtensions = validExtensionTypes();

	filesystem::recursive_directory_iterator rdi_itr = filesystem::recursive_directory_iterator(path, filesystem::directory_options::skip_permission_denied);
	filesystem::recursive_directory_iterator it_rdi = begin(rdi_itr);
	while (it_rdi != end(rdi_itr)) {	// Loops through recursively
		bool valid = true;
		for (fspath nomediadir : nomediadirs) {	// Confirms path does not violate nomediadir
			if (it_rdi->path().string().find(nomediadir.string()) != string::npos) {
				valid = false;
				break;
			}
		}

		if (valid && find(begin(validExtensions), end(validExtensions), it_rdi->path().extension()) != end(validExtensions))
			validfiles.push_back(it_rdi->path());	// Has valid extension

		it_rdi++;
	}

	return validfiles;
}

// Plays a given song from a path and fades it in/out for given a time
// Precondition: A valid song path and positive fadeMS
// Postcondition: Plays the song if the SDL2 audio setup works, handles appropriate signals
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
	signal(REWIND, signal_handler);
	signal(EXIT, signal_handler);

	while (signalStatus != SKIP && signalStatus != EXIT && (Mix_MusicDuration(music) - Mix_GetMusicPosition(music)) * 1000 > fadeMS) {
		if (signalStatus == PAUSE) {
			Mix_PauseMusic();
		} else if (signalStatus == RESUME) {
			Mix_ResumeMusic();
		} else if (signalStatus == REWIND) {
			Mix_PauseMusic();
			Mix_RewindMusic();
			Mix_ResumeMusic();
		}

		SDL_Delay(fadeMS);
	}

	if (signalStatus == SKIP)
		signalStatus = RESUME;

	Mix_FadeOutMusic(fadeMS);
	Mix_FreeMusic(music);
}

// Plays a given music_list of songs, fades each in/out for fadeMS
// Precondition: A valid music_list and positive fadeMS
// Postcondition: Plays each song, handling provided signals correctly
void playMusic(music_list songs, int fadeMS)
{
	if (songs.size() == 0) {	// Nothing to do
		return;
	}

	if (SDL_Init(SDL_INIT_AUDIO) < 0) {	// Could not get audio control
		cerr << "Failed to init SDL: " << SDL_GetError() << endl;
		exit(1);
	}

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) < 0) {	// Tries defaults
		cerr << "Could not open audio: " << SDL_GetError() << endl;
		exit(1);
	}

	/* Collects information to present */
	int audio_rate, audio_channels;
	SDL_AudioFormat audio_format;
	Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
	cout << "Opened audio at " << audio_rate << " Hz, "
		<< (audio_format&0xFF) << " bit" << (SDL_AUDIO_ISFLOAT(audio_format) ? " (float), " : ", ")
		<< ((audio_channels > 2) ? "surround" : (audio_channels > 1) ? "stereo" : "mono") << endl;

	/* Plays each song until told to exit or runs out of songs */
	for (fspath song : songs) {
		if (signalStatus == EXIT)
			break;

		playSong(song, fadeMS);
	}

	/* Clean up */
	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
}

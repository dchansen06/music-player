#include <filesystem>
#include <vector>

#ifndef FUNCTIONS_H
	#define FUNCTIONS_H
	typedef std::vector<std::filesystem::path> music_list;
	void shuffleMusic(music_list& files);
	music_list findMusicFiles();
	void playMusic(music_list path, int fadeMS);
#endif

#include <filesystem>
#include <vector>

#ifndef SERVER_FUNCTIONS_H
	#define SERVER_FUNCTIONS_H
	typedef std::vector<std::filesystem::path> music_list;
	typedef std::filesystem::path fspath;
	void shuffleMusic(music_list& files);
	music_list findMusicFiles(fspath path);
	void playMusic(music_list path, int fadeMS);
#endif

#include <filesystem>
#include <vector>

#ifndef FUNCTIONS_H
	#define FUNCTIONS_H
	typedef std::filesystem::path musicfile;
	void shuffleMusic(std::vector<musicfile>& files);
	std::vector<musicfile> findMusicFiles();
	void playMusic(std::vector<musicfile> path, int fadeMS);
#endif

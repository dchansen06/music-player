#include <filesystem>
#include <vector>

#ifndef FUNCTIONS_H
	#define FUNCTIONS_H
	void shuffleMusic(std::vector<std::filesystem::path>& files);
	std::vector<std::filesystem::path> findValidFiles();
	void playMusic(std::vector<std::filesystem::path> path, int fadeMS);
#endif

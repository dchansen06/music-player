#include <iostream>
#include <filesystem>
#include <vector>

#include "functions.h"

using namespace std;

int main()
{
	music_list validmusic = findMusicFiles();
	shuffleMusic(validmusic);
	playMusic(validmusic, 250);
}

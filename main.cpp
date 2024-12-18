#include <iostream>
#include <filesystem>
#include <vector>

#include "functions.h"

using namespace std;

int main()
{
	vector<filesystem::path> validfiles = findValidFiles();
	shuffleMusic(validfiles);
	playMusic(validfiles, 250);
}

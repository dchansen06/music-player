#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "functions.h"

int main()
{
	vector<filesystem::path> validfiles = findValidFiles();

	for (filesystem::path p : validfiles) {
		cout << p << endl;
		playMusic(p);
	}
}

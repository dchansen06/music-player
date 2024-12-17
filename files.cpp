#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

using namespace std;

namespace fs = std::filesystem;

int main()
{
	filesystem::path path;
	cout << "Enter library name: ";
	cin >> path;

	if (path.empty())
		return -1;

	if (path.string().substr(0, 1) == "~")
		path = getenv("HOME") + path.string().substr(1);

	cout << "Root path " << path.root_path() << endl;
	cout << "Root name " << path.root_name() << endl;
	cout << "Root directory " << path.root_directory() << endl;
	cout << "Relative path " << path.relative_path() << endl;
	cout << "Parent path " << path.parent_path() << endl;
	cout << "Filename " << path.filename() << endl;
	cout << "Stem " << path.stem() << endl;
	cout << "Extension " << path.extension() << endl;

	vector<filesystem:directory_entry> validfiles;

	for (const filesystem::directory_entry& dir_entry : filesystem::recursive_directory_iterator(path))
		if (filesystem::is_regular_file(dir_entry) && dir_entry().extension() == "mp3")
			std::cout << dir_entry << '\n';

	std::filesystem::remove_all("sandbox");
}

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<filesystem::path> findValidFiles()
{
	filesystem::path path;
	cout << "Enter library path: ";
	cin >> path;

	if (path.empty())
		exit(-1);

	if (path.string().substr(0, 1) == "~")
		path = getenv("HOME") + path.string().substr(1);

	cerr << "Root path " << path.root_path() << endl;
	cerr << "Root name " << path.root_name() << endl;
	cerr << "Root directory " << path.root_directory() << endl;
	cerr << "Relative path " << path.relative_path() << endl;
	cerr << "Parent path " << path.parent_path() << endl;
	cerr << "Filename " << path.filename() << endl;
	cerr << "Stem " << path.stem() << endl;
	cerr << "Extension " << path.extension() << endl;

	vector<filesystem::path> validfiles;

	filesystem::recursive_directory_iterator rdi_itr = filesystem::recursive_directory_iterator(path, filesystem::directory_options::skip_permission_denied);
	filesystem::recursive_directory_iterator it_rdi = begin(rdi_itr);
	while (it_rdi != end(rdi_itr)) {
		if (it_rdi->path().extension() == ".ogg" || it_rdi->path().extension() == ".mp3")
			validfiles.push_back(it_rdi->path());

		it_rdi++;
	}

	return validfiles;
}

int main()
{
	vector<filesystem::path> validfiles = findValidFiles();

	for (filesystem::path p : validfiles)
		cout << p << endl;
}

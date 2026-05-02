#include <filesystem>
#include <iomanip>
#include <iostream>
#include <string>

#define BLUE "\033[34m"
#define WHITE "\033[37m"
#define CYAN "\033[36m"

std::string_view rm_path_file_name(std::string_view file_name)
{
	unsigned long i = file_name.rfind('/');

	if (i == std::string::npos)
		return {};

	return file_name.substr(i + 1, file_name.size());
}

void tree_write(const std::filesystem::path &root_path)
{
	int fs_level = 0;

	std::string_view prev_dir;

	for (const auto &entry : std::filesystem::directory_iterator(root_path)) {

		const std::string file_name = entry.path().string();
		std::string_view curr_dir{rm_path_file_name(file_name)};

		if (entry.is_directory()) {
			if (curr_dir != prev_dir)
				--fs_level;

			prev_dir = curr_dir;
			++fs_level;

			std::cout << BLUE << std::setw(fs_level * 3) << curr_dir << '/' << '\n';

		} else if (entry.is_regular_file()) {

			std::cout << WHITE << std::setw(fs_level * 3) << "" << rm_path_file_name(file_name) << '\n';

		} else if (entry.is_symlink()) {

			std::cout << CYAN << std::setw(fs_level * 3) << "" << file_name << '\n';
		}
	}
}

int main(int argc, char *argv[])
{
	try {
		const auto root_path{argc >= 2 ? argv[1] : std::filesystem::current_path()};
		tree_write(root_path);
	} catch (std::filesystem::filesystem_error &fs_error) {
		std::cerr << "filesystem error: " << fs_error.what() << std::endl;
	}

	return 0;
}

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <vector>

#define BLUE "\033[34m"
#define WHITE "\033[37m"
#define CYAN "\033[36m"

namespace fs = std::filesystem;

const std::vector<std::string> end_prefixes = {"└── ", "    "};
const std::vector<std::string> inner_prefixes = {"├── ", "│   "};

bool is_hidden(const fs::directory_entry &entry) { return entry.path().filename().string()[0] == '.'; }

void write_entry(const fs::directory_entry &entry, std::string_view prefix, unsigned int prefix_offset)
{
	if (entry.is_directory()) {
		std::cout << std::setw(prefix_offset * 3) << WHITE << prefix << BLUE << entry.path().filename().string() << std::endl;
	} else if (entry.is_regular_file()) {
		std::cout << std::setw(prefix_offset * 3) << WHITE << prefix << entry.path().filename().string() << std::endl;
	} else if (entry.is_symlink()) {
		std::cout << std::setw(prefix_offset * 3) << WHITE << prefix << CYAN << entry.path().filename().string() << std::endl;
	}
}

void walk(const fs::path &rooth_path, std::string prefix, unsigned int prefix_offset = 1)
{
	std::vector<fs::directory_entry> entries;

	for (const auto &entry : fs::directory_iterator(rooth_path)) {
		if (!is_hidden(entry)) {
			entries.emplace_back(entry);
		}
	}

	std::sort(std::begin(entries), std::end(entries), [](const fs::directory_entry &a, const fs::directory_entry &b) { return a.path().filename() < b.path().filename(); });

	for (size_t i{0}; i < entries.size(); ++i) {
		const fs::directory_entry entry{entries[i]};

		std::vector<std::string> prefixes{(i == entries.size() - 1) ? end_prefixes : inner_prefixes};

		write_entry(entry, prefixes[0], prefix_offset);

		if (entry.is_directory()) {
			walk(entry.path(), prefix + prefixes[1], ++prefix_offset);
		}
	}
}

int main(int argc, char *argv[])
{
	try {
		const auto root_path{argc >= 2 ? argv[1] : fs::current_path()};
		std::cout << BLUE << root_path.string() << std::endl;
		walk(root_path, "");

	} catch (fs::filesystem_error &fs_error) {
		std::cerr << "filesystem error: " << fs_error.what() << std::endl;
	}

	return 0;
}

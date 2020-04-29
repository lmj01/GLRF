#include <GLRF/TextureManager.hpp>

void TextureManager::registerSource(fs::path path) {
    if (fs::exists(path)) {
        this->registered_paths.insert(path);
    }
    else {
        std::cout << "The specified path '" << path.generic_string() << "' does not exist." << std::endl;
    }
}

fs::path TextureManager::findTexturePath(std::string filename) {
    fs::path file = fs::path(filename);
    // use cached path if possible to reduce search time
    auto it = this->cached_paths.find(file);
    if (it != this->cached_paths.end()) {
        return it->second;
    }
    else {
        // path was not cached
        // search registered paths
        fs::path target;
        for (fs::path path : this->registered_paths)
        {
            fs::directory_entry dir(path);
            if (!dir.exists()) {
                // remove directories that no longer exist
                this->registered_paths.erase(path);
                continue;
            }
            // registered path may be target
            if (findPathLocally(dir, filename, &target)) {
                break;
            }
            bool is_found = false;
            for (auto& subdir : fs::directory_iterator(path))
            {
                // registered path may contain target
                if (findPathLocally(subdir, filename, &target)) {
                    is_found = true;
                    break;
                }
            }
            if (is_found) {
                break;
            }
        }
        return target;
    }
}

bool TextureManager::findPathLocally(fs::directory_entry dir, std::string filename, fs::path * target) {
    fs::path path = dir.path();
    if (dir.is_directory() && path.filename().generic_string() == filename) {
        // registered path is target
        this->cached_paths.insert(std::pair<std::string, fs::path>(filename, path));
        *target = path;
        return true;
    }
    return false;
}
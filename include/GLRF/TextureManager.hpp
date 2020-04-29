#pragma once
#include <memory>
#include <string>
#include <set>
#include <map>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

typedef unsigned long long TextureSpaceSize;

class TextureManager {
private:
    std::set<fs::path> registered_paths;
    std::map<std::string, fs::path> cached_paths;
    TextureSpaceSize next_texture_id;
    TextureManager();
    TextureManager(const TextureManager&);
    TextureManager & operator = (const TextureManager &);
    bool findPathLocally(fs::directory_entry dir, std::string filename, fs::path * target);
public:
    static TextureManager& getInstance() {
        static TextureManager instance;
        return instance;
    }

    ~TextureManager();

    void registerSource(fs::path path);
    fs::path findTexturePath(std::string name);
};
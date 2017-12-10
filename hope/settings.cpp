#include "settings.hpp"

#include <fstream>
#include <streambuf>
#include <string>

std::string readFile(const std::string &file_name) {
    std::ifstream t(file_name);
    return std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
}

Settings loadSettings(std::string path_to_dump) {
    Settings settings;

    if (path_to_dump.back() != '/')
        path_to_dump.push_back('/');

    settings.path_to_dump = path_to_dump;

    std::string str;
    std::ifstream stream(path_to_dump + "settings.txt");
    while (stream >> str) {
        if (str == "mode") {
            stream >> str;

            if (str == "default")
                settings.mode = Mode::Default;
            else if (str == "mark")
                settings.mode = Mode::Mark;
            else
                throw std::runtime_error("invalid settings");
        } else if (str == "crop_rect") {
            stream >> settings.crop_rect.x >> settings.crop_rect.y >> settings.crop_rect.width >>
                settings.crop_rect.height;
        } else
            throw std::runtime_error("invalid settings");
    }

    std::cout << "Succesfully loaded settings\n";

    return settings;
}

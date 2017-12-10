#pragma once

#include <string>
#include <opencv2/opencv.hpp>

enum class Mode {
    Default,
    Mark,
};

struct Settings {
    std::string path_to_dump;
    Mode mode;
    cv::Rect crop_rect;
};

Settings loadSettings(std::string path_to_dump);

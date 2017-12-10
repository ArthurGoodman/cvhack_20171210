#pragma once

#include <opencv2/opencv.hpp>

struct Frame {
    cv::Mat image;
    uint32_t time;
};

class VideoPlayer {
public:
    VideoPlayer(const std::string &file_name, cv::Rect crop_rect);
};

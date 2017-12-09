#include <opencv2/opencv.hpp>

int main(int, char **) {
    // cv::VideoCapture capture(0);
    cv::VideoCapture capture("data1/2/camera1.avi");

    if (!capture.isOpened())
        return -1;

    cv::Mat edges;
    cv::namedWindow("video", 1);

    while (true) {
        cv::Mat frame;
        capture >> frame;

        cv::cvtColor(frame, edges, cv::COLOR_BGR2GRAY);

        cv::GaussianBlur(edges, edges, cv::Size(7, 7), 1.5, 1.5);
        cv::Canny(edges, edges, 0, 30, 3);

        cv::imshow("video", edges);

        if (cv::waitKey(30) >= 0)
            break;
    }

    return 0;
}

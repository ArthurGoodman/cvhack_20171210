#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat frame;
bool first_point_set = false;
cv::Point first_point;

void mouseCallback(int event, int x, int y, int flags, void *param) {
    const int size = 5;

    if (event == cv::EVENT_LBUTTONDOWN) {
        if (!first_point_set) {
            std::cout << x << " " << y << "\n";

            first_point = cv::Point(x, y);
            first_point_set = true;

            cv::circle(frame, cv::Point(x, y), size, cv::Scalar(0, 0, 255), size);

            cv::imshow("video", frame);
        } else {
            cv::circle(frame, cv::Point(x, y), size, cv::Scalar(0, 0, 255), size);
            cv::line(frame, cv::Point(x, y), first_point, cv::Scalar(0, 255, 0), 5, CV_AA);

            cv::imshow("video", frame);
        }
    }
}

int main(int argc, char **argv) {
    cv::Point2f principal_point(5.8479e+2, 3.8181e+2);
    cv::Point2f focal_length(9.9856e+2, 9.9856e+2);
    std::vector<double> distortion_coeffs = {
        -4.5984e-01, 3.4864e-01, -2.4388e-01, -7.6548e-03, 1.5670e-02
    }; // P1, P2, P3, K1, K2

    cv::VideoCapture capture("data1/2/camera1.avi");

    if (!capture.isOpened())
        return -1;

    cv::namedWindow("video", 1);

    while (true) {
        capture >> frame;

        cv::imshow("video", frame);

        int key = cv::waitKey(30);
        if (key >= 0) {
            if (key == 27)
                return 0;

            break;
        }
    }

    cv::setMouseCallback("video", mouseCallback, 0);

    cv::imshow("video", frame);
    cv::waitKey(0);

    return 0;
}

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

cv::Point2f principal_point(5.8479e+2, 3.8181e+2);
double focal_length = 9.9856e+2;
std::vector<double> distortion_coeffs = { -4.5984e-01,
                                          3.4864e-01,
                                          -2.4388e-01,
                                          -7.6548e-03,
                                          1.5670e-02 }; // P1, P2, P3, K1, K2

cv::Mat original_frame, frame;
bool in_the_middle_of_setting_line = false;

struct Line {
    cv::Point2f p1, p2;
};

std::vector<Line> lines;

int frameWidth = 1280;
int frameHeight = 720;

#define PI 3.1415926535897932

cv::Point2f intersection;

double camera_height = 0;
double FOV_x = 0;
double FOV_y = 0;

cv::Point2f window_to_world(cv::Point2f p) {
    double tx = camera_height * tan(FOV_x / 2);
    double ty = camera_height * tan(FOV_y / 2);
    return cv::Point2f(2 * p.x * (tx / frameWidth) - tx, 2 * p.y * (ty / frameHeight) - ty);
}

cv::Mat birdView(cv::Mat source) {
    cv::Mat destination;

    int alpha_ = 7, beta_ = 90, gamma_ = 90;
    int f_ = 500, dist_ = 545;

    // resize(source, source, cv::Size(frameWidth, frameHeight));

    Mat t_original3 = (Mat_<float>(3, 3) << 1, 0, -intersection.x + frameWidth/2,
                                  0, 1, -intersection.y + frameHeight/2,
                                  0, 0, 1);

    Mat t_original4 = (Mat_<float>(4, 4) << 1, 0, 0, -intersection.x + frameWidth/2,
                                  0, 1, 0, -intersection.y+frameHeight/2,
                                  0, 0, 1, 0,
                                  0, 0, 0, 1);

    double focalLength, dist, alpha, beta, gamma;

    alpha = ((double)alpha_ - 90) * PI / 180;
    beta = ((double)beta_ - 90) * PI / 180;
    gamma = ((double)gamma_ - 90) * PI / 180;
    focalLength = (double)f_;
    dist = (double)dist_;

    Size image_size = source.size();
    double w = (double)image_size.width, h = (double)image_size.height;

    // Projecion matrix 2D -> 3D
    Mat A1 =
        (Mat_<float>(4, 3) << 1,
         0,
         -w / 2, //-w/2
         0,
         1,
         -h / 2, //-h/2
         0,
         0,
         0,
         0,
         0,
         1);

    // Rotation matrices Rx, Ry, Rz

    Mat RX =
        (Mat_<float>(4, 4) << 1,
         0,
         0,
         0,
         0,
         cos(alpha),
         -sin(alpha),
         0,
         0,
         sin(alpha),
         cos(alpha),
         0,
         0,
         0,
         0,
         1);

    Mat RY =
        (Mat_<float>(4, 4) << cos(beta),
         0,
         -sin(beta),
         0,
         0,
         1,
         0,
         0,
         sin(beta),
         0,
         cos(beta),
         0,
         0,
         0,
         0,
         1);

    Mat RZ =
        (Mat_<float>(4, 4) << cos(gamma),
         -sin(gamma),
         0,
         0,
         sin(gamma),
         cos(gamma),
         0,
         0,
         0,
         0,
         1,
         0,
         0,
         0,
         0,
         1);

    // R - rotation matrix
    Mat R = RX * RY * RZ;

    // T - translation matrix
    Mat T = (Mat_<float>(4, 4) << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, dist, 0, 0, 0, 1);

    // K - intrinsic matrix
    Mat K =
        (Mat_<float>(3, 4) << 9.9856e+02,
         0,
         5.8479e+02,
         0,
         0,
         9.9856e+02,
         3.8181e+02,
         0,
         0,
         0,
         1,
         0);

    Mat H = t_original3 * (K * (T * (R * A1)));
    std::cout << H << std::endl;

    warpPerspective(original_frame, destination, H, image_size, INTER_CUBIC | WARP_INVERSE_MAP);

    cv::Mat_<float> p1(3, 1);
    cv::Mat_<float> p2(3, 1);

    p1(0, 0) = lines[1].p1.x;
    p1(1, 0) = lines[1].p1.y;
    p1(2, 0) = 1;

    p2(0, 0) = lines[1].p2.x;
    p2(1, 0) = lines[1].p2.y;
    p2(2, 0) = 1;

    cv::Mat_<float> p1_bird = H * p1;
    cv::Mat_<double> p2_bird = H * p2;

    cv::Point2f wp1 = window_to_world(cv::Point2f(p1_bird(0, 0), p1_bird(1, 0)));
    cv::Point2f wp2 = window_to_world(cv::Point2f(p2_bird(0, 0), p2_bird(1, 0)));

    std::cout << "len=" << sqrt(pow(wp1.x - wp2.x, 2) + pow(wp1.y - wp2.y, 2)) << "\n";

    cv::imshow("birdview", destination);

    return destination;
}

void mouseCallback(int event, int x, int y, int flags, void *param) {
    const int size = 5;

    if (event == cv::EVENT_LBUTTONDOWN) {
        if (!in_the_middle_of_setting_line) {
            lines.push_back({});

            std::cout << x << " " << y << "\n";

            lines.back().p1 = cv::Point(x, y);
            in_the_middle_of_setting_line = true;

            cv::circle(frame, cv::Point(x, y), size, cv::Scalar(0, 0, 255), size);

            cv::imshow("video", frame);
        } else {
            lines.back().p2 = cv::Point(x, y);
            in_the_middle_of_setting_line = false;

            cv::circle(frame, cv::Point(x, y), size, cv::Scalar(0, 0, 255), size);
            cv::line(frame, lines.back().p1, lines.back().p2, cv::Scalar(0, 255, 0), 5, CV_AA);

            if (lines.size() == 2) {
                float x1 = lines[0].p1.x;
                float x2 = lines[0].p2.x;
                float x3 = lines[1].p1.x;
                float x4 = lines[1].p2.x;

                float y1 = lines[0].p1.y;
                float y2 = lines[0].p2.y;
                float y3 = lines[1].p1.y;
                float y4 = lines[1].p2.y;

                intersection.x =
                    ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) /
                    ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
                intersection.y =
                    ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) /
                    ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

                cv::circle(frame, intersection, size, cv::Scalar(255, 0, 0), size);
                cv::line(frame, lines[0].p2, intersection, cv::Scalar(0, 255, 0), 5, CV_AA);
                cv::line(frame, lines[1].p2, intersection, cv::Scalar(0, 255, 0), 5, CV_AA);

                birdView(frame);
            }

            cv::imshow("video", frame);
        }
    }
}

int main(int argc, char **argv) {
    FOV_x = 2 * atan(double(frameWidth) / 2 / focal_length);
    FOV_y = 2 * atan(double(frameHeight) / 2 / focal_length);

    std::cout << FOV_x / PI * 180 << " " << FOV_y / PI * 180 << "\n";

    camera_height = 1.2;

    cv::VideoCapture capture("data1/4/camera1.avi");

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

            original_frame = frame.clone();

            break;
        }
    }

    cv::setMouseCallback("video", mouseCallback, 0);
    while (true)
        if (cv::waitKey(0) == 27)
            break;

    return 0;
}

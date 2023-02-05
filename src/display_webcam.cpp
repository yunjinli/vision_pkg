#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

int main(int argc, char** argv) {
  cv::VideoCapture capture(0);
  if (!capture.isOpened()) {
    // error in opening the video input
    std::cerr << "Unable to open file!" << std::endl;
  }
  while (true) {
    cv::Mat frame;
    capture >> frame;
    if (frame.empty()) break;
    // int down_sample_factor = 2;
    int down_width = 320;
    int down_height = 240;
    cv::Mat resized_down;
    // resize down
    cv::resize(frame, resized_down, cv::Size(down_width, down_height),
               cv::INTER_LINEAR);
    cv::imshow("frame", resized_down);
    int keyboard = cv::waitKey(30);
    if (keyboard == 'q' || keyboard == 27) break;
  }
}
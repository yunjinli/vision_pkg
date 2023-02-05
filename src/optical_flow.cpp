#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/optflow.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv) {
  const string about =
      "This sample demonstrates Lucas-Kanade Optical Flow calculation.\n"
      "The example file can be downloaded from:\n"
      "  "
      "https://www.bogotobogo.com/python/OpenCV_Python/images/"
      "mean_shift_tracking/slow_traffic_small.mp4";
  const string keys =
      "{ h help |      | print this help message }"
      "{ @image | vtest.avi | path to image file }";
  CommandLineParser parser(argc, argv, keys);
  parser.about(about);
  if (parser.has("help")) {
    parser.printMessage();
    return 0;
  }
  string filename = samples::findFile(parser.get<string>("@image"));
  if (!parser.check()) {
    parser.printErrors();
    return 0;
  }

  VideoCapture capture(filename);
  if (!capture.isOpened()) {
    // error in opening the video input
    cerr << "Unable to open file!" << endl;
    return 0;
  }
  Mat frame1, prvs;
  capture >> frame1;
  cvtColor(frame1, prvs, COLOR_BGR2GRAY);
  while (true) {
    Mat frame2, next;
    capture >> frame2;
    if (frame2.empty()) break;
    cvtColor(frame2, next, COLOR_BGR2GRAY);
    Mat flow(prvs.size(), CV_32FC2);
    calcOpticalFlowFarneback(prvs, next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
    // visualization
    Mat flow_parts[2];
    split(flow, flow_parts);
    Mat magnitude, angle, magn_norm;
    cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
    normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);
    angle *= ((1.f / 360.f) * (180.f / 255.f));
    // build hsv image
    Mat _hsv[3], hsv, hsv8, bgr;
    _hsv[0] = angle;
    _hsv[1] = Mat::ones(angle.size(), CV_32F);
    _hsv[2] = magn_norm;
    merge(_hsv, 3, hsv);
    hsv.convertTo(hsv8, CV_8U, 255.0);
    cvtColor(hsv8, bgr, COLOR_HSV2BGR);
    imshow("frame2", bgr);
    int keyboard = waitKey(30);
    if (keyboard == 'q' || keyboard == 27) break;
    prvs = next;
  }
}
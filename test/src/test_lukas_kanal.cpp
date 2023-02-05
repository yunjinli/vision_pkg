#include <gtest/gtest.h>
#include <lukas_kanal.h>

TEST(LukasKanalTestSuite, DummyTest) {
  ASSERT_TRUE(2 + 5 == 7);
  ASSERT_LE(23424, 435634563456);
}

TEST(LukasKanalTestSuite, CreateMethodTest) {
  std::string name = "lukas_kanal";
  std::shared_ptr<OpticalFlow> method = OpticalFlow::create(name);
}

TEST(LukasKanalTestSuite, AddORBDetector) {
  std::string name = "lukas_kanal";
  std::shared_ptr<OpticalFlow> method = OpticalFlow::create(name);
  method->initialize();
}

// TEST(LukasKanalTestSuite, DetecCornersRealtime) {
//   std::string name = "lukas_kanal";
//   std::shared_ptr<OpticalFlow> method = OpticalFlow::create(name);
//   method->initialize();

//   cv::VideoCapture capture(0);
//   if (!capture.isOpened()) {
//     // error in opening the video input
//     std::cerr << "Unable to open file!" << std::endl;
//   }
//   cv::Mat frame1;
//   cv::Mat frame2;
//   cv::Mat flow;
//   capture >> frame1;
//   while (true) {
//     capture >> frame2;
//     if (frame2.empty()) break;
//     method->compute(frame1, frame2, flow);

//     //-- Draw keypoints
//     cv::Mat img1_keypoints;
//     // cv::Mat img2_keypoints;
//     cv::drawKeypoints(frame1, method->keypoint1(), img1_keypoints);
//     // cv::drawKeypoints(frame2, method->keypoint2(), img2_keypoints);
//     //-- Show detected (drawn) keypoints
//     cv::imshow("Oriented FAST Keypoints frame1", img1_keypoints);
//     // cv::imshow("Oriented FAST Keypoints frame2", img2_keypoints);
//     frame1 = frame2;
//     int keyboard = cv::waitKey(30);
//     if (keyboard == 'q' || keyboard == 27) break;
//   }
// }

// TEST(LukasKanalTestSuite, DetectMatchesRealtime) {
//   std::string name = "lukas_kanal";
//   std::shared_ptr<OpticalFlow> method = OpticalFlow::create(name);

//   method->initialize();

//   cv::VideoCapture capture(0);
//   if (!capture.isOpened()) {
//     // error in opening the video input
//     std::cerr << "Unable to open file!" << std::endl;
//   }
//   cv::Mat frame1;
//   cv::Mat frame2;
//   cv::Mat flow;
//   cv::Mat img_match;
//   capture >> frame1;
//   while (true) {
//     capture >> frame2;
//     if (frame2.empty()) break;
//     method->compute(frame1, frame2, flow);

//     //-- Draw keypoints
//     cv::Mat img1_keypoints;
//     cv::Mat img2_keypoints;
//     // cv::drawKeypoints(frame1, method->keypoint1(), img1_keypoints);
//     // cv::drawKeypoints(frame2, method->keypoint2(), img2_keypoints);
//     //-- Show detected (drawn) keypoints
//     // cv::imshow("Oriented FAST Keypoints frame2", img2_keypoints);
//     cv::drawMatches(frame1, method->keypoint1(), frame2, method->keypoint2(),
//                     method->matches(), img_match);
//     cv::imshow("All matches", img_match);
//     frame1 = frame2;
//     int keyboard = cv::waitKey(30);
//     if (keyboard == 'q' || keyboard == 27) break;
//   }
// }

TEST(LukasKanalTestSuite, CalcOpticalFlowRealtime) {
  std::string name = "lukas_kanal";
  std::shared_ptr<OpticalFlow> method = OpticalFlow::create(name);

  method->initialize();

  cv::VideoCapture capture(0);
  // cv::VideoCapture capture(cv::samples::findFile("../../data/people.mp4"));

  if (!capture.isOpened()) {
    // error in opening the video input
    std::cerr << "Unable to open file!" << std::endl;
  }
  cv::Mat frame1;

  cv::Mat old;

  capture >> frame1;
  cv::cvtColor(frame1, old, cv::COLOR_BGR2GRAY);
  while (true) {
    cv::Mat frame2;
    cv::Mat next;
    cv::Mat flow;
    capture >> frame2;
    cv::cvtColor(frame2, next, cv::COLOR_BGR2GRAY);
    if (frame2.empty()) break;
    method->compute(old, next, flow);
    // cv::Mat flow_keypoints;
    // cv::drawKeypoints(flow, method->keypoint2(), flow_keypoints);
    cv::imshow("Lukas Kanal Optical Flow", flow);
    // frame1 = frame2;
    old = next;
    int keyboard = cv::waitKey(30);
    if (keyboard == 'q' || keyboard == 27) break;
  }
}

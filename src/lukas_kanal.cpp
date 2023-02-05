#include <lukas_kanal.h>

std::shared_ptr<OpticalFlow> OpticalFlow::create(const std::string& name) {
  if (name == LukasKanal::getName()) {
    return std::shared_ptr<OpticalFlow>(new LukasKanal());
  } else {
    std::cerr << "Optical method " << name << " is not implemented."
              << std::endl;
    std::abort();
  }
}

void LukasKanal::initialize() {
  feature_detector_ = cv::ORB::create();
  feature_descriptor_ = cv::ORB::create();
  feature_matcher_ = cv::DescriptorMatcher::create("BruteForce-Hamming");
}

void LukasKanal::compute(const cv::Mat& img1, const cv::Mat& img2,
                         cv::Mat& flow) {
  keypoints1_.clear();
  keypoints2_.clear();
  matches_.clear();
  // First, we detect keypoints
  feature_detector_->detect(img1, keypoints1_);
  feature_detector_->detect(img2, keypoints2_);
  // Second, we transform keypoints into descriptors
  feature_descriptor_->compute(img1, keypoints1_, descriptors1_);
  feature_descriptor_->compute(img2, keypoints2_, descriptors2_);

  // BFMatcher matcher ( NORM_HAMMING );
  feature_matcher_->match(descriptors1_, descriptors2_, matches_);
  // feature_matcher_->knnMatch(descriptors1_, descriptors2_, matches_, 10);

  calc_flow(img1, img2, flow);
}

void LukasKanal::calc_flow(const cv::Mat& img1, const cv::Mat& img2,
                           cv::Mat& flow) {
  flow = img2.clone();
  const int& length_ = img1.cols;
  const int& height_ = img1.rows;
  // We have to determine the size of w(x)
  int l = 10;
  int h = 10;
  int delta = 1;
  for (const auto& m : matches_) {
    cv::Point p = keypoints1_[m.queryIdx].pt;
    int x_start = std::max(0, p.x - (int)(l / 2));
    int x_end = std::min(length_, p.x + (int)(l / 2));

    int y_start = std::max(0, p.y - (int)(h / 2));
    int y_end = std::min(length_, p.y + (int)(h / 2));

    double M_data[2][2] = {0, 0, 0, 0};
    double q_data[2][1] = {0, 0};
    cv::Mat M(2, 2, CV_64F, M_data);
    cv::Mat q(2, 1, CV_64F, q_data);

    // Iterate through the window
    for (int i = x_start + delta; i <= x_end - delta; i++) {
      for (int j = y_start + delta; j <= y_end - delta; j++) {
        int x1 = i - delta,
            x2 = i + delta;  // we can then calculate dx = x2 - x1
        int y1 = j - delta,
            y2 = j + delta;  // we can then calculate dy = y2 - y1

        double Ix =
            ((int)img1.at<uchar>(j, x1) - (int)img1.at<uchar>(j, x2)) / 2;
        double Iy =
            ((int)img1.at<uchar>(y1, i) - (int)img1.at<uchar>(y2, i)) / 2;
        double It = ((int)img1.at<uchar>(j, i) - (int)img2.at<uchar>(j, i));

        M.at<double>(0, 0) += Ix * Ix;
        M.at<double>(1, 1) += Iy * Iy;
        M.at<double>(0, 1) += Ix * Iy;
        M.at<double>(1, 0) += Iy * Ix;
        q.at<double>(0, 0) += It * Ix;
        q.at<double>(1, 0) += It * Iy;
      }
    }
    double v_data[2][1] = {0, 0};
    cv::Mat v(2, 1, CV_64F, v_data);
    v = -M.inv() * q;
    cv::Point temp =
        p + cv::Point(v.at<double>(0, 0) * 10, v.at<double>(1, 0) * 10);
    cv::circle(flow, p, 2, cv::Scalar(255));
    cv::arrowedLine(flow, p, temp, cv::Scalar(255), 1);
  }
}
#ifndef LUKAS_KANAL_H_
#define LUKAS_KANAL_H_

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

class OpticalFlow {
 public:
  virtual std::string name() const = 0;
  virtual ~OpticalFlow() = default;
  // virtual cv::Mat compute(const cv::Mat img1, const cv::Mat img2);
  static std::shared_ptr<OpticalFlow> create(const std::string& name);
  virtual std::vector<cv::KeyPoint>& keypoint1() = 0;
  virtual std::vector<cv::KeyPoint>& keypoint2() = 0;
  virtual std::vector<cv::DMatch>& matches() = 0;
  virtual void initialize() = 0;
  virtual void compute(const cv::Mat& img1, const cv::Mat& img2,
                       cv::Mat& flow) = 0;

 private:
};

class LukasKanal : public OpticalFlow {
 public:
  static std::string getName() { return "lukas_kanal"; }
  std::string name() const { return getName(); }
  LukasKanal() = default;
  std::vector<cv::KeyPoint>& keypoint1() { return keypoints1_; }
  std::vector<cv::KeyPoint>& keypoint2() { return keypoints2_; }
  std::vector<cv::DMatch>& matches() { return matches_; }
  // cv::Mat compute(const cv::Mat img1, const cv::Mat img2);
  void initialize();
  void compute(const cv::Mat& img1, const cv::Mat& img2, cv::Mat& flow);

 private:
  void calc_flow(const cv::Mat& img1, const cv::Mat& img2, cv::Mat& flow);
  cv::Ptr<cv::FeatureDetector> feature_detector_;
  cv::Ptr<cv::DescriptorExtractor> feature_descriptor_;
  cv::Ptr<cv::DescriptorMatcher> feature_matcher_;
  std::vector<cv::KeyPoint> keypoints1_, keypoints2_;
  cv::Mat descriptors1_, descriptors2_;
  std::vector<cv::DMatch> matches_;
};

#endif /* LUKAS_KANAL_H_ */

/**
 * @file test.cpp
 * @author Lowell Lobo
 * @author Mayank Deshpande
 * @author Kautilya Chappidi
 * @brief Declares unit tests to be run and verified using GitHub Actions
 * It helpes in testing detection, displaying and tracking classes
 * @version 0.1
 * @date 2023-10-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <tuple>
#include <utility>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>

#include "detection.hpp"
#include "tracking.hpp"

/**
 * @brief static TrackingClass object to be used in the unit test
 *
 */
// static TrackingClass
// obj_("../../models/res10_300x300_ssd_iter_140000_fp16.caffemodel",
// "../../models/deploy.prototxt", 0, 0, 0, 1.57, 0.7);

/**
 * @brief Construct a new TEST object.
 * unit test for checking the initVideoStream method of class DetectionClass
 */
TEST(unit_test_initialise_VideoStream, this_should_pass) {
  DetectionClass obj(
      "../../models/res10_300x300_ssd_iter_140000_fp16.caffemodel",
      "../../models/deploy.prototxt");
  bool val = obj.initVideoStream(-1);

  EXPECT_FALSE(val);
}

/**
 * @brief Construct a new TEST object.
 * unit test for checking the detectFaces method of class DetectionClass
 */
TEST(unit_test_detect_faces, this_should_pass) {
  DetectionClass obj(
      "../../models/res10_300x300_ssd_iter_140000_fp16.caffemodel",
      "../../models/deploy.prototxt");
  // obj.initVideoStream(0);
  cv::Mat frame = cv::imread("../../assets/faceImage.jpg");
  // obj.videoCapture >> frame;
  auto val = obj.detectFaces(frame);

  EXPECT_EQ(val.size(), 1);
}

/**
 * @brief Construct a new TEST object.
 * unit test for checking the assignIDAndTrack method of class TrackingClass
 */
TEST(unit_test_assign_ID, this_should_pass) {
  DetectionClass obj(
      "../../models/res10_300x300_ssd_iter_140000_fp16.caffemodel",
      "../../models/deploy.prototxt");
  TrackingClass obj_(
      "../../models/res10_300x300_ssd_iter_140000_fp16.caffemodel",
      "../../models/deploy.prototxt", 0, 0, 0, 1.57, 0.7);
  cv::Mat frame = cv::imread("../../assets/faceImage.jpg");
  auto val = obj.detectFaces(frame);
  auto ids = obj_.assignIDAndTrack(val);

  EXPECT_EQ(ids.size(), 1);
}

/**
 * @brief Construct a new TEST object.
 * unit test for checking the distFromCamera method of class TrackingClass
 */
TEST(unit_test_dist_from_camera, this_should_pass) {
  DetectionClass obj(
      "../../models/res10_300x300_ssd_iter_140000_fp16.caffemodel",
      "../../models/deploy.prototxt");
  TrackingClass obj_(
      "../../models/res10_300x300_ssd_iter_140000_fp16.caffemodel",
      "../../models/deploy.prototxt", 0, 0, 0, 1.57, 0.7);
  cv::Mat frame = cv::imread("../../assets/faceImage.jpg");
  auto val = obj.detectFaces(frame);
  obj_.obstacleMapVector = obj_.assignIDAndTrack(val);
  auto distCamera = obj_.distFromCamera(frame.cols, frame.rows);

  EXPECT_GT(sqrt(pow(std::get<0>(distCamera.at(1)), 2) +
                 pow(std::get<1>(distCamera.at(1)), 2) +
                 pow(std::get<2>(distCamera.at(1)), 2)),
            2);
}

/**
 * @brief Construct a new TEST object.
 * unit test for checking the distFromCar method of class TrackingClass
 */
TEST(unit_test_dist_from_car, this_should_pass) {
  DetectionClass obj(
      "../../models/res10_300x300_ssd_iter_140000_fp16.caffemodel",
      "../../models/deploy.prototxt");
  TrackingClass obj_(
      "../../models/res10_300x300_ssd_iter_140000_fp16.caffemodel",
      "../../models/deploy.prototxt", 0, 0, 0, 1.57, 0.7);
  cv::Mat frame = cv::imread("../../assets/multi_faces.jpg");
  auto val = obj.detectFaces(frame);
  obj_.obstacleMapVector = obj_.assignIDAndTrack(val);
  auto distCamera = obj_.distFromCamera(frame.cols, frame.rows);
  auto distCar = obj_.distFromCar(distCamera);

  EXPECT_GT(sqrt(pow(std::get<0>(distCar.at(1)), 2) +
                 pow(std::get<1>(distCar.at(1)), 2) +
                 pow(std::get<2>(distCar.at(1)), 2)),
            2);
}

/**
 * @brief Construct a new TEST object.
 * unit test for checking the findDepth method of class TrackingClass
 */
TEST(unit_test_find_depth, this_should_pass) {
  DetectionClass obj(
      "../../models/res10_300x300_ssd_iter_140000_fp16.caffemodel",
      "../../models/deploy.prototxt");
  TrackingClass obj_(
      "../../models/res10_300x300_ssd_iter_140000_fp16.caffemodel",
      "../../models/deploy.prototxt", 0, 0, 0, 1.57, 0.7);
  cv::Mat frame = cv::imread("../../assets/multi_faces.jpg");
  auto val = obj.detectFaces(frame);
  obj_.obstacleMapVector = obj_.assignIDAndTrack(val);
  auto depth = obj_.findDepth(1);

  EXPECT_GT(depth, 0);
}

/**
 * @brief Construct a new TEST object.
 * unit test for checking the assignIDAndTrack method of class TrackingClass
 */
TEST(unit_test_assign_ID_2, this_should_pass) {
  DetectionClass obj(
      "../../models/res10_300x300_ssd_iter_140000_fp16.caffemodel",
      "../../models/deploy.prototxt");
  TrackingClass obj_(
      "../../models/res10_300x300_ssd_iter_140000_fp16.caffemodel",
      "../../models/deploy.prototxt", 0, 0, 0, 1.57, 0.7);
  cv::VideoCapture videoCapture("../../assets/video.mp4");
  cv::Mat frame;
  while (true) {
    videoCapture >> frame;
    if (frame.empty()) {
      break;
    }
    auto val = obj.detectFaces(frame);
    auto ids = obj_.assignIDAndTrack(val);

    EXPECT_GT(ids.size(), 0);
  }
}

/**
 * @brief Construct a new TEST object.
 * unit test for checking the findDepth method of class TrackingClass
 */
TEST(unit_test_no_face, this_should_pass) {
  DetectionClass obj(
      "../../models/res10_300x300_ssd_iter_140000_fp16.caffemodel",
      "../../models/deploy.prototxt");
  TrackingClass obj_(
      "../../models/res10_300x300_ssd_iter_140000_fp16.caffemodel",
      "../../models/deploy.prototxt", 0, 0, 0, 1.57, 0.7);
  cv::Mat frame = cv::imread("../../assets/no_face.jpg");
  auto val = obj.detectFaces(frame);
  obj_.obstacleMapVector = obj_.assignIDAndTrack(val);
  auto depth = obj_.findDepth(1);

  EXPECT_GT(depth, 0);
}

/**
 * @brief Construct a new TEST object.
 * unit test for checking the findDepth method of class TrackingClass
 */
TEST(unit_test_close_face, this_should_pass) {
  DetectionClass obj(
      "../../models/res10_300x300_ssd_iter_140000_fp16.caffemodel",
      "../../models/deploy.prototxt");
  TrackingClass obj_(
      "../../models/res10_300x300_ssd_iter_140000_fp16.caffemodel",
      "../../models/deploy.prototxt", 0, 0, 0, 1.57, 0.7);
  cv::Mat frame = cv::imread("../../assets/too_close.jpg");
  auto val = obj.detectFaces(frame);
  obj_.obstacleMapVector = obj_.assignIDAndTrack(val);
  auto depth = obj_.findDepth(1);

  EXPECT_EQ(depth, 0);
}

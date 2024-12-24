/**
Copyright © 2023 <copyright holders>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/**
 * @file main.cpp
 * @author Lowell Lobo
 * @author Mayank Deshpande
 * @author Kautilya Chappidi
 * @brief The main application of the project, will be used to test the
 * implementation in the future.
 * @version 0.1
 * @date 2023-10-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <iostream>
#include <opencv2/imgcodecs.hpp>

#include "tracking.hpp"

/**
 * @brief The main method of the file used to test and check whether the library
 * works as intended
 *
 * @return int
 */
int main() {
  /**
   * @brief variables used to get the natural configuration of camera and car
   *
   */
  double x, y, z, th, tv;
  std::cout << "Enter x offset distance in inches: ";
  std::cin >> x;
  std::cout << std::endl << "Enter y offset distance in inches: ";
  std::cin >> y;
  std::cout << std::endl << "Enter z offset distance in inches: ";
  std::cin >> z;
  std::cout << std::endl << "Enter horizontal field of view angle in radians: ";
  std::cin >> th;
  std::cout << std::endl << "Enter vertical field of view angle in radians: ";
  std::cin >> tv;

  /**
   * @brief Initialise a tracker class to be used for tracking obstacles
   *
   */
  TrackingClass tracker("models/res10_300x300_ssd_iter_140000_fp16.caffemodel",
                        "models/deploy.prototxt", x, y, z, th, tv);

  /**
   * @brief Initialise the video
   *
   */
  if (!tracker.image.initVideoStream(0)) {
    return 0;
  }

  /**
   * @brief Initialise a frame
   *
   */
  cv::Mat frame;

  while (true) {
    tracker.image.videoCapture >> frame;

    /**
     * @brief Get detections and store in variable rectangles
     *
     */
    auto rectangles = tracker.image.detectFaces(frame);

    /**
     * @brief Assign IDs to the detections
     *
     */
    tracker.obstacleMapVector = tracker.assignIDAndTrack(rectangles);

    /**
     * @brief Find distance from camera frame
     *
     */
    auto cameraDistance = tracker.distFromCamera(frame.cols, frame.rows);

    /**
     * @brief Find distance from car frame in inches
     *
     */
    auto carDistance = tracker.distFromCar(cameraDistance);

    cv::Scalar color(0, 105, 205);

    /**
     * @brief Draw rectangles for the detections and the distances
     *
     */
    for (const auto& r : tracker.obstacleMapVector) {
      std::cout << "Obstacle " << r.first << " at point ("
                << static_cast<int>(std::get<0>(carDistance[r.first])) << ", "
                << static_cast<int>(std::get<1>(carDistance[r.first])) << ", "
                << static_cast<int>(std::get<2>(carDistance[r.first])) << ")"
                << std::endl;
      cv::rectangle(frame, r.second, color, 4);
      cv::putText(frame,
                  std::to_string(r.first) + ": (" +
                      std::to_string(
                          static_cast<int>(std::get<0>(carDistance[r.first]))) +
                      ", " +
                      std::to_string(
                          static_cast<int>(std::get<1>(carDistance[r.first]))) +
                      ", " +
                      std::to_string(
                          static_cast<int>(std::get<2>(carDistance[r.first]))) +
                      ")",
                  cv::Point(r.second.x, r.second.y - 5),
                  cv::FONT_HERSHEY_COMPLEX, 1.0, CV_RGB(255, 0, 0), 2);
    }

    /**
     * @brief Display the frame
     *
     */
    imshow("Image", frame);
    int esc_key = 27;
    if (cv::waitKey(10) == esc_key) {
      break;
    }
  }

  cv::destroyAllWindows();
}

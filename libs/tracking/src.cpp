/**
Copyright © 2023 <copyright holders>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/**
 * @file src.cpp
 * @author Lowell Lobo
 * @author Mayank Deshpande
 * @author Kautilya Chappidi
 * @brief Class Decleration for the TrackingClass
 * @version 0.1
 * @date 2023-10-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "tracking.hpp"

/**
 * @brief Default constructor.
 */
TrackingClass::TrackingClass(const std::string& detectModelPath,
                             const std::string& detectConfigPath, double x,
                             double y, double z, double th, double tv)
    : xOffset(x),
      yOffset(y),
      zOffset(z),
      horizontalFOI(th),
      verticalFOI(tv),
      count(0),
      image(detectModelPath, detectConfigPath) {}

/**
 * @brief Default Destructor.
 */
TrackingClass::~TrackingClass() {}

/**
 * @brief Finds the depth of an object in the scene.
 * The depth is found analytically by comparing detection height values and
 * z distances. A series of linear functions are then built using the
 * tested cases to predict the z distance.
 *
 * @param id Variable used to access the obstacles in the obstacleMap
 * @return double The depth of the object in meters.
 */
double TrackingClass::findDepth(int id) {
  double height = obstacleMapVector[id].height;
  if (height < 108) {
    return ((-73) * (height - 108) / (56)) + 46;
  } else if (height < 251) {
    return ((-25) * (height - 251) / (143)) + 21;
  } else if (height < 405) {
    return ((-12) * (height - 405) / (154)) + 9;
  } else if (height < 445) {
    return ((-9) * (height - 405) / (35)) + 9;
  } else {
    return 0.0;
  }
}

/**
 * @brief Assigns IDs to objects in the scene.
 * 1 - When the obstacleMap is empty the method adds new detections into the
 * Map. 2 - When the number of detected obstacles and current obstacles are the
 * same, the function will find the minimum Euclidean distance value and
 * reassign the ID to the nearest detection. 3 - When the number of detected
 * obstacles are greater than the current obstacles, all current obstacles will
 * be mapped and then the remaining detections will be assigned a new ID. 4 -
 * When the number of detected obstacles are lesser than the current obstacles,
 * all current obstacles will be mapped and only obstacles near the edge of
 * frame will be deleted. If the obstacle is in the center, the algorithm
 * assumes that the detection failed because of low accuracy and when the
 * obstacle reappears the ID will be reassigned, in the mean time the obstacle
 * bounding box will be displayed at the last detected location.
 *
 * @param detections A vector containing all the detected faces in image frame
 * @return std::map<int, cv::Rect> A map of object IDs to object names.
 */
std::map<int, cv::Rect> TrackingClass::assignIDAndTrack(
    std::vector<cv::Rect>& detections) {
  int len = detections.size();
  int lenObjMap = obstacleMapVector.size();

  if (detections.size() > 0) {
    for (const auto& r : obstacleMapVector) {
      std::vector<double> distances(detections.size());
      std::map<double, cv::Rect> distMap;
      if (detections.size() > 0) {
        for (int i = 0; i < detections.size(); i++) {
          distances.push_back(pow((detections[i].x - r.second.x), 2) +
                              pow((detections[i].x - r.second.x), 2));
          distMap[distances[i]] = detections[i];
        }
      } else {
        distances.push_back(-100);
      }

      int minVal = *std::min_element(distances.begin(), distances.end());

      if ((len < lenObjMap)) {
        if (((r.second.x < 10) || (r.second.y < 10) ||
             (r.second.x + r.second.width > 640 - 10) ||
             (r.second.y + r.second.height > 480 - 10)) &&
            (minVal < 0)) {
          obstacleMapVector.erase(r.first);
        } else if (((r.second.x < 10) || (r.second.y < 10) ||
                    (r.second.x + r.second.width > 640 - 10) ||
                    (r.second.y + r.second.height > 480 - 10)) &&
                   (minVal > 2500)) {
          obstacleMapVector.erase(r.first);
        } else if ((minVal < 0) || (minVal > 2500)) {
          continue;
        } else if (detections.size() > 0) {
          obstacleMapVector[r.first] = distMap[minVal];
          detections.erase(
              std::find(detections.begin(), detections.end(), distMap[minVal]));
        }
      } else {
        obstacleMapVector[r.first] = distMap[minVal];
        detections.erase(
            std::find(detections.begin(), detections.end(), distMap[minVal]));
      }
    }

    if (len > lenObjMap) {
      for (int i = 0; i < len - obstacleMapVector.size(); i++) {
        obstacleMapVector[++count] = detections[i];
      }
    }

    if (obstacleMapVector.size() > 0) {
      return obstacleMapVector;
    }

    return std::map<int, cv::Rect>();
  } else {
    count = 0;
    return std::map<int, cv::Rect>();
  }
}

/**
 * @brief The method used to compute the (x, y, z) distance of obstacle in
 * Camera Reference Frame. Finds the centroid values of x and y, and gets z
 * value by calling the findDepth function.
 *
 * @param frameWidth The pixel width of the image frame
 * @param frameHeight The pixel height of the image frame
 * @return std::map<int, std::tuple<double, double, double>> A map containing
 * the object ID and tuple containing distance in meters
 */
std::map<int, std::tuple<double, double, double>> TrackingClass::distFromCamera(
    int frameWidth, int frameHeight) {
  std::map<int, std::tuple<double, double, double>> distances;
  for (const auto& r : obstacleMapVector) {
    double z = findDepth(r.first);
    double xDist = r.second.x - (frameWidth / 2) + (r.second.width / 2);
    double yDist = r.second.y - (frameHeight / 2) + (r.second.height / 2);
    distances[r.first] = std::make_tuple(xDist, yDist, z);
  }
  return distances;
}

/**
 * @brief Calculates the distance between an object and the car.
 * Converts the Camera reference distance into Car reference distance, using
 * geometry. The method takes into account prototype configurations, such that
 * the camera not being placed at the origin of car reference and field of view
 * values of different cameras.
 *
 * @param input A map with containing the object ID, and tuple containing
 * distance in meters
 *
 *
 * @return A map with containing the object ID, and tuple containing distance in
 * meters
 *
 */
std::map<int, std::tuple<double, double, double>> TrackingClass::distFromCar(
    std::map<int, std::tuple<double, double, double>>& input) {
  std::map<int, std::tuple<double, double, double>> distances;

  for (const auto& r : input) {
    double x, y;
    double z = std::get<2>(input[r.first]);
    x = 2 * z * tan(horizontalFOI / 2) * std::get<0>(input[r.first]) /
        480;  // width
    y = 2 * z * tan(verticalFOI / 2) * std::get<1>(input[r.first]) /
        640;  // height
    double xDist = x + xOffset;
    double yDist = z + yOffset;
    double zDist = -y + zOffset;
    distances[r.first] = std::make_tuple(xDist, yDist, zDist);
        // add each offset to each coord from class declar var
  }
  return distances;
}

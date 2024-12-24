# Human Tracking in C++

## Table of Contents
- [Overview](#overview)
- [Project Assumptions](#project-assumptions)
- [Key Features](#key-features)
- [Constraints](#constraints)
- [Installation & Setup](#installation--setup)
- [API Libraries](#api-libraries)
  - [Detection Library](#1---detection-library)
  - [Tracking Library](#2---tracking-library)
- [Building & Running](#building--running)
  - [Build from Command Line](#build-from-command-line)
  - [Run the Application](#run-the-application)
  - [Run Unit Tests](#run-unit-tests)
  - [Generate Documentation](#generate-documentation)
- [Code Coverage](#code-coverage)
- [compile_commands.json Tips](#compile_commandsjson-tips)
- [Suggested Repository Name](#suggested-repository-name)

---

## Overview
This C++ project provides a monocular human detection and tracking module suitable for an autonomous car’s perception system. The camera feed is analyzed to detect human obstacles, assign unique IDs, and track their position in both camera and robot reference frames. The module integrates CI/CD pipelines with unit test coverage reports.

---

## Project Assumptions
- A single monocular camera is mounted on top of a car, pointing outward.  
- The camera’s coordinate system:  
  - **Z-axis** out of the lens  
  - **X-axis** to the right  
  - **Y-axis** downward  
- The robot’s coordinate system:  
  - **Z-axis** upward  
  - **X-axis** to the right  
  - **Y-axis** forward  
- Occlusion handling is limited due to monocular constraints.  
- Configuration details (e.g., transformation parameters) are user-provided.

---

## Key Features
1. **Human Detection:** Identifies human obstacles using a pretrained ResNet Caffe model via OpenCV.  
2. **Tracking & ID Assignment:** Assigns and manages unique IDs for detected obstacles based on their bounding-box positions across frames.  
3. **Reference Frames:** Computes obstacle positions with respect to both camera and robot frames.  
4. **CI/CD & Code Coverage:** Includes GitHub Actions and Codecov integration to ensure quality and maintainability.  
5. **Doxygen Documentation:** Automatically generates API docs for easy reference.

---

## Constraints
- Monocular camera usage can limit depth accuracy.  
- No advanced occlusion management implemented.  
- Real-time performance depends on system hardware and camera specs.  
- A deep learning model is required for accurate detections (ResNet Caffe model used here).

---

## Installation & Setup

### Dependencies
- **OpenCV** for video capture and image processing  
- **ResNet Caffemodel** for human detection  
- **GoogleTest** (for unit testing)  
- **CMake** (build system)  
- **Doxygen** (optional, for generating docs)  

### Installing OpenCV (Example on Ubuntu)
```bash
sudo apt-get update
sudo apt-get install libopencv-dev
```

---

## API Libraries

### 1 - Detection Library
- **Purpose:** Initializes and manages video streams, detects human faces or obstacles using a deep-learning model in OpenCV.  
- **Methods/Constructor:**  
  - **Constructor**: Loads the Caffe model with `cv::dnn::readNet()`.  
  - `initVideoStream()`: Captures frames in a loop from the camera.  
  - `detectFaces()`: Scans each frame and returns bounding boxes (as `cv::Rect`) above a confidence threshold.

### 2 - Tracking Library
- **Purpose:** Assigns IDs to detected bounding boxes, estimates their (x, y, z) location relative to both camera and robot frames, and manages obstacle IDs across multiple frames.  
- **Methods:**  
  - `assignIDAndTrack()`: Maintains ID continuity and reassigns IDs based on Euclidean distance, creating new IDs for new obstacles.  
  - `distFromCamera()`: Calculates the pixel-distance (x, y, z) from camera coordinates.  
  - `distFromCar()`: Converts camera-frame distances into robot-frame distances (in inches).  
  - `findDepth()`: Estimates depth (z) analytically, leveraging linearized sampling.

---

## Building & Running

### Build from Command Line
```bash
# Generate build files
cmake -S ./ -B build/

# Compile
cmake --build build/

# (Optional) Clean build
cmake --build build/ --target clean
rm -rf build/
```

### Run the Application
```bash
# Run the main executable
./build/app/human-tracker
```

### Run Unit Tests
```bash
cd build/
ctest
cd -
# or
ctest --test-dir build/
```

### Generate Documentation
**Method 1:**
```bash
sudo apt-get install doxygen
doxygen dconfig
```
**Method 2:**
```bash
cmake --build build/ --target docs
open docs/html/index.html
```

---

## Code Coverage

1. **Install Coverage Tools**:  
   ```bash
   sudo apt-get install gcovr lcov
   ```
2. **Configure & Build with Coverage Flags**:  
   ```bash
   cmake -D WANT_COVERAGE=ON -D CMAKE_BUILD_TYPE=Debug -S ./ -B build/
   cmake --build build/ --clean-first --target all test_coverage
   open build/test_coverage/index.html
   ```
3. **Alternatively**, run coverage on the application target (`app_coverage`):
   ```bash
   cmake --build build/ --clean-first --target all app_coverage
   open build/app_coverage/index.html
   ```

---

## compile_commands.json Tips
If errors arise while generating `compile_commands.json`, remove the existing file and run:
```bash
bear -- cmake --build build/ --clean-first

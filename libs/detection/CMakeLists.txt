# Create a library called "myLib1" (in Linux, this library is created
# with the name of either libmyLib1.a or myLib1.so).
find_package(OpenCV REQUIRED)

add_library (myLib1
  # list of cpp source files:
  src.cpp
  )

# Indicate what directories should be added to the include file search
# path when using this library.
target_include_directories(myLib1 PUBLIC
  # list of directories:
  .
  ${OpenCV_INCLUDE_DIRS}
  )

  target_link_libraries(myLib1
  ${OpenCV_LIBS}
  )
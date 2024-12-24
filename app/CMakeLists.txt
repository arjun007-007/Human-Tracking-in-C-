# Any C++ source files needed to build this target (human-tracker).
add_executable(human-tracker
  # list of source cpp files:
  main.cpp
  )

# Any include directories needed to build this target.
# Note: we do not need to specify the include directories for the
# dependent libraries, they are automatically included.
target_include_directories(human-tracker PUBLIC
  # list inclue directories:
  ${CMAKE_SOURCE_DIR}/include
)

# Any dependent libraires needed to build this target.
target_link_libraries(human-tracker PUBLIC
  # list of libraries
  myLib1
  myLib3
  )

# target_link_options(human-tracker PUBLIC
#   --static
#   )
  

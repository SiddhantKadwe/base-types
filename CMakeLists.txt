cmake_minimum_required(VERSION 3.10)
project(base VERSION 1.1 DESCRIPTION "Common types for ROCK systems")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Eigen3 REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(BASE_LOGGING REQUIRED base-logging)

# Find SISL
option(USE_SISL "set to OFF to disable the usage of SISL." ON)
if(USE_SISL)
    find_library(SISL_LIBRARY sisl PATHS /usr/local/lib)
    find_path(SISL_INCLUDE_DIR sisl.h PATHS /usr/local/include)
    
    if(NOT SISL_LIBRARY OR NOT SISL_INCLUDE_DIR)
        message(FATAL_ERROR "The usage of SISL is requested, but the library could not be found."
            " Either make sure SISL is installed, or disable the usage via"
            " -DUSE_SISL=OFF")
    else()
        set(SISL_FOUND TRUE)
    endif()
endif()

if(SISL_FOUND)
    message(STATUS "SISL found: ${SISL_LIBRARIES}")
    list(APPEND OPTIONAL_HPP
        src/Spline.hpp
        src/Trajectory.hpp
    )
    list(APPEND OPTIONAL_CPP
        src/Spline.cpp
        src/Trajectory.cpp
    )
endif()

# Define header and source files
set(BASE_HEADERS_SRC
    src/Acceleration.hpp
    src/Angle.hpp
    src/CircularBuffer.hpp
    src/Deprecated.hpp
    src/Eigen.hpp
    src/Float.hpp
    src/JointLimitRange.hpp
    src/JointLimits.hpp
    src/JointState.hpp
    src/JointsTrajectory.hpp
    src/JointTransform.hpp
    src/Logging.hpp
    src/LogMessage.hpp
    src/Matrix.hpp
    src/NamedVector.hpp
    src/Point.hpp
    src/Pose.hpp
    src/Pressure.hpp
    src/RigidBodyStateSE3.hpp
    src/Singleton.hpp
    src/Temperature.hpp
    src/Time.hpp
    src/TimeMark.hpp
    src/Timeout.hpp
    src/TransformWithCovariance.hpp
    src/Twist.hpp
    src/TwistWithCovariance.hpp
    src/Waypoint.hpp
    src/Wrench.hpp
    ${OPTIONAL_HPP}
)

set(BASE_HEADERS_COMMANDS
    src/commands/Joints.hpp
    src/commands/Motion2D.hpp
    src/commands/Speed6D.hpp
    src/commands/LinearAngular6DCommand.hpp
)

set(BASE_HEADERS_LOGGING
    src/logging/logging_iostream_style.h
    src/logging/logging_printf_style.h
)

set(BASE_HEADERS_SAMPLES
    src/samples/Acceleration.hpp
    src/samples/BoundingBox.hpp
    src/samples/BodyState.hpp
    src/samples/CommandSamples.hpp
    src/samples/DepthMap.hpp
    src/samples/DistanceImage.hpp
    src/samples/Frame.hpp
    src/samples/IMUSensors.hpp
    src/samples/Joints.hpp
    src/samples/LaserScan.hpp
    src/samples/OrientedBoundingBox.hpp
    src/samples/Pointcloud.hpp
    src/samples/Pressure.hpp
    src/samples/RigidBodyAcceleration.hpp
    src/samples/RigidBodyState.hpp
    src/samples/RigidBodyStateSE3.hpp
    src/samples/RigidBodyStateSE3Vector.hpp
    src/samples/Sonar.hpp
    src/samples/SonarBeam.hpp
    src/samples/SonarScan.hpp
    src/samples/PoseWithCovariance.hpp
    src/samples/Twist.hpp
    src/samples/Temperature.hpp
    src/samples/Wrench.hpp
    src/samples/Wrenches.hpp
    src/samples/Event.hpp
    src/samples/EventArray.hpp
)

set(BASE_HEADERS_TEMPLATES
    src/templates/TimeStamped.hpp
)

set(BASE_SOURCES_SRC
    src/Acceleration.cpp
    src/Angle.cpp
    src/JointLimitRange.cpp
    src/JointLimits.cpp
    src/JointState.cpp
    src/JointsTrajectory.cpp
    src/JointTransform.cpp
    src/Pose.cpp
    src/Pressure.cpp
    src/RigidBodyStateSE3.cpp
    src/Temperature.cpp
    src/Time.cpp
    src/TimeMark.cpp
    src/Timeout.cpp
    src/TransformWithCovariance.cpp
    src/Twist.cpp
    src/TwistWithCovariance.cpp
    src/Waypoint.cpp
    src/Wrench.cpp
    ${OPTIONAL_CPP}
)

set(BASE_SOURCES_COMMANDS
    src/commands/Motion2D.cpp
)

set(BASE_SOURCES_SAMPLES
    src/samples/BodyState.cpp
    src/samples/BoundingBox.cpp
    src/samples/DepthMap.cpp
    src/samples/DistanceImage.cpp
    src/samples/Frame.cpp
    src/samples/Joints.cpp
    src/samples/LaserScan.cpp
    src/samples/OrientedBoundingBox.cpp
    src/samples/PoseWithCovariance.cpp
    src/samples/Pressure.cpp
    src/samples/RigidBodyAcceleration.cpp
    src/samples/RigidBodyState.cpp
    src/samples/Sonar.cpp
    src/samples/SonarBeam.cpp
    src/samples/SonarScan.cpp
    src/samples/Temperature.cpp
)

# Create the shared library
add_library(base-types SHARED
    ${BASE_HEADERS_SRC}
    ${BASE_HEADERS_COMMANDS}
    ${BASE_HEADERS_LOGGING}
    ${BASE_HEADERS_SAMPLES}
    ${BASE_HEADERS_TEMPLATES}
    ${BASE_SOURCES_SRC}
    ${BASE_SOURCES_COMMANDS}
    ${BASE_SOURCES_SAMPLES}
)

# Include directories
target_include_directories(base-types PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${BOOST_INCLUDEDIR}
    ${BASE_LOGGING_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIR}
)    

# include_directories()

# Link with dependencies
target_link_libraries(base-types PRIVATE ${BASE_LOGGING_LIBRARIES})
target_link_libraries(base-types PUBLIC
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
)

if(SISL_FOUND)
    target_include_directories(base-types PUBLIC
        ${SISL_INCLUDE_DIR}
    ) 
    target_link_libraries(base-types PUBLIC
        ${SISL_LIBRARY}
    )
    install(FILES ${CMAKE_SOURCE_DIR}/src/Spline.hpp
            DESTINATION include/base/geometry)
endif()

# Generate and install pkg-config files
configure_file(base-lib.pc.in ${CMAKE_BINARY_DIR}/base-lib.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/base-lib.pc DESTINATION lib/pkgconfig)

# Install headers
install(FILES ${BASE_HEADERS_SRC} DESTINATION include/base/)
install(FILES ${BASE_HEADERS_COMMANDS} DESTINATION include/base/commands/)
install(FILES ${BASE_HEADERS_LOGGING} DESTINATION include/base/logging/)
install(FILES ${BASE_HEADERS_SAMPLES} DESTINATION include/base/samples/)
install(FILES ${BASE_HEADERS_TEMPLATES} DESTINATION include/base/templates/)

# Install library
install(TARGETS base-types DESTINATION lib)

#
# The find_package macro for Orocos-RTT works best with
# cmake >= 2.6.3
#
cmake_minimum_required(VERSION 2.6.3)

#
# This creates a standard cmake project. You may extend this file with
# any cmake macro you see fit.
#
# The CMake 'target' names are identical to the first argument of the
# macro, except for orocos_typekit, where the target is fully
# controlled by generated code of 'typegen'.
#
project(yarp-transport)

# Set the CMAKE_PREFIX_PATH in case you're not using Orocos through ROS
# for helping these find commands find RTT.
find_package(OROCOS-RTT REQUIRED ${RTT_HINTS})

# Defines the orocos_* cmake macros. See that file for additional
# documentation.
include(${OROCOS-RTT_USE_FILE_PATH}/UseOROCOS-RTT.cmake)

#
# Enable testing option
#
option(ENABLE_TESTING "Enable testing")
if(ENABLE_TESTING)
	enable_testing()
endif()

#
# Looking for Yarp.
#
if(NOT DEFINED YARP_FOUND)
	find_package(YARP REQUIRED)
	include_directories(${YARP_INCLUDE_DIRS})
endif(NOT DEFINED YARP_FOUND)

#
# Building the Yarp toolkit
#
set(AC_INSTALL_DIR lib/orocos/types)
if ( ${OROCOS_TARGET} STREQUAL "gnulinux" OR ${OROCOS_TARGET} STREQUAL "lxrt" OR ${OROCOS_TARGET} STREQUAL "xenomai")
	set( LIB_NAME ${PROJECT_NAME}-${OROCOS_TARGET})
else()
	set( LIB_NAME ${PROJECT_NAME})
endif()
message( "Building library ${PROJECT_NAME}" )
add_library( ${PROJECT_NAME} SHARED YarpTransport.cpp )
set_target_properties( ${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME ${LIB_NAME}
	INSTALL_RPATH_USE_LINK_PATH 1
	LINK_FLAGS ${USE_OROCOS_LINK_FLAGS}
)
target_link_libraries( ${PROJECT_NAME} ${OROCOS-RTT_LIBRARIES} ${YARP_LIBRARIES})
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${AC_INSTALL_DIR})
link_directories( ${CMAKE_CURRENT_BINARY_DIR} )

#
# Test file
#
if(ENABLE_TESTING)
	orocos_executable(FooTest testing/FooTest.cpp)
	target_link_libraries(FooTest ${PROJECT_NAME})
	add_test(YarpTransport FooTest-${OROCOS_TARGET})
endif()

#
# Additional headers (not in typekit):
#
# Installs in the include/orocos/transports/ directory
#
orocos_install_headers( YarpTransport.hpp YarpTemplateProtocol.hpp YarpChannelElement.hpp yarp_bottle_archive.hpp ) # ...you may add multiple header files

#
# Generates and installs our package. Must be the last statement such
# that it can pick up all above settings.
#
orocos_generate_package()


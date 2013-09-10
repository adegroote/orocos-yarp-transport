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
set(AC_INSTALL_DIR lib/orocos/${OROCOS_TARGET}/types)
if ( ${OROCOS_TARGET} STREQUAL "gnulinux" OR ${OROCOS_TARGET} STREQUAL "lxrt" OR ${OROCOS_TARGET} STREQUAL "xenomai")
	set( LIB_NAME ${PROJECT_NAME}-${OROCOS_TARGET})
else()
	set( LIB_NAME ${PROJECT_NAME})
endif()
message( "Building library ${PROJECT_NAME}" )
include_directories(transport/)
add_library( ${PROJECT_NAME} SHARED transport/YarpTransport.cpp )
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
orocos_install_headers( transport/YarpTransport.hpp 
						transport/YarpTemplateProtocol.hpp
						transport/YarpChannelElement.hpp 
						transport/yarp_bottle_archive.hpp
						)

#
# Generates and installs our package. Must be the last statement such
# that it can pick up all above settings.
#
#orocos_generate_package()
set(PC_NAME ${PROJECT_NAME}-${OROCOS_TARGET})
set(PC_LIBS "Libs: ")
set(PC_LIBS "${PC_LIBS} -L\${libdir} ${OROCOS_DEFINED_LIBS}")
set(PC_LIBS "${PC_LIBS} -L\${orocos_libdir} ${OROCOS_DEFINED_COMPS}")
set(PC_LIBS "${PC_LIBS} -L\${orocos_libdir}/plugins ${OROCOS_DEFINED_PLUGINS}")
set(PC_LIBS "${PC_LIBS} -L\${orocos_libdir}/types ${OROCOS_DEFINED_TYPES}")
set(PC_PREFIX ${CMAKE_INSTALL_PREFIX})
set(PC_LIB_DIR "\${libdir}/orocos${OROCOS_SUFFIX}/${PROJECT_NAME}")
set(PC_CONTENTS "prefix=@PC_PREFIX@
libdir=\${prefix}/lib
includedir=\${prefix}/include/orocos
orocos_libdir=${PC_LIB_DIR}

Name: ${PC_NAME}
Description: ${PC_NAME} package for Orocos
Requires: orocos-rtt-${OROCOS_TARGET} ${ORO_CREATE_PC_DEPENDS}
Version: ${ORO_CREATE_PC_VERSION}
${PC_LIBS}
Cflags: -I\${includedir} -I${YARP_INCLUDE_DIRS}
")
string(CONFIGURE "${PC_CONTENTS}" INSTALLED_PC_CONTENTS @ONLY)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${PC_NAME}.pc ${INSTALLED_PC_CONTENTS})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PC_NAME}.pc DESTINATION lib/pkgconfig )
orocos_uninstall_target()


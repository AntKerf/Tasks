cmake_minimum_required(VERSION 2.8)	 
project(Clusterizer)			

set(SOURCE_EXE main.cpp)		

set(SOURCE_LIB Cluster.cpp)			

add_library(Cluster STATIC ${SOURCE_LIB})	

add_executable(Clusterizer ${SOURCE_EXE})	

target_link_libraries(Clusterizer Cluster)		

option(HAVE_BOOST "CMake try the find Boost package" OFF)
option(HAVE_OPENCV "CMake try the find OpenCV package" OFF)
################################################
if(HAVE_BOOST)
	find_package( Boost COMPONENTS filesystem REQUIRED )
	if(NOT Boost_FOUND)
		message(SEND_ERROR "Failed to find Boost::filesystem")
		return()
	else()
		include_directories(${Boost_INCLUDE_DIRS})
		target_link_libraries(Clusterizer ${Boost_LIBRARIES})
	endif()
else()
	message("without BOOST")
	message("For the project to work, you must manually specify the directory with BOOST libraries!!!")
endif()
###########################################################
if(HAVE_OPENCV)
	find_package( OpenCV REQUIRED )
	if(NOT OpenCV_FOUND)
		message(SEND_ERROR "Failed to find OpenCV")
		return()
	else()
		include_directories( ${OpenCV_INCLUDE_DIRS} )
		target_link_libraries(Clusterizer ${OpenCV_LIBS})
	endif()
else()
	message("without OpenCV")
	message("For the project to work, you must manually specify the directory with OpenCV libraries!!!")
endif()
##########################################################
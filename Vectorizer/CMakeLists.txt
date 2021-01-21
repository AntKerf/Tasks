cmake_minimum_required(VERSION 3.15)	 
project(Vectorizer)			

set(SOURCE_FILE 
	Source/main.cpp		
	Source/Vectorize.cpp
	Source/Vectorize.h)			

add_executable(Vectorizer ${SOURCE_FILE})	
		
option(HAVE_BOOST "CMake try the find Boost package" OFF)

option(HAVE_OPENCV "CMake try the find OpenCV package" OFF)

###################_find_package
###################_Boost::filesystem
if(HAVE_BOOST)
	set(Boost_USE_STATIC_LIBS ON)
	find_package( Boost COMPONENTS filesystem REQUIRED )
else()
	message("without BOOST")
	message("For the project to work, you must manually specify the directory with BOOST libraries!!!")
endif()
#############################################################
###################_OpenCV

if(HAVE_OPENCV)
	find_package( OpenCV REQUIRED )
else()
	message("without OpenCV")
	message("For the project to work, you must manually specify the directory with OpenCV libraries!!!")
endif()
##############################################################

###################_link_package
###################_Boost::filesystem
if(HAVE_BOOST)
	if(NOT Boost_FOUND)
		message(SEND_ERROR "Failed to find Boost::filesystem")
		return()
	else()
		include_directories(${Boost_INCLUDE_DIRS})
		target_link_libraries(Vectorizer ${Boost_LIBRARIES})
	endif()
endif()
################################################################
##################_OpenCV
if(HAVE_OPENCV)
	if(NOT OpenCV_FOUND)
		message(SEND_ERROR "Failed to find OpenCV")
		return()
	else()
		include_directories( ${OpenCV_INCLUDE_DIRS} )
		target_link_libraries(Vectorizer ${OpenCV_LIBS})
	endif()
endif()
##########################################################
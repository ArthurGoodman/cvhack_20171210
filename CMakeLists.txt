CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(hackathone)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ggdb -lpthread")

FIND_PACKAGE(OpenCV REQUIRED )
FIND_PACKAGE(Threads REQUIRED)

INCLUDE_DIRECTORIES(
	${OpenCV_INCLUDE_DIR}
)


#set(SOURCE_LIBS
#	)
	

#add_library(common STATIC ${SOURCE_LIBS})

add_executable(test test.cpp)


target_link_libraries(test
    ${OpenCV_LIBRARIES}
#    common
)



#SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_BUILD_TYPE Debug)


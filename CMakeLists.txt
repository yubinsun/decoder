cmake_minimum_required(VERSION
	2.10)
project(testDecoder)
find_package( OpenCV REQUIRED )
set(CMAKE_CXX_STANDARD
	14)
include_directories( 
	${OpenCV_INCLUDE_DIRS} 
	) 
add_executable(testDecoder
	src/main.cpp
	src/decode.cpp
	)
cmake_minimum_required(VERSION 2.8)
	
target_link_libraries( testDecoder ${OpenCV_LIBS} )
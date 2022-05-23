cmake_minimum_required(VERSION
	2.10)
project(testDecoder)

set(CMAKE_CXX_STANDARD
	14)

add_executable(testDecoder
	src/main.cpp
	src/decode.cpp

	)

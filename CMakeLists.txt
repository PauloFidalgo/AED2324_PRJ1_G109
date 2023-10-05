cmake_minimum_required(VERSION 3.26)
project(AED2324_PRJ1_G_109_)

set(CMAKE_CXX_STANDARD 17)

add_executable(AED2324_PRJ1_G_109_ main.cpp
        cmake-build-debug/DataModel/Class.cpp
        cmake-build-debug/DataModel/Class.h
        DataModel/Student.cpp
        DataModel/Student.h
        DataModel/Schedule.cpp
        DataModel/Schedule.h
        DataModel/Class.cpp
        DataModel/Class.cpp)

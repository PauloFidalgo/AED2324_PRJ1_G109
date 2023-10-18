cmake_minimum_required(VERSION 3.26)
project(project)

set(CMAKE_CXX_STANDARD 17)

add_executable(project main.cpp
        DataModel/TurmaUC.cpp
        DataModel/TurmaUC.h
        DataModel/UC.cpp
        DataModel/UC.h
)

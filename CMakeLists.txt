cmake_minimum_required(VERSION 3.26)
project(project)

set(CMAKE_CXX_STANDARD 17)

add_executable(project main.cpp
        DataModel/UC.cpp
        DataModel/UC.h
        DataModel/Estudante.cpp
        DataModel/Estudante.h
        DataModel/Aula.cpp
        DataModel/Aula.h
        FilesRepository/Manager.h
        FilesRepository/Manager.cpp
        FilesRepository/Pedido.cpp
        FilesRepository/Pedido.h
        FilesRepository/Menu.cpp
)

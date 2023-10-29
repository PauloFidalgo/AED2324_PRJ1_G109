#include <iostream>// Include the header file
#include "FilesRepository/Manager.h"
#include "FilesRepository/Menu.h"

int main() {

    //Menu menu;
    //menu.iniciar();
    Manager manager;
    manager.readFiles();
    manager.printNumeroDeAlunosPorAno();

    return 0;

}



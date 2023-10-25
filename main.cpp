#include <iostream>// Include the header file
#include "FilesRepository/Manager.h"
#include "FilesRepository/Menu.h"

int main() {
    Manager manager;
    manager.readFiles();
    manager.numeroEstudantesEmPeloMenosNUCS(7);
    Menu menu;
    menu.iniciar();
    return 0;
}

#include <iostream>// Include the header file
#include "FilesRepository/Manager.h"
#include "FilesRepository/Menu.h"

int main() {
    Manager manager;
    manager.readFiles();
    manager.printUCS();
    return 0;
}

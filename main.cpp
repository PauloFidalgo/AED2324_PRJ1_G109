#include <iostream>// Include the header file
#include "FilesRepository/Manager.h"
#include "FilesRepository/Menu.h"

int main() {
    Manager manager;
    manager.readFiles();
    cout << "---------------------------" << endl;
    manager.printPratica(202021492);
    cout << "---------------------------" << endl;
    cout << "---------------------------" << endl;
    manager.printPratica(202025742);
    //manager.printHorarioEstudante(202025742);
    cout << "---------------------------" << endl;
    manager.inputToPedido("L.EIC005",202021492,"H",202025742);
    return 0;
}



#include <iostream>// Include the header file
#include "FilesRepository/Manager.h"
#include "FilesRepository/Menu.h"

int main() {
    Manager manager;
    manager.readFiles();
    /*
    cout << "---------------------------" << endl;
    manager.printPratica(202021492);
    cout << "---------------------------" << endl;
    cout << "---------------------------" << endl;
    manager.printPratica(202025742);
    //manager.printHorarioEstudante(202025742);
    cout << "---------------------------" << endl;
    manager.inputToPedido("L.EIC005",202021492,"H",202025742);
    manager.printPratica(202025742); */
    auto estudante = manager.getEstudante(202025742);
    for (auto a : manager.obterHorarioEstudantePraticas(estudante)) {
        a.printData();
    }
    cout << "----------------------------" << endl;
    for (auto b : manager.obterHorarioEstudantePraticasExceto(estudante,"L.EIC003")) {
        b.printData();
    }
    cout << endl;
    string a = "LEIC";
    cout << estudante.inscrito(a) << endl;
    cout << estudante.getTurma("L.EI") << endl;
    cout << manager.verificarAulaSobreposta(manager.obterHorarioEstudantePraticas(estudante),Aula("Wednesday", 10, 2, "TP"));
    return 0;
}



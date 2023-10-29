#include <iostream>// Include the header file
#include "FilesRepository/Manager.h"
#include "FilesRepository/Menu.h"

int main() {

    Menu menu;
    menu.iniciar();
    return 0;

    Manager manager;
    manager.readFiles();

    manager.printAllDays();


    Aula aula1 = Aula("Wednesday", 10.5, 1.5, "TP");
    Aula aula5 = Aula("Wednesday", 9, 2, "TP");
    Aula aula6 = Aula("Wednesday", 13, 2, "TP");
    Aula aula7 = Aula("Wednesday", 14, 1, "TP");
    Aula aula2 = Aula("Wednesday", 11.5, 1.5, "TP");
    Aula aula3 = Aula("Wednesday", 9, 1, "TP");
    Aula aula4 = Aula("Wednesday", 10.0, 2, "T");

    vector<pair<string,pair<string,Aula>>> horario;
    horario.push_back({"U",{"TURMA1", aula1}});
    horario.push_back({"U",{"TURMA1", aula2}});
    horario.push_back({"U",{"TURMA1", aula3}});
    horario.push_back({"U",{"TURMA1", aula3}});
    horario.push_back({"U",{"TURMA1", aula4}});
    horario.push_back({"U",{"TURMA1", aula5}});
    horario.push_back({"U",{"TURMA1", aula6}});
    horario.push_back({"U",{"TURMA1", aula7}});
    manager.printHorario(horario);

    /*
    for (vector<pair<string,pair<string,Aula>>>::iterator aula = horario.begin(); aula != horario.end() ; aula++) {
        auto next = aula++;
        while (next != horario.end()) {
            aula->second.second.printData();
            next->second.second.printData();
            cout << "Sobrepoe? " << aula->second.second.sobreposta(next->second.second) << endl;
            next++;
        }
    } */

    /*
    Manager manager;
    manager.readFiles();
    cout << "---------------------------" << endl;
    //manager.printPratica(202025742);
    //cout << "---------------------------" << endl;
    cout << "---------------------------" << endl;
    //manager.printPratica(202029992);
    //manager.printHorarioEstudante(202025742);
    cout << "---------------------------" << endl;
    manager.inputToPedido("L.EIC003",202028717,"R");
    //manager.printPratica(202025742);
    auto estudante = manager.getEstudante(202027867);
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
*/
    return 0;
}



//
// Created by Wagner Pedrosa on 25/10/2023.
//

#include "Menu.h"
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;
Menu::Menu() {
    Manager manager;
    this->manager = manager;
    this->manager.readFiles();

}

Menu::~Menu() {
    exitTimeThread = true;
    exitPedidoThread =  true;
    // Join the time update thread in the destructor
    if (timeThread.joinable() || pedidoThread.joinable()) {
        timeThread.join();
        pedidoThread.join();
    }
}

void Menu::getUserInput() {
    while (true) {
        cout << "Escolha a opção: ";
        string line;
        cin >> line;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (line.length() != 1) {
            cout << "Escolha inválida, tente outra vez." << endl;
            continue;
        }
        this->userInput = line[0];
        break;
    }
}

void Menu::getStudentNumber() {
    while (true) {
        cout << "Número do Aluno: ";
        string line;
        cin >> line;

        try {
            this->numero_estudante = stoi(line);
            if (line.length() == 9) {
                if (!manager.estudanteValido(this->numero_estudante)) {
                    cout << "Aluno não encontrado! " << endl;
                    continue;
                }
            }
            break;
        }
        catch (exception e) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.clear();
            cout << "Número inválido" << endl;
        }
    }
}

void Menu::getTurma() {
    while (true) {
        cout << "Turma: ";
        cin >> this->turma;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.clear();

        if (!manager.turmaValida(this->turma)) {
            cout << "Turma não encontrada! " << endl;
            continue;
        }
        break;
    }
}

void Menu::getUC() {
    while (true) {
        cout << "UC: ";
        cin >> this->uc;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.clear();

        if (!manager.ucValida(uc)) {
            cout << "UC não encontrada! " << endl;
            continue;
        }
        break;
    }
}

void Menu::getNuc() {
    while (true) {
        cout << "Número de UC's: ";
        string line;
        cin >> line;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.clear();

        try {
            this->nU = stoi(line);
            if (!manager.nUcValido(this->nU)) {
                cout << "Número inválido, tente novamente" << endl;
                continue;
            }
            break;
        }
        catch (exception e) {
            cout << "Número inválido, tente novamente" << endl;
        }
    }
}



void Menu::getUC_Turma() {
    while (true) {
        cout << "UC: ";
        cin >> this->uc;
        cout << "Turma: ";
        cin >> this->turma;

        if (uc.length() != 8) {
            cout << "A UC deve ter 8 caracteres" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.clear();
            continue;
        }

        if (turma.length() != 7) {
            cout << "A turma deve ter 7 caracteres" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.clear();
            continue;
        }

        if (!manager.turmaValidaNaUc(this->uc, this->turma)) {
            cout << "UC ou turma inválida. Tente novamente." << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.clear();
            continue;
        }
        break;
    }
}

void Menu::getAno() {
    while(true){
        cout << "Ano: ";
        cin >> ano;

        if (!(ano > 0 and ano < 4)) {
            cout << "O ano deve estar entre 1 e 3" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.clear();
            continue;
        }
        break;

    }
}

void Menu::getSecondStudent(){
    while (true) {
        cout << "Número do outro Aluno: ";
        string line;
        cin >> line;

        try {
            this->estudante2 = stoi(line);
            if (line.length() == 9) {
                if (!manager.estudanteValido(this->estudante2)) {
                    cout << "Aluno não encontrado! " << endl;
                    continue;
                }
            }
            break;
        }
        catch (exception e) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.clear();
            cout << "Número inválido" << endl;
        }
    }
}

void Menu::updateTime() {
    while (!exitTimeThread) {
        // Get the current time
        data = time(0);
        hora = localtime(&data);

        this_thread::sleep_for(chrono::seconds(1));
    }
}

void Menu::updatePedidos() {
    while (!exitPedidoThread) {
        pedidosAtivo = manager.getPedidos();
    }
}

void Menu::menuInicial(const tm* hora) {


    cout << "_____________________________________________________________________________________________________" << endl;
    cout << "|  "<< std::setfill('0') << std::setw(2)<<hora->tm_mday<< "/" <<std::setfill('0') << std::setw(2)<< (hora->tm_mon+1) << "/" <<std::setfill('0') << std::setw(2)<<(hora->tm_year+1900)<<"                                                                             "<<std::setfill('0') << std::setw(2)<< hora->tm_hour<< ":" <<std::setfill('0') << std::setw(2)<< hora->tm_min<<"     |" << endl;
    cout << "|                                                                                                   |" << endl;
    cout << "|  Menu Principal                                                                                   |" << endl;
    cout << "|                         Consulta                          Gestão de Horário                       |" << endl;
    cout << "|                                                                                                   |" << endl;
    cout << "|                   1 - Horário Estudante                  6 - Fazer pedido                         |" << endl;
    cout << "|                   2 - Horário UC                         7 - Executar Pedido                      |" << endl;
    cout << "|                   3 - Horário Turma                      8 - Ver histórico                        |" << endl;
    cout << "|                   4 - Listagens                          9 - Andar para trás                      |" << endl;
    cout << "|                   5 - Estatisticas                                                                |" << endl;
    cout << "|                                                                                                   |" << endl;
    cout << "|                                                                                                   |" << endl;
    cout << "|    Q - Exit                                                                        Pedidos: " << this->pedidosAtivo << "     |" << endl;
    cout << "-----------------------------------------------------------------------------------------------------" << endl;
    cout << endl;

    getUserInput();

}

void Menu::menuListagens(){

    cout << "________________________________________________________________________________________________________" <<endl;
    cout << "|    Listagens                                                                                         |" << endl;
    cout << "|                                                                                                      |" << endl;
    cout << "|                                                                                                      |" << endl;
    cout << "|                                   1 - Turmas por UC                                                  |" << endl;
    cout << "|                                   2 - Estudantes por ano                                             |" << endl;
    cout << "|                                   3 - Estudantes por UC                                              |" << endl;
    cout << "|                                   4 - Estudantes por turma                                           |" << endl;
    cout << "|                                   5 - Estudantes em pelo menos N ucs                                 |" << endl;
    cout << "|                                   6 -                                                                |" << endl;
    cout << "|                                                                                                      |" << endl;
    cout << "|   Q - Exit                                                                                           |" << endl;
    cout << "--------------------------------------------------------------------------------------------------------" << endl;

    getUserInput();
}

void Menu::menuOrdenacaoTotal() {

    cout << "________________________________________________________________________________________________________" <<endl;
    cout << "|  Ordenação                                                                                           |" << endl;
    cout << "|                                                                                                      |" << endl;
    cout << "|                                                                                                      |" << endl;
    cout << "|                                   1 - Ordenar por nº e ordem crescente                               |" << endl;
    cout << "|                                   2 - Ordenar por nº e ordem decrescente                             |" << endl;
    cout << "|                                   3 - Ordenar por nome e ordem crescente                             |" << endl;
    cout << "|                                   4 - Ordenar por nome e ordem decrescente                           |" << endl;
    cout << "|                                                                                                      |" << endl;
    cout << "|                                                                                                      |" << endl;
    cout << "|                                                                                                      |" << endl;
    cout << "|  Q - Exit                                                                                            |" << endl;
    cout << "--------------------------------------------------------------------------------------------------------" << endl;

    getUserInput();

}

void Menu::menuOrdenacaoParcial(){

    cout << "________________________________________________________________________________________________________" <<endl;
    cout << "|  Ordenação                                                                                           |" << endl;
    cout << "|                                                                                                      |" << endl;
    cout << "|                                                                                                      |" << endl;
    cout << "|                                                                                                      |" << endl;
    cout << "|                                                                                                      |" << endl;
    cout << "|                                            1 - Ordem crescente                                       |" << endl;
    cout << "|                                            2 - Ordem decrescente                                     |" << endl;
    cout << "|                                                                                                      |" << endl;
    cout << "|                                                                                                      |" << endl;
    cout << "|                                                                                                      |" << endl;
    cout << "|  Q - Exit                                                                                            |" << endl;
    cout << "--------------------------------------------------------------------------------------------------------" << endl;

    getUserInput();
}

void Menu::menuOpcoesPedidos(){
    cout << "________________________________________________________________________________________________________" <<endl;
    cout << "|  Opçoes pedidos                                                                                      |" << endl;
    cout << "|                                                                                                      |" << endl;
    cout << "|                                                                                                      |" << endl;
    cout << "|                                                                                                      |" << endl;
    cout << "|                                            1 - Troca hórario UC                                      |" << endl;
    cout << "|                                            2 - Adicionar UC                                          |" << endl;
    cout << "|                                            3 - Remover UC                                            |" << endl;
    cout << "|                                                                                                      |" << endl;
    cout << "|                                                                                                      |" << endl;
    cout << "|                                                                                                      |" << endl;
    cout << "|   Q - Exit                                                                                           |" << endl;
    cout << "--------------------------------------------------------------------------------------------------------" << endl;

    getUserInput();
}

void Menu::menuOpcoesTurmas(vector<string> lista) {
    int i = 1;
    int inicio = 0, fim = 0;
    cout << "--------------------------------" << endl;
    for (auto turma : lista) {
        int total = 30 - (to_string(i).length() + 3 + turma.length());
        if (total % 2 != 0) {
            inicio = (total / 2) + 1;
            fim = total / 2;

        }
        else {
            inicio = total / 2;
            fim = total / 2;
        }
        cout <<"|" << string(inicio, ' ') << i << " - " << turma << string(fim, ' ') << "|" << endl;
        i++;
    }
    cout << "--------------------------------" << endl;
}


void Menu::iniciar() {

    bool exitMenu = false;
    bool showMainMenu = true;
    hora = std::localtime(&data);
    timeThread = thread(&Menu::updateTime, this);
    pedidoThread = thread(&Menu::updatePedidos, this);

    while (!exitMenu) {
        if (showMainMenu) {
            menuInicial(hora);
        }

        switch (this->userInput) {
            case '1': // Hórario da Estudante
                getStudentNumber();
                cout << endl << "O hórario do Estudante " << this->numero_estudante << " é o seguinte : "<< endl;
                manager.inputToHorario('E',"","",this->numero_estudante);
                break;
            case '2': // Horário por UC
                getUC();
                cout << endl << " O hórario da UC " << this->uc <<" é o seguinte: " << endl;
                manager.inputToHorario('U',this->uc,"",0);
                break;
            case '3': // Hórario por turma
                getTurma();
                cout << endl << "O hórario da Turma " << this->turma <<  " é o seguinte: " << endl;
                manager.inputToHorario('T', "", this->turma, 0);
                break;
            case '4': // Listagens
                menuListagens();
                switch (this->userInput) {
                    case '1':// turmas por uc
                        menuOrdenacaoParcial();
                        getUC();
                        switch(userInput){
                            case '1': // listagem das turmas por uc orderm crescente
                                manager.printTurmasPorUC(uc,true);
                                break;
                            case '2': // listagem das turmas por uc orderm decrescente
                                manager.printTurmasPorUC(uc,false);
                                break;
                            case 'b': // back
                                menuListagens();
                                break;
                            case 'q': // fechar programa
                                exitMenu = true;
                                break;
                        }
                        break;
                    case '2': // estudantes por ano
                        menuOrdenacaoTotal();
                        getAno();
                        switch(this->userInput){
                            case '1':
                                manager.printEstudantesPorAno(ano,true,true);
                                break;
                            case '2':
                                manager.printEstudantesPorAno(ano,true,false);
                                break;
                            case '3':
                                manager.printEstudantesPorAno(ano, false,true);
                                break;
                            case '4':
                                manager.printEstudantesPorAno(ano,false,false);
                                break;
                            case 'b' :
                                menuListagens();
                                break;
                            case 'q' :
                                exitMenu = true;
                                break;
                        }
                        break;
                    case '3': // estudante por uc
                        menuOrdenacaoTotal();
                        switch(this->userInput){
                            case '1':
                                getUC();
                                manager.printEstudantesPorUC(uc,true,true);
                                break;
                            case '2':
                                getUC();
                                manager.printEstudantesPorUC(uc,true,false);
                                break;
                            case '3':
                                getUC();
                                manager.printEstudantesPorUC(uc,false,true);
                                break;
                            case '4':
                                getUC();
                                manager.printEstudantesPorUC(uc,false,false);
                                break;
                            case 'b' :
                                menuListagens();
                                break;
                            case 'q' :
                                exitMenu = true;
                                break;
                        }
                        break;
                    case '4' : // estudante por turma por uc
                        menuOrdenacaoTotal();
                        switch(this->userInput) {
                            case '1':
                                getUC_Turma();
                                manager.printEstudantesPorTurmaNaUc(uc, turma, true, true);
                                break;
                            case '2':
                                getUC_Turma();
                                manager.printEstudantesPorTurmaNaUc(uc, turma, true, false);
                                break;
                            case '3':
                                getUC_Turma();
                                manager.printEstudantesPorTurmaNaUc(uc, turma, false, true);
                                break;
                            case '4':
                                getUC_Turma();
                                manager.printEstudantesPorTurmaNaUc(uc, turma, false, false);
                                break;
                            case 'b':
                                menuListagens();
                                break;
                            case 'q':
                                exitMenu = true;
                                break;
                        }
                        break;

                    case '5' :
                        menuOrdenacaoTotal();
                        switch(this->userInput) {
                            case '1':
                                getNuc();
                                manager.numeroEstudantesEmPeloMenosNUCS(this->nU, true, true);
                                break;
                            case '2':
                                getNuc();
                                manager.numeroEstudantesEmPeloMenosNUCS(this->nU, true, false);
                                break;
                            case '3':
                                getNuc();
                                manager.numeroEstudantesEmPeloMenosNUCS(this->nU, false, true);
                                break;
                            case '4':
                                getNuc();
                                manager.numeroEstudantesEmPeloMenosNUCS(this->nU, false, false);
                                break;
                            case 'b':
                                menuListagens();
                                break;
                            case 'q':
                                exitMenu = true;
                                break;
                        }
                        break;

                    case 'b':
                        showMainMenu = true;
                        cout << setw(0);
                        break;

                    case 'q':
                        exitMenu = true;
                        break;


                    default:
                        cout << "Opção inválida. Tente novamente." << endl;
                        cin.clear();  // Clear the fail state
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        showMainMenu = true; // Display the menu again
                }
                break;
            case '6' : // Fazer pedidos
                menuOpcoesPedidos();
                switch (this->userInput) {
                    case '1':
                        getUC();
                        getStudentNumber();
                        getSecondStudent();
                        while (!manager.inputToPedido(this->uc, this->numero_estudante,"H",this->estudante2)){
                            getUC();
                            getStudentNumber();
                            getSecondStudent();
                        }
                        break;
                    case '2':
                        getUC();
                        getStudentNumber();
                        if (manager.validarNovaUc(this->uc, this->numero_estudante)) {
                            vector<string> turmas = manager.enviaListaDeAulaPossivel(this->uc, this->numero_estudante);
                            menuOpcoesTurmas(turmas);

                            while (true) {
                                cout << "Escolha a turma: " << endl;
                                string line;
                                cin >> line;
                                int idx;
                                try {
                                    idx = stoi(line);
                                    if (idx >= 0 && idx <= turmas.size()) {
                                        string turma = turmas[idx];
                                        manager.inputToPedido(this->uc, this->numero_estudante, "A", 0, turma);
                                        break;
                                    }
                                }
                                catch (exception e) {
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cout << "Indique um número dentro de 0 até " << turmas.size() - 1 << endl;
                                }
                            }
                        }
                        break;
                    case '3':
                        getUC();
                        getStudentNumber();
                        while (!manager.inputToPedido(this->uc, this->numero_estudante, "R")) {
                            getUC();
                            getStudentNumber();
                        }
                        break;
                    default:
                        cout << "Opção inválida. Tente novamente." << endl;
                }
                break;
            case '7' : // Proximo pedido
                manager.proximoPedido();
                break;
            case '8':
                manager.printHistorico();
                break;
            case '9':
                manager.reverterPedido();
                break;
            default:
                cout<< "Escolha uma opção válida"<<endl;


        }
    }
}


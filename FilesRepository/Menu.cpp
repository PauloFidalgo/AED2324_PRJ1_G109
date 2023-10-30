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
        cout << "Escolha uma opção: ";
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
        cout << "Número do Aluno (Escreva 'sair' para voltar atrás): ";
        string line;
        cin >> line;

        if (line == "sair") {
            this->sair = true;
            return;
        }

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

void Menu::horarioEstudante() {
    getStudentNumber();
    if (sair) {sair = false; return;}
    cout << endl << "O horário do Estudante " << this->numero_estudante << " é o seguinte:" << endl;
    manager.inputToHorario('E', "", "", this->numero_estudante);
}

void Menu::horarioUc() {
    getUC();
    if (sair) {sair = false; return;}
    cout << endl << " O hórario da uc é o seguinte: " << endl;
    manager.inputToHorario('U',this->uc,"",0);
}

void Menu::getTurma() {
    while (true) {
        cout << "Turma (Escreva 'sair para voltar atrás):";
        cin >> this->turma;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.clear();

        if (this->turma == "sair") {
            this->sair = true;
            return;
        }

        if (!manager.turmaValida(this->turma)) {
            cout << "Turma não encontrada! " << endl;
            continue;
        }
        break;
    }
}

void Menu::getUC() {
    while (true) {
        cout << "UC (Escreva 'sair' para voltar atrás): ";
        cin >> this->uc;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.clear();

        if (this->uc == "sair") {
            this->sair = true;
            return;
        }

        if (!manager.ucValida(uc)) {
            cout << "UC não encontrada! " << endl;
            continue;
        }
        break;
    }
}

void Menu::getNuc() {
    while (true) {
        cout << "Número de UC's (Escreva 'sair' para voltar atrás): ";
        string line;
        cin >> line;

        if (line == "sair") {
            this->sair = true;
            return;
        }

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

void Menu::getAno() {
    while(true){
        cout << "Ano (Escreva 'sair' para voltar atrás): ";
        string line;
        cin >> line;

        if (line == "sair") {
            this->sair = true;
            return;
        }

        try {
            ano = stoi(line);
            if (!(ano > 0 and ano < 4)) {
                cout << "O ano deve estar entre 1 e 3" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.clear();
                continue;
            }
            break;
        }
        catch (exception e) {
            cin.clear();
            cin.ignore();
        }
    }
}

void Menu::getSecondStudent(){
    while (true) {
        cout << "Número do outro Aluno (Escreva 'sair' para voltar atrás): ";
        string line;
        cin >> line;

        if (line == "sair") {
            this->sair = true;
            return;
        }

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

void Menu::trocarHorarioUc() {
    getUC();
    if (sair) {sair = false; return;}
    getStudentNumber();
    if (sair) {sair = false; return;}
    getSecondStudent();
    if (sair) {sair = false; return;}
    while (!manager.inputToPedido(this->uc, this->numero_estudante,"H",this->estudante2)){
        getUC();
        if (sair) {sair = false; return;}
        getStudentNumber();
        if (sair) {sair = false; return;}
        getSecondStudent();
        if (sair) {sair = false; return;}
    }
}

void Menu::adicionarUc(){
    getUC();
    if (sair) {sair = false; return;}
    getStudentNumber();
    if (sair) {sair = false; return;}
    if (manager.validarNovaUc(this->uc, this->numero_estudante)) {
        vector<string> turmas = manager.enviaListaDeAulaPossivel(this->uc, this->numero_estudante);
        if (!turmas.empty()) {
            menuOpcoesTurmas(turmas);
            while (true) {
                cout << "Escolha uma turma: ";
                string line;
                cin >> line;
                int idx;
                try {
                    idx = stoi(line);
                    if (idx > 0 && idx <= turmas.size()) {
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
        else {
            cout << "Não é possível adicionar um novo aluno à UC" << endl;
        }
    }
}

void Menu::removerUc() {
    getStudentNumber();
    if (sair) {sair = false; return;}
    set<pair<string,string>> ucs = manager.enviaUCParaRemover(this->numero_estudante);

    if (!ucs.empty()) {
        while (true) {
            int i = 1;
            cout << "________________________________________________________________________________________________________" << endl;
            cout << "|                                                                                                      |" << endl;
            cout << "|                                   Escolha a UC que pretende remover:                                 |" << endl;
            cout << "|                                                                                                      |" << endl;
            for (auto uc: ucs) {
                int leni = to_string(i).length();
                int len = (102 - 3 - uc.first.length() - leni) / 2;
                int lenf = (102 - 3 - uc.first.length() - leni) % 2 == 0 ? len : len + 1;
                cout << "|" << string(lenf, ' ') << i << " - " << uc.first << string(len, ' ') << "|" <<  endl;
                i++;
            }
            cout
                    << "|  m - menu                                                                                            |"
                    << endl;
            cout
                    << "|  b - anterior                                                                                        |"
                    << endl;
            cout
                    << "|  q - sair                                                                                            |"
                    << endl;
            cout
                    << "--------------------------------------------------------------------------------------------------------"
                    << endl;

            string line;
            cout << "Escolha uma opção: ";
            cin >> line;

            cin.clear();
            cin.ignore();
            if (line == "b") return;
            if (line == "m") {this->menu = true; return;}
            if (line == "q") exit(0);
            try {
                int choice = stoi(line);
                if (choice > 0 && choice <= ucs.size()) {

                    auto it = ucs.begin();
                    for (int i = 1; i < choice; i++) {
                        it++;
                    }

                    if (it != ucs.end()) {
                        manager.inputToPedido(it->first, this->numero_estudante, "R");
                    }
                }
            }
            catch (exception e) {
                cin.ignore();
                cin.clear();
                cout << "Opção inválida, tente novamente" << endl;
            }
        }
    }
}

void Menu::menuInicial(const tm* hora) {


    cout << "_____________________________________________________________________________________________________" << endl;
    cout << "|  "<< std::setfill('0') << std::setw(2)<<hora->tm_mday<< "/" <<std::setfill('0') << std::setw(2)<< (hora->tm_mon+1) << "/" <<std::setfill('0') << std::setw(2)<<(hora->tm_year+1900)<<"                                                                             "<<std::setfill('0') << std::setw(2)<< hora->tm_hour<< ":" <<std::setfill('0') << std::setw(2)<< hora->tm_min<<"     |" << endl;
    cout << "|                                          Menu Principal                                           |" << endl;
    cout << "|                                                                                                   |" << endl;
    cout << "|                         Consulta                          Gestão de Horário                       |" << endl;
    cout << "|                                                                                                   |" << endl;
    cout << "|                      1 - Horários                         4 - Fazer pedido                        |" << endl;
    cout << "|                      2 - Listagens                        5 - Executar Pedido                     |" << endl;
    cout << "|                      3 - Estatisticas                     6 - Histórico de Pedidos                |" << endl;
    cout << "|                                                           7 - Reverter Pedido                     |" << endl;
    cout << "|                                                                                                   |" << endl;
    cout << "|                                                                                                   |" << endl;
    cout << "|                                                                                                   |" << endl;
    cout << "|    q - sair                                                                        Pedidos: " << this->pedidosAtivo << "     |" << endl;
    cout << "-----------------------------------------------------------------------------------------------------" << endl;
    cout << endl;

    this->menu = false;
    getUserInput();

} 

void Menu::vereficarHorario(){
    while(true){
        cout << "____________________________________________________________________________________________________" << endl;
        cout << "|                                                                                                  |" << endl;
        cout << "|                                        Escolha a Opção:                                          |" << endl;
        cout << "|                                                                                                  |" << endl;
        cout << "|                                       1 - Horário Estudante                                      |" << endl;
        cout << "|                                       2 - Horário UC                                             |" << endl;
        cout << "|                                       3 - Horário Turma                                          |" << endl;
        cout << "|                                                                                                  |" << endl;
        cout << "|    b - anterior                                                                                  |" << endl;
        cout << "|    q - sair                                                                                      |" << endl;
        cout << "----------------------------------------------------------------------------------------------------" << endl;
        cout << endl;

        getUserInput();

        switch (this->userInput) {
            case '1':// horário estudante
                horarioEstudante();
                if (this->menu) return;
                break;
            case '2': // horário UC
                menuAno(Tipo::menuAnoUc);
                if (this->menu) return;
                break;
            case '3': // horário Turma
                menuAno(Tipo::menuAnoTurma);
                if (this->menu) return;
                break;
            case 'b' :
                return;
            case 'q':
                exit(0);
            default:
                cout << "Opção inválida. Escolha uma opção valida." << endl;

        }
    }

}

void Menu::menuTurmaPorAno(const int &ano) {
    set<string> turmas = manager.getTurmasPorAno(ano);
    while (true) {
        int i = 1;

        cout << "________________________________________________________________________________________________________" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                           Escolha a Turma:                                           |" << endl;
        cout << "|                                                                                                      |" << endl;
        for (auto turma: turmas) {
            int leni = to_string(i).length();
            int len = (102 - 3 - turma.length() - leni) / 2;
            int lenf = (102 - 3 - turma.length() - leni) % 2 == 0 ? len : len + 1;
            cout << "|" << string(lenf, ' ') << i << " - " << turma << string(len, ' ') << "|" <<  endl;
            i++;
        }
        cout << "|  m - menu                                                                                            |" << endl;
        cout << "|  b - anterior                                                                                        |" << endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;

        string line;
        cout << "Escolha uma opção: ";
        cin >> line;

        cin.clear();
        cin.ignore();

        if (line == "b") return;
        if (line == "m") {this->menu = true; return;}
        if (line == "q") exit(0);
        try {
            int choice = stoi(line);
            if (choice > 0 && choice <= turmas.size()) {

                auto it = turmas.begin();
                for (int i = 1; i < choice; i++) {
                    it++;
                }

                if (it != turmas.end()) {
                    manager.inputToHorario('T', "", *it,0);
                    break;
                }
            }
        }
        catch (exception e) {
            cin.ignore();
            cin.clear();
            cout << "Opção inválida, tente novamente" << endl;
        }
    }
}

void Menu::menuAno(Tipo tipo) {
    while(true){
        cout << "________________________________________________________________________________________________________"<< endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                            Escolha o Ano:                                            |"<< endl;
        cout << "|                                                                                                      |"<< endl;
        cout << "|                                              1 - 1º Ano                                              |"<< endl;
        cout << "|                                              2 - 2º Ano                                              |"<< endl;
        cout << "|                                              3 - 3º Ano                                              |"<< endl;
        cout << "|                                                                                                      |"<< endl;
        cout << "|  m - menu                                                                                            |" << endl;
        cout << "|  b - anterior                                                                                        |"<< endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------"<< endl;

        switch (tipo) {
            case Tipo::menuAnoTurma: {
                getUserInput();
                switch (userInput){
                    case '1':
                        menuTurmaPorAno(1);
                        if (this->menu) return;
                        break;
                    case'2':
                        menuTurmaPorAno(2);
                        if (this->menu) return;
                        break;
                    case '3':
                        menuTurmaPorAno(3);
                        if (this->menu) return;
                        break;
                    case 'm':
                        this->menu = true;
                        return;;
                    case 'b' :
                        return;
                    case 'q':
                        exit(0);
                    default:
                        cout << "Opção inválida. Escolha uma opção valida." << endl;


                }
                break;
            }
            case Tipo::menuAnoUc: {
                getUserInput();
                switch (userInput){
                    case '1':
                        menuOpçoes1Ano();
                        if (this->menu) return;
                        break;
                    case'2':
                        menuOpçoes2Ano();
                        if (this->menu) return;
                        break;
                    case '3':
                        menuOpçoes3Ano();
                        if (this->menu) return;
                        break;
                    case 'm':
                        this->menu = true;
                        return;
                    case 'b' :
                        return;
                    case 'q':
                        exit(0);
                    default:
                        cout << "Opção inválida. Escolha uma opção valida." << endl;

                }
                break;
            }
            case Tipo::menuAnoTurmaUc: {
                getUserInput();
                switch (userInput){
                    case '1':
                        menuListagemUc(1, ListagemUc::turma);
                        if (this->menu) return;
                        break;
                    case'2':
                        menuListagemUc(2, ListagemUc::turma);
                        if (this->menu) return;
                        break;
                    case '3':
                        menuListagemUc(3,ListagemUc::turma);
                        if (this->menu) return;
                        break;
                    case 'm':
                        this->menu = true;
                        return;
                    case 'b' :
                        return;
                    case 'q':
                        exit(0);
                    default:
                        cout << "Opção inválida. Escolha uma opção valida." << endl;

                }
                break;
            }
            case Tipo::menuEstudantesPorAno: {
                getUserInput();
                switch (userInput){
                    case '1':
                        ordernaçãoTotal(OrdenacaoTotal::estudantesPorAno, 1);
                        if (this->menu) return;
                        break;
                    case'2':
                        ordernaçãoTotal(OrdenacaoTotal::estudantesPorAno,2);
                        if (this->menu) return;
                        break;
                    case '3':
                        ordernaçãoTotal(OrdenacaoTotal::estudantesPorAno, 3);
                        if (this->menu) return;
                        break;
                    case 'm':
                        this->menu = true;
                        return;
                    case 'b' :
                        return;
                    case 'q':
                        exit(0);
                    default:
                        cout << "Opção inválida. Escolha uma opção valida." << endl;

                }
                break;
            }
            case Tipo::menuEstudantesPorUc: {
                getUserInput();
                switch (userInput){
                    case '1':
                        menuListagemUc(1, ListagemUc::uc);
                        if (this->menu) return;
                        break;
                    case'2':
                        menuListagemUc(2, ListagemUc::uc);
                        if (this->menu) return;
                        break;
                    case '3':
                        menuListagemUc(3, ListagemUc::uc);
                        if (this->menu) return;
                        break;
                    case 'm':
                        this->menu = true;
                        return;
                    case 'b' :
                        return;
                    case 'q':
                        exit(0);
                    default:
                        cout << "Opção inválida. Escolha uma opção valida." << endl;

                }
                break;
            }
            case Tipo::numeroDeEstudantesPorUc: {
                getUserInput();
                switch (userInput){
                    case '1':
                        menuListagemUc(1,ListagemUc::numeroUc);
                        if (this->menu) return;
                        break;
                    case'2':
                        menuListagemUc(2,ListagemUc::numeroUc);
                        if (this->menu) return;
                        break;
                    case '3':
                        menuListagemUc(3,ListagemUc::numeroUc);
                        if (this->menu) return;
                        break;
                    case 'm':
                        this->menu = true;
                        return;
                    case 'b' :
                        return;
                    case 'q':
                        exit(0);
                    default:
                        cout << "Opção inválida. Escolha uma opção valida." << endl;

                }
                break;
            }
            case Tipo::printTurmasPorUC: {
                getUserInput();
                switch (userInput){
                    case '1':
                        menuListagemUc(1,ListagemUc::printTurmas);
                        if (this->menu) return;
                        break;
                    case'2':
                        menuListagemUc(2,ListagemUc::printTurmas);
                        if (this->menu) return;
                        break;
                    case '3':
                        menuListagemUc(3,ListagemUc::printTurmas);
                        if (this->menu) return;
                        break;
                    case 'm':
                        this->menu = true;
                        return;
                    case 'b' :
                        return;
                    case 'q':
                        exit(0);
                    default:
                        cout << "Opção inválida. Escolha uma opção valida." << endl;

                }
                break;
            }
        }

    }
}

void Menu::menuOpçoes1Ano() {
    while(true){
        cout << "________________________________________________________________________________________________________" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                   Escolha a Unidade Curricular:                                      |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                               1 - Álgebra Linear e Geometria Analítica                               |" << endl;
        cout << "|                               2 - Análise Matemática I                                               |" << endl;
        cout << "|                               3 - Fundamentos da Programação                                         |" << endl;
        cout << "|                               4 - Fundamentos de Sistemas Computacionais                             |" << endl;
        cout << "|                               5 - Matemática Discreta                                                |" << endl;
        cout << "|                               6 - Projeto UP                                                         |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|  m - menu                                                                                            |" << endl;
        cout << "|  b - anterior                                                                                        |" << endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;

        getUserInput();

        switch (this->userInput) {
            case '1':// turmas por uc
                manager.inputToHorario('U',"L.EIC001","",0);
                break;
            case '2': // estudantes por ano
                manager.inputToHorario('U',"L.EIC002","",0);
                break;
            case '3': // estudante por uc
                manager.inputToHorario('U',"L.EIC003","",0);
                break;
            case '4' : // estudante por turma por uc
                manager.inputToHorario('U',"L.EIC004","",0);
                break;
            case '5' :
                manager.inputToHorario('U',"L.EIC005","",0);
                break;
            case '6':
                manager.inputToHorario('U',"UP001","",0);
                break;
            case'm':
                this->menu = true;
                return;
            case 'b' :
                return;
            case 'q':
                exit(0);
            default:
                cout << "Opção inválida. Escolha uma opção valida." << endl;

        }
    }
}

void Menu::menuOpçoes2Ano() {
    while(true){
        cout << "________________________________________________________________________________________________________" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                   Escolha a Unidade Curricular:                                      |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                               1 - Algoritmos e Estruturas de Dados                                   |"<< endl;
        cout << "|                               2 - Bases de Dados                                                     |"<< endl;
        cout << "|                               3 - Física II                                                          |"<< endl;
        cout << "|                               4 - Laboratório de Desenho e Teste de Software                         |"<< endl;
        cout << "|                               5 - Sistemas Operativos                                                |"<< endl;
        cout << "|                                                                                                      |"<< endl;
        cout << "|  m - menu                                                                                            |" << endl;
        cout << "|  b - anterior                                                                                        |"<< endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------"<< endl;

        getUserInput();

        switch (this->userInput) {
            case '1':// turmas por uc
                manager.inputToHorario('U',"L.EIC011","",0);
                break;
            case '2': // estudantes por ano
                manager.inputToHorario('U',"L.EIC012","",0);
                break;
            case '3': // estudante por uc
                manager.inputToHorario('U',"L.EIC013","",0);
                break;
            case '4' : // estudante por turma por uc
                manager.inputToHorario('U',"L.EIC014","",0);
                break;
            case '5' :
                manager.inputToHorario('U',"L.EIC015","",0);
                break;
            case'm':
                this->menu = true;
                return;
            case 'b' :
                return;
            case 'q':
                exit(0);
            default:
                cout << "Opção inválida. Escolha uma opção valida." << endl;

        }
    }
}

void Menu::menuOpçoes3Ano() {
    while(true){
        cout << "________________________________________________________________________________________________________" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                   Escolha a Unidade Curricular:                                      |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                               1 - Fundamentos de Segurança Informática                               |"<< endl;
        cout << "|                               2 - Interação Pessoa Computador                                        |"<< endl;
        cout << "|                               3 - Laboratório de Bases de Dados e Aplicações Web                     |"<< endl;
        cout << "|                               4 - Programação Funcional e em Lógica                                  |"<< endl;
        cout << "|                               5 - Redes de Computadores                                              |"<< endl;
        cout << "|                                                                                                      |"<< endl;
        cout << "|  m - menu                                                                                            |" << endl;
        cout << "|  b - anterior                                                                                        |"<< endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------"<< endl;

        getUserInput();

        switch (this->userInput) {
            case '1':// turmas por uc
                manager.inputToHorario('U',"L.EIC021","",0);
                break;
            case '2': // estudantes por ano
                manager.inputToHorario('U',"L.EIC022","",0);
                break;
            case '3': // estudante por uc
                manager.inputToHorario('U',"L.EIC023","",0);
                break;
            case '4' : // estudante por turma por uc
                manager.inputToHorario('U',"L.EIC024","",0);
                break;
            case '5' :
                manager.inputToHorario('U',"L.EIC025","",0);
                break;
            case'm':
                this->menu = true;
                return;
            case 'b' :
                return;
            case 'q':
                exit(0);
            default:
                cout << "Opção inválida. Escolha uma opção valida." << endl;

        }
    }
}

void Menu::menuListagens() {
    while(true) {
        cout << "________________________________________________________________________________________________________"<< endl;
        cout << "|                                                                                                      |"<< endl;
        cout << "|     Listagens                       Escolha uma opção:                                               |" << endl;
        cout << "|                                                                                                      |"<< endl;
        cout << "|                                     1 - Turmas por UC                                                |"<< endl;
        cout << "|                                     2 - Estudantes por Ano                                           |"<< endl;
        cout << "|                                     3 - Estudantes por UC                                            |"<< endl;
        cout << "|                                     4 - Estudantes por Turma                                         |"<< endl;
        cout << "|                                     5 - Estudantes em pelo menos N UC's                              |"<< endl;
        cout << "|                                     6 - Número de Estudantes por Turma numa UC                       |"<< endl;
        cout << "|                                     7 - Informação do Estudante                                      |" << endl;
        cout << "|                                     8 - Procurar Estudante por nome                                  |"<< endl;
        cout << "|                                     9 - Número de estudantes por UC                                  |"<< endl;
        cout << "|                                                                                                      |"<< endl;
        cout << "|  b - anterior                                                                                        |"<< endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------"<< endl;

        getUserInput();

        switch (this->userInput) {
            case '1':// turmas por uc
                menuAno(Tipo::printTurmasPorUC);
                if (this->menu) return;
                break;
            case '2': // estudantes por ano
                menuAno(Tipo::menuEstudantesPorAno);
                if (this->menu) return;
                break;
            case '3': // estudante por uc
                menuAno(Tipo::menuEstudantesPorUc);
                if (this->menu) return;
                break;
            case '4' : // estudante por turma por uc
                menuAno(Tipo::menuAnoTurmaUc);
                if (this->menu) return;
                break;
            case '5' :
                ordernaçãoTotal(OrdenacaoTotal::estudantesNucs);
                if (this->menu) return;
                break;
            case '6':
                menuAno(Tipo::numeroDeEstudantesPorUc);
                if (this->menu) return;
                break;
            case '7':
                getStudentNumber();
                if (sair) {sair = false; return;}
                manager.printInfoEstudante(this->numero_estudante);
                break;

            case '8': {
                ordenaçãoParcial(OrdenacaoParcial::pesquisaEstudantePorNome);
                if (this->menu) return;
                break;
            }
            case '9': {
                manager.printNumeroEstudantesDeTodasUc();
                break;
            }
            case 'b' :
                return;
            case 'q':
                exit(0);
            default:
                cout << "Opção inválida. Escolha uma opção valida." << endl;

        }
    }
}

void Menu::menuOpcoesPedidos(){
    while(true){
        cout << "________________________________________________________________________________________________________" <<endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                      Escolha o tipo de Pedido:                                       |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                         1 - Troca hórario UC                                         |" << endl;
        cout << "|                                         2 - Adicionar UC                                             |" << endl;
        cout << "|                                         3 - Remover UC                                               |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|  b - menu                                                                                            |"<< endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;

        getUserInput();

        switch (this->userInput) {
            case '1':
                trocarHorarioUc();
                if (this->menu) return;
                break;
            case '2':
                adicionarUc();
                if (this->menu) return;
                break;
            case '3':
                removerUc();
                if (this->menu) return;
                break;
            case 'b':
                return;
            case 'q':
                exit(0);
            default:
                cout << "Opção inválida. Tente novamente." << endl;
        }
    }
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

void Menu::menuListagemTurmasPorUc(const string& uc) {
    set<string> turmas = manager.getTurmasPorUc(uc);

    while (true) {
        int i = 1;
        cout << "________________________________________________________________________________________________________" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                           Escolha a Turma:                                           |" << endl;
        cout << "|                                                                                                      |" << endl;
        for (auto turma: turmas) {
            int leni = to_string(i).length();
            int len = (102 - 3 - turma.length() - leni) / 2;
            int lenf = (102 - 3 - turma.length() - leni) % 2 == 0 ? len : len + 1;
            cout << "|" << string(lenf, ' ') << i << " - " << turma << string(len, ' ') << "|" <<  endl;
            i++;
        }
        cout << "|  m - menu                                                                                            |" << endl;
        cout << "|  b - anterior                                                                                        |" << endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;

        string line;
        cout << "Escolha uma opção: ";
        cin >> line;

        cin.clear();
        cin.ignore();

        if (line == "b") return;
        if (line == "m") {this->menu = true; return;}
        if (line == "q") exit(0);
        try {
            int choice = stoi(line);
            if (choice > 0 && choice <= turmas.size()) {

                auto it = turmas.begin();
                for (int i = 1; i < choice; i++) {
                    it++;
                }

                if (it != turmas.end()) {
                    ordernaçãoTotal(OrdenacaoTotal::estudantesPorTurmaNaUc, 1, uc, *it);
                    if (this->menu) return;
                    break;
                }
            }
        }
        catch (exception e) {
            cin.ignore();
            cin.clear();
            cout << "Opção Inválida, tente novamente" << endl;
        }
    }
}

void Menu::menuListagemUc(const int &ano, ListagemUc listagem) {
    set<string> ucs = manager.getUcPorAno(ano);
    while (true) {
        int i = 1;
        cout << "________________________________________________________________________________________________________" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                     Escolha a Unidade Curricular:                                    |" << endl;
        cout << "|                                                                                                      |" << endl;
        for (auto uc: ucs) {
            int leni = to_string(i).length();
            int len = (102 - 3 - uc.length() - leni) / 2;
            int lenf = (102 - 3 - uc.length() - leni) % 2 == 0 ? len : len + 1;
            cout << "|" << string(lenf, ' ') << i << " - " << uc << string(len, ' ') << "|" <<  endl;
            i++;
        }
        cout << "|  m - menu                                                                                            |" << endl;
        cout << "|  b - anterior                                                                                        |" << endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;

        string line;
        cout << "Escolha uma opção: ";
        cin >> line;

        cin.clear();
        cin.ignore();

        if (line == "b") return;
        if (line == "m") {this->menu = true;  return;}
        if (line == "q") exit(0);
        try {
            int choice = stoi(line);
            if (choice > 0 && choice <= ucs.size()) {

                auto it = ucs.begin();
                for (int i = 1; i < choice; i++) {
                    it++;
                }

                if (it != ucs.end()) {
                    switch (listagem) {
                        case ListagemUc::turma: {
                            this->menuListagemTurmasPorUc(*it);
                            if (this->menu) return;
                            break;
                        }
                        case ListagemUc::uc: {
                            ordernaçãoTotal(OrdenacaoTotal::estudantesPorUc, 1, *it);
                            if (this->menu) return;
                            break;
                        }
                        case ListagemUc::numeroUc: {
                            ordernaçãoTotal(OrdenacaoTotal::numeroEstuntesPorTurmaPorUc,1, *it);
                            if (this->menu) return;
                            break;
                        }
                        case ListagemUc::printTurmas: {
                            ordenaçãoParcial(OrdenacaoParcial::printTurmasPorUc, *it);
                            if (this->menu) return;
                            break;
                        }
                    }
                    break;
                }
            }
        }
        catch (exception e) {
            cin.ignore();
            cin.clear();
            cout << "Opção inválida, tente novamente" << endl;
        }
    }
}

void Menu::iniciar() {

    hora = std::localtime(&data);
    timeThread = thread(&Menu::updateTime, this);
    pedidoThread = thread(&Menu::updatePedidos, this);

    while (true) {
        menuInicial(hora);
        switch (this->userInput) {
            case '1':  // Horário
                vereficarHorario();
                break;
            case '2': // Listagens
                menuListagens();
                break;
            case '3': // estatisticas
                menuEstatistica();
                break;
            case '4' : // Fazer pedidos
                menuOpcoesPedidos();
                break;
            case '5' : // Proximo pedido
                manager.proximoPedido();
                break;
            case '6': // Ver historico
                manager.printHistorico();
                break;
            case '7': // andar para tras
                manager.reverterPedido();
                break;
            case 'q':
                cout << "Fechando o programa" << endl;
                exit(0);
            default:
                cout << "Escolha uma opção válida" << endl;



        }
    }

}

void Menu::ordernaçãoTotal(OrdenacaoTotal ord, int ano, string uc, string turma) {
    while (true) {
        cout << "________________________________________________________________________________________________________" <<endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                    Escolha a opção de ordenação:                                     |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                 1 - Ordenar por nº e ordem crescente                                 |" << endl;
        cout << "|                                 2 - Ordenar por nº e ordem decrescente                               |" << endl;
        cout << "|                                 3 - Ordenar por nome e ordem crescente                               |" << endl;
        cout << "|                                 4 - Ordenar por nome e ordem decrescente                             |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|  m - menu                                                                                            |" << endl;
        cout << "|  b - anterior                                                                                        |"<< endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;

        getUserInput();

        switch (ord) {
            case OrdenacaoTotal::estudantesPorTurmaNaUc: {
                switch (this->userInput) {
                    case '1':
                        manager.printEstudantesPorTurmaNaUc(uc, turma, true, true);
                        break;
                    case '2':
                        manager.printEstudantesPorTurmaNaUc(uc, turma, true, false);
                        break;
                    case '3':
                        manager.printEstudantesPorTurmaNaUc(uc, turma, false, true);
                        break;
                    case '4':
                        manager.printEstudantesPorTurmaNaUc(uc, turma, false, false);
                        break;
                    case 'b':
                        return;
                    case 'm':
                        this->menu = true;
                        return;
                    case 'q':
                        exit(0);
                    default:
                        cout << "Opção inválida. Escolha uma opção valida" << endl;
                }
                break;
            }
            case OrdenacaoTotal::estudantesPorAno: {
                switch (userInput) {
                    case '1': {
                        manager.printEstudantesPorAno(ano, true, true);
                        break;
                    }
                    case '2': {
                        manager.printEstudantesPorAno(ano, true, false);
                        break;
                    }
                    case '3': {
                        manager.printEstudantesPorAno(ano, false, true);
                        break;
                    }
                    case '4': {
                        manager.printEstudantesPorAno(ano, false, false);
                        break;
                    }
                    case 'b': {
                        return;
                    }
                    case 'm': {
                        this->menu = true;
                        return;
                    }
                    case 'q': {
                        exit(0);
                    }
                }
                break;
            }
            case OrdenacaoTotal::estudantesNucs: {
                switch (this->userInput) {
                    case '1':
                        getNuc();
                        if (sair) {sair = false; return;}
                        manager.numeroEstudantesEmPeloMenosNUCS(this->nU, true, true);
                        break;
                    case '2':
                        getNuc();
                        if (sair) {sair = false; return;}
                        manager.numeroEstudantesEmPeloMenosNUCS(this->nU, true, false);
                        break;
                    case '3':
                        getNuc();
                        if (sair) {sair = false; return;}
                        manager.numeroEstudantesEmPeloMenosNUCS(this->nU, false, true);
                        break;
                    case '4':
                        getNuc();
                        if (sair) {sair = false; return;}
                        manager.numeroEstudantesEmPeloMenosNUCS(this->nU, false, false);
                        break;
                    case 'b':
                        return;
                    case 'm':
                        this->menu = true;
                        return;
                    case 'q':
                        exit(0);
                    default:
                        cout << "Opção invalida. Escolha uma opção valida" << endl;
                }
                break;
            }
            case OrdenacaoTotal::numeroEstuntesPorTurmaPorUc: {
                switch (this->userInput) {
                    case '1':
                        manager.printNumeroEstudantesPorTurmaPorUc(uc, true, true);
                        break;
                    case '2':
                        manager.printNumeroEstudantesPorTurmaPorUc(uc, true, false);
                        break;
                    case '3':
                        manager.printNumeroEstudantesPorTurmaPorUc(uc, false, true);
                        break;
                    case '4':
                        manager.printNumeroEstudantesPorTurmaPorUc(uc, false, false);
                        break;
                    case 'b' :
                        return;
                    case 'm':
                        this->menu = true;
                        return;
                    case 'q' :
                        exit(0);
                    default:
                        cout << "Opção invalida. Escolha uma opção valida"<<endl;
                }
                break;
            }
            case OrdenacaoTotal::estudantesPorUc: {
                switch (this->userInput) {
                    case '1':
                        manager.printEstudantesPorUC(uc, true, true);
                        break;
                    case '2':
                        manager.printEstudantesPorUC(uc, true, false);
                        break;
                    case '3':
                        manager.printEstudantesPorUC(uc, false, true);
                        break;
                    case '4':
                        manager.printEstudantesPorUC(uc, false, false);
                        break;
                    case 'b' :
                        return;
                    case 'm':
                        this->menu = true;;
                        return;
                    case 'q' :
                        exit(0);
                    default:
                        cout << "Opção invaldia. Escolha uma opção valida" << endl;
                }
                break;
            }

        }

    }

}

void Menu::ordenaçãoParcial(OrdenacaoParcial ord, string uc) {
    while(true){

        cout << "________________________________________________________________________________________________________" <<endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                    Escolha a opção de ordenação:                                     |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                       1 - Ordem crescente                                            |" << endl;
        cout << "|                                       2 - Ordem decrescente                                          |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|  m - menu                                                                                            |" << endl;
        cout << "|  b - anterior                                                                                        |"<< endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;

        getUserInput();
        switch (ord) {
            case OrdenacaoParcial::pesquisaEstudantePorNome: {
                switch (userInput) {
                    case '1': // listagem das turmas por uc orderm crescente
                    {
                        string nome;
                        cout << "Introduza o Nome do Estudante: ";
                        cin >> nome;
                        manager.printEstudantesPorNome(nome, true);
                        break;
                    }
                    case '2': // listagem das turmas por uc orderm decrescente
                    {
                        string nome;
                        cout << "Introduza o Nome do Estudante: ";
                        cin >> nome;
                        manager.printEstudantesPorNome(nome, false);
                        break;
                    }
                    case 'b': // back
                        return;
                    case 'q':
                        exit(0);
                    case 'm':
                        this->menu = true;;
                        return;
                    default:
                        cout << "Opção invalida. Escolha uma opção valida."<<endl;
                }
                break;
            }
            case OrdenacaoParcial::printTurmasPorUc: {
                switch (userInput) {
                    case '1': // listagem das turmas por uc orderm crescente
                        manager.printTurmasPorUC(uc, true);
                        break;
                    case '2': // listagem das turmas por uc orderm decrescente
                        manager.printTurmasPorUC(uc, false);
                        break;
                    case 'b': // back
                        return;
                    case 'q':
                        exit(0);
                    case 'm':
                        this->menu = true;;
                        return;
                    default:
                        cout << "Opção invalida. Escolha uma opção valida."<<endl;
                }
                break;
            }

        }

    }
}

void Menu::menuEstatistica() {
    while(true){
        cout << "________________________________________________________________________________________________________" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                         Menu Estatística:                                            |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                    1 - N UC's com mais alunos                                        |"<< endl;
        cout << "|                                    2 - N UC's com menos alunos                                       |"<< endl;
        cout << "|                                    3 - N Turmas com mais alunos                                      |"<< endl;
        cout << "|                                    4 - N Turmas com menos alunos                                     |"<< endl;
        cout << "|                                    5 - Número de alunos por UC                                       |"<< endl;
        cout << "|                                    6 - Número de alunos por ano                                      |"<< endl;
        cout << "|                                    7 - Número de estudantes por número de UC's                       |"<< endl;
        cout << "|                                                                                                      |"<< endl;
        cout << "|  b - anterior                                                                                        |"<< endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------"<< endl;

        getUserInput();

        switch (this->userInput) {
            case '1':

                break;
            case '2':

                break;
            case '3':

                break;
            case '4' :

                break;
            case '5' :

                break;
            case'6':
                break;
            case '7':
                break;
            case 'b' :
                return;
            case 'q':
                exit(0);
            default:
                cout << "Opção inválida. Escolha uma opção valida." << endl;

        }
    }
}
/*
 * void Menu::menuOrdenacaoParcial(){

    while(true){

        cout << "________________________________________________________________________________________________________" <<endl;
        cout << "|  Ordenação                                                                                           |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                            1 - Ordem crescente                                       |" << endl;
        cout << "|                                            2 - Ordem decrescente                                     |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|  m - Menu principal                                                                                  |" << endl;
        cout << "|  b - Menu anterior                                                                                   |"<< endl;
        cout << "|  q - Sair do programa                                                                                |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;

        getUserInput();
        switch (userInput) {
            case '1': // listagem das turmas por uc orderm crescente
                getUC();
                manager.printTurmasPorUC(uc, true);
                break;
            case '2': // listagem das turmas por uc orderm decrescente
                getUC();
                manager.printTurmasPorUC(uc, false);
                break;
            case 'b': // back
                return;
            case 'q':
                exit(0);
            case 'm':
                
return;;
                break;
            default:
                cout << "Opção invalida. Escolha uma opção valida."<<endl;
        }
    }
}
 * void Menu::menuOrdenacaoPesquisaEstudanteNome() {
    while(true){

        cout << "________________________________________________________________________________________________________" <<endl;
        cout << "|  Ordenação                                                                                           |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                            1 - Ordem crescente                                       |" << endl;
        cout << "|                                            2 - Ordem decrescente                                     |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|  m - Menu principal                                                                                  |" << endl;
        cout << "|  b - Menu anterior                                                                                   |"<< endl;
        cout << "|  q - Sair do programa                                                                                |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;

        getUserInput();
        switch (userInput) {
            case '1': // listagem das turmas por uc orderm crescente
                {
                string nome;
                cout << "Introduza o Nome do Estudante: ";
                cin >> nome;
                manager.printEstudantesPorNome(nome, true);
                break;
                }
            case '2': // listagem das turmas por uc orderm decrescente
             {
                string nome;
                cout << "Introduza o Nome do Estudante: ";
                cin >> nome;
                manager.printEstudantesPorNome(nome, false);
                break;
        }
            case 'b': // back
                return;
            case 'q':
                exit(0);
            case 'm':
                
return;;
                break;
            default:
                cout << "Opção invalida. Escolha uma opção valida."<<endl;
        }
    }
}
 * void Menu::menuOrdenacaoEstudanteUc() {
    while(true){

        cout << "________________________________________________________________________________________________________" <<endl;
        cout << "|  Ordenação                                                                                           |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                   1 - Ordenar por Turma e ordem crescente                            |" << endl;
        cout << "|                                   2 - Ordenar por Turma e ordem decrescente                          |" << endl;
        cout << "|                                   3 - Ordenar por Ocupação e ordem crescente                         |" << endl;
        cout << "|                                   4 - Ordenar por Ocupação e ordem decrescente                       |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|  m - Menu principal                                                                                  |" << endl;
        cout << "|  b - Menu anterior                                                                                   |"<< endl;
        cout << "|  q - Sair do programa                                                                                |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;

        getUserInput();

        switch (this->userInput) {
            case '1':
                getUC();
                manager.printNumeroEstudantesPorTurmaPorUc(this->uc, true, true);
                break;
            case '2':
                getUC();
                manager.printNumeroEstudantesPorTurmaPorUc(this->uc, true, false);
                break;
            case '3':
                getUC();
                manager.printNumeroEstudantesPorTurmaPorUc(this->uc, false, true);
                break;
            case '4':
                getUC();
                manager.printNumeroEstudantesPorTurmaPorUc(this->uc, false, false);
                break;
            case 'b' :
                return;
            case 'm':
                
return;;
                break;
            case 'q' :
                exit(0);
            default:
                cout << "Opção invalida. Escolha uma opção valida"<<endl;
        }
    }
}
 * void Menu::menuOrdenacaoTotalAno() {
    while(true){

        cout << "________________________________________________________________________________________________________" <<endl;
        cout << "|  Ordenação                                                                                           |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                   1 - Ordenar por nº e ordem crescente                               |" << endl;
        cout << "|                                   2 - Ordenar por nº e ordem decrescente                             |" << endl;
        cout << "|                                   3 - Ordenar por nome e ordem crescente                             |" << endl;
        cout << "|                                   4 - Ordenar por nome e ordem decrescente                           |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|  m - Menu principal                                                                                  |" << endl;
        cout << "|  b - Menu anterior                                                                                   |"<< endl;
        cout << "|  q - Sair do programa                                                                                |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;

        getUserInput();

        switch (this->userInput) {
            case '1':
                getAno();
                manager.printEstudantesPorAno(ano, true, true);
                break;
            case '2':
                getAno();
                manager.printEstudantesPorAno(ano, true, false);
                break;
            case '3':
                getAno();
                manager.printEstudantesPorAno(ano, false, true);
                break;
            case '4':
                getAno();
                manager.printEstudantesPorAno(ano, false, false);
                break;
            case 'b' :
                return;
            case 'm':
                
return;;
                break;
            case 'q' :
                exit(0);
            default:
                cout << "Opção invalida. Escolha uma opção valida"<<endl;
        }
    }
}
 void Menu::menuOrdenacaoTotalUc() {
    while(true){

        cout << "________________________________________________________________________________________________________" <<endl;
        cout << "|  Ordenação                                                                                           |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                   1 - Ordenar por nº e ordem crescente                               |" << endl;
        cout << "|                                   2 - Ordenar por nº e ordem decrescente                             |" << endl;
        cout << "|                                   3 - Ordenar por nome e ordem crescente                             |" << endl;
        cout << "|                                   4 - Ordenar por nome e ordem decrescente                           |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|  m - Menu principal                                                                                  |" << endl;
        cout << "|  b - Menu anterior                                                                                   |"<< endl;
        cout << "|  q - Sair do programa                                                                                |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;

        getUserInput();

        switch (this->userInput) {
            case '1':
                getUC();
                manager.printEstudantesPorUC(uc, true, true);
                break;
            case '2':
                getUC();
                manager.printEstudantesPorUC(uc, true, false);
                break;
            case '3':
                getUC();
                manager.printEstudantesPorUC(uc, false, true);
                break;
            case '4':
                getUC();
                manager.printEstudantesPorUC(uc, false, false);
                break;
            case 'b' :
                return;
            case 'm':
                
return;;
                break;
            case 'q' :
                exit(0);
            default:
                cout << "Opção invaldia. Escolha uma opção valida" << endl;
        }
    }
}

void Menu::menuOrdenacaoTotalTurmaUc() {
    while(true){

        cout << "________________________________________________________________________________________________________" <<endl;
        cout << "|  Ordenação                                                                                           |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                   1 - Ordenar por nº e ordem crescente                               |" << endl;
        cout << "|                                   2 - Ordenar por nº e ordem decrescente                             |" << endl;
        cout << "|                                   3 - Ordenar por nome e ordem crescente                             |" << endl;
        cout << "|                                   4 - Ordenar por nome e ordem decrescente                           |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|  m - Menu principal                                                                                  |" << endl;
        cout << "|  b - Menu anterior                                                                                   |"<< endl;
        cout << "|  q - Sair do programa                                                                                |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;

        getUserInput();

        switch (this->userInput) {
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
                return;
            case 'm':
                
return;;
                break;
            case 'q':
                exit(0);
            default:
                cout << "Opçãi invalida. Escolha uma opção valida" << endl;
        }
    }
}

void Menu::menuOrdenacaoTotalEstudantesNucs() {
    while (true) {
        cout << "________________________________________________________________________________________________________" <<endl;
        cout << "|  Ordenação                                                                                           |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                   1 - Ordenar por nº e ordem crescente                               |" << endl;
        cout << "|                                   2 - Ordenar por nº e ordem decrescente                             |" << endl;
        cout << "|                                   3 - Ordenar por nome e ordem crescente                             |" << endl;
        cout << "|                                   4 - Ordenar por nome e ordem decrescente                           |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|  m - Menu principal                                                                                  |" << endl;
        cout << "|  b - Menu anterior                                                                                   |"<< endl;
        cout << "|  q - Sair do programa                                                                                |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;

        getUserInput();

        switch (this->userInput) {
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
                return;
            case 'm':
                
return;;
                break;
            case 'q':
                exit(0);
            default:
                cout << "Opção invalida. Escolha uma opção valida" << endl;
        }
    }
}

 void Menu::menuOrdenaçãoListagemTurmaNaUc(const string &uc,const string &turma) {
    while(true){
        cout << "________________________________________________________________________________________________________" << endl;
        cout << "|  Ordenação                                                                                           |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                   1 - Ordenar por nº e ordem crescente                               |" << endl;
        cout << "|                                   2 - Ordenar por nº e ordem decrescente                             |" << endl;
        cout << "|                                   3 - Ordenar por nome e ordem crescente                             |" << endl;
        cout << "|                                   4 - Ordenar por nome e ordem decrescente                           |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|  m - Menu principal                                                                                  |" << endl;
        cout << "|  b - Menu anterior                                                                                   |"<< endl;
        cout << "|  q - Sair do programa                                                                                |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;

        getUserInput();

        switch (this->userInput) {
            case '1':
                manager.printEstudantesPorTurmaNaUc(uc, turma, true, true);
                break;
            case '2':
                manager.printEstudantesPorTurmaNaUc(uc, turma, true, false);
                break;
            case '3':
                manager.printEstudantesPorTurmaNaUc(uc, turma, false, true);
                break;
            case '4':
                manager.printEstudantesPorTurmaNaUc(uc, turma, false, false);
                break;
            case 'b':
                return;
            case 'm':
                
return;;
                break;
            case 'q':
                exit(0);
            default:
                cout << "Opçãi invalida. Escolha uma opção valida" << endl;
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
 void Menu::horarioTurma() {
    getTurma();
    cout << endl << "O horário da Turma " << this->turma << " é o seguinte: " << endl;
    manager.inputToHorario('T', "", this->turma,0);
}
 */
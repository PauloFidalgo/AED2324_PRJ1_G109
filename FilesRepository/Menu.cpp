//
// Created by Wagner Pedrosa on 25/10/2023.
//

#include "Menu.h"
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;
Menu::Menu() {
    // inicia a tabela com o tambho 24 linhas e 6 colunas
    table = std::vector<std::vector<std::string>>(24, std::vector<std::string>(6));
    Manager manager;
    this->manager = manager;
    this->manager.readFiles();

}

Menu::~Menu() {
    exitTimeThread=true;
    // Join the time update thread in the destructor
    if (timeThread.joinable()) {
        timeThread.join();
    }
}

char Menu::getUserInput() {
    while (true) {
        try {
            cout << "Escolha a opção: ";
            cin >> navegar_menu;
            cin.ignore();
            cin.clear();

            if (navegar_menu.length() == 1) {
                userInput = navegar_menu[0];
                return userInput;
            } else {
                throw std::runtime_error("escolha invalida. Tente outra vez.");
            }
        } catch (const std::exception& e) {
            cerr << e.what() << std::endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

int Menu::getStudentNumber() {
    while (true) {
        try {
            cout << "Numero Estudante: ";
            cin >> numero_estudante;

            if (to_string(numero_estudante).length() == 9) {
                while (!manager.estudanteValido(numero_estudante)) {
                    cout << "Aluno não encontrado! " << endl;
                    cout << "Numero do aluno: ";
                    cin >> numero_estudante;
                }
                return numero_estudante;
            } else {
                cout << "Número deve ter 9 dígitos. Tente novamente." << endl;
                continue;
            }
        }
        catch (std::exception &e) {
            cout << "Numero invalido. Tente novamente." << endl;
            cin.clear();  // Clear any error flags from cin
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

void Menu::getUC() {
    while (true) {
        cout << "UC: ";
        cin >> this->uc;
        cin.clear();
        cin.ignore();

        if (uc.length() != 8) {
            cout << "UC deve ter 8 dígitos. Tente novamente." << endl;
            continue;
        }
        if (!manager.ucValido(uc)) {
            cout << "UC não encontrada! " << endl;
            continue;
        }
        break;
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
            continue;
        }

        if (turma.length() != 7) {
            cout << "A turma deve ter 7 caracteres" << endl;
            continue;
        }

        if (!manager.turmaValidaNaUc(this->uc, this->turma)) {
            cout << "UC ou turma inválida. Tente novamente." << endl;
            continue;
        }
        break;
    }
}

int Menu::getAno(){
    while(true){
        cout << "Ano: ";
        cin >> ano;

        if (ano > 0 and ano < 3) {
            return ano;
        }
        else
        {
            cout << "O ano deve ser maior que zero e menor que 3." << endl;
            continue;
        }

    }
}

void Menu::createTable() {
    // preenche o cabeçalho da tabela com os dias da semana
    vector<string> days = {"     Hours", "    Monday", "    Tuesday", "   Wednesday", "   Thursday", "    Friday"};
    for (int col = 0; col < 6; col++) {
        table[0][col] = days[col];
    }

    // Fill the first column with time intervals from 8:00 to 19:00 with 30-minute intervals
    int hour = 8;
    int minute = 0;
    for (int row = 1; row < 24; row++) {
        string time, endTime;
        if (hour >= 10) {
            time = ' '+to_string(hour) + ":" + (minute == 0 ? "00" : "30");
            if (minute == 0) {
                endTime = to_string(hour) + ":30 ";
            } else {
                endTime = to_string(hour + 1) + ":00 ";
            }
        } else {
            time = "0" + to_string(hour) + ":" + (minute == 0 ? "00" : "30");
            if (minute == 0) {
                endTime = "0" + to_string(hour) + ":30";
            } else {
                if (hour + 1 >= 10) {
                    endTime = to_string(hour + 1) + ":00";
                } else {
                    endTime = "0" + to_string(hour + 1) + ":00";
                }
            }
        }

        table[row][0] =time+ " - " + endTime;

        // Increment the time by 30 minutes
        minute += 30;
        if (minute == 60) {
            hour++;
            minute = 0;
        }
    }
}

void Menu::updateTime() {
    while (!exitTimeThread) {
        // Get the current time
        data = std::time(0);
        hora = std::localtime(&data);

        // Sleep for one second (adjust as needed for your desired refresh rate)
        std::this_thread::sleep_for(std::chrono::seconds(1));
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
    cout << "|                   5 - estatisticas                                                                |" << endl;
    cout << "|                                                                                                   |" << endl;
    cout << "|                                                                                                   |" << endl;
    cout << "|    Q - Exit                                                                                       |" << endl;
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
    cout << "|                                                                                                      |" << endl;
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
    cout << "|                                                                                                      |" << endl;
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
    cout << "|                                                                                                      |" << endl;
    cout << "--------------------------------------------------------------------------------------------------------" << endl;

    getUserInput();
}

void Menu::iniciar() {

    bool exitMenu = false;
    bool showMainMenu = true;
    hora = std::localtime(&data);
    timeThread = std::thread(&Menu::updateTime, this);

    while (!exitMenu) {
        if (showMainMenu) {
            menuInicial(hora);
        }

        switch (userInput) {
            case '1': // Hórario da Estudante
                getStudentNumber();
                cout << " o hórario do estudante é o seguinte : "<< endl;
                break;
            case '2': // Horário por UC
                // getUCNumber();
                cout << " o hórario da uc é o seguinte: " << endl;
                break;
            case '3': // Hórario por turma
                // getTurmaNumber();
                cout << "o hórario da turma é o seguinte: " << endl;
                break;
            case '4': // Listagens
                menuListagens();
                switch (userInput) {
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
                        switch(userInput){
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
                        getUC();
                        switch(userInput){
                            case '1':
                                manager.printEstudantesPorUC(uc,true,true);
                                break;
                            case '2':
                                manager.printEstudantesPorUC(uc,true,false);
                                break;
                            case '3':
                                manager.printEstudantesPorUC(uc,false,true);
                                break;
                            case '4':
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
                        getUC_Turma();
                        switch(userInput) {
                            case '1':
                                manager.printEstudantesPorTurma(uc, turma, true, true);
                                break;
                            case '2':
                                manager.printEstudantesPorTurma(uc, turma, true, false);
                                break;
                            case '3':
                                manager.printEstudantesPorTurma(uc, turma, false, true);
                                break;
                            case '4':
                                manager.printEstudantesPorTurma(uc, turma, false, false);
                                break;
                            case 'b':
                                menuListagens();
                                break;
                            case 'q':
                                exitMenu = true;
                                break;
                        }
                        break;

                    case '5' : // estudante em pelo menos n Ucs
                        break;
                    case '6' : // ucs com mais estudantes
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
            default:
                cout<< "Escolha uma opção valdia 1 "<<endl;


        }
    }

}
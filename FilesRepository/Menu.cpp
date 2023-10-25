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
}
void Menu::createTable() {
    // Fill the first row with days of the week
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
void Menu::printTableTurma() {
    cout << endl;
    cout << "Horário do aluno:" << endl;
    cout << endl;
    cout << "_________________________________________________________________________________________________" << endl;

    for (int col = 0; col < 6; col++) {
        cout << left << "|" << setw(15) << table[0][col];
    }
    cout << "|" << endl;
    cout << "-------------------------------------------------------------------------------------------------" << endl;

    for (int row = 1; row < 24; row++) {
        for (int col = 0; col < 6; col++) {
            cout << left << "|" << setw(15) << table[row][col];
        }
        cout << "|" << endl;
        cout << "-------------------------------------------------------------------------------------------------" << endl;
    }
}


void Menu::printTableUC() {
    cout << endl;
    cout << "Horário da UC:" << endl;
    cout << endl;
    cout<< "_________________________________________________________________________________________________"<< endl;
    for (int col = 0; col < 6; col++) {
        cout << left << "|" << setw(15) << table[0][col];
    }
    cout << "|" << endl;
    cout<<"-------------------------------------------------------------------------------------------------"<<endl;

    for (int row = 1; row < 24; row++) {
        for (int col = 0; col < 6; col++) {
            cout << left << "|" << setw(15) << table[row][col];
        }
        cout << "|" << endl;
        cout<<"-------------------------------------------------------------------------------------------------"<<endl;
    }
}

void Menu::iniciar() {
    bool exitMenu = false;
    bool showMainMenu = true;

    while (!exitMenu) {
        if (showMainMenu) {
            cout << "Menu: " << endl;
            cout << "1 - Horário da turma" << endl;
            cout << "2 - Horário da UC" << endl;
            cout << "q - Quit" << endl;
            cout << "Escolha a opção: ";
        }

        char entrada;
        cin >> entrada;

        switch (entrada) {
            case '1':
                createTable();
                printTableTurma();
                cout << "b - Back" << endl;
                cout << "q - Quit" << endl;
                cout << "Voltar ao menu principal: ";
                showMainMenu = false;
                break;

            case '2':
                createTable();
                printTableUC();
                cout << "b - Back" << endl;
                cout << "q - Quit" << endl;
                cout << "Voltar ao menu principal: ";
                showMainMenu = false;
                break;

            case 'b':
                showMainMenu = true;
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
    }
}


#ifndef PROJECT_MENU_H
#define PROJECT_MENU_H

#include <vector>
#include <string>
#include <ctime>
#include <thread>
#include <chrono>
#include <iostream>
#include "Manager.h"

class Menu {
private:
    std::vector<std::vector<std::string>> table;
    std::time_t data = std::time(0);
    std:: tm* hora = localtime(&data);
    std:: thread timeThread;
    bool exitTimeThread;
    Manager manager;



    char userInput;
    string navegar_menu;
    int numero_estudante;
    string uc;



public:
    Menu();
    ~Menu(); // thread
    void menuInicial(const tm* hora);
    void menuOrdenacaoTotal();
    void menuOrdenacaoParcial();
    void menuListagens();
    void createTable();
    void printTableTurma();
    void printTableUC();
    void iniciar();
    void updateTime();

    char getUserInput();
    int getStudentNumber();
    string getUC();

};

#endif // PROJECT_MENU_H

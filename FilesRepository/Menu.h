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
    int estudante2;


    char userInput;
    string navegar_menu;
    int numero_estudante;
    string uc;
    string turma;
    int ano;



public:
    Menu();
    ~Menu(); // thread
    void menuInicial(const tm* hora);
    void menuOrdenacaoTotal();
    void menuOrdenacaoParcial();
    void menuListagens();
    void menuOpcoesPedidos();
    void createTable();
    void printTableTurma();
    void printTableUC();
    void iniciar();
    void updateTime();
    void getFirstStudent();
    void getSecondStudent();

    char getUserInput();
    int getStudentNumber();
    void getUC();
    string getTurma();
    void getUC_Turma();
    int getAno();
//dasda
};

#endif // PROJECT_MENU_H

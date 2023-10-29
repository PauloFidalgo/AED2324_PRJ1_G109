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
    time_t data = std::time(0);
    tm* hora = localtime(&data);
    thread timeThread;
    thread pedidoThread;
    bool exitTimeThread;
    bool exitPedidoThread;
    Manager manager;
    int estudante2;
    int pedidosAtivo;
    int nU;
    char userInput;
    int numero_estudante;
    string uc;
    string turma;
    int ano;


public:
    Menu();
    ~Menu();
    void run();// thread
    void menuInicial(const tm* hora);
    void menuOrdenacaoTotal();
    void menuOrdenacaoParcial();
    void menuListagens();
    void menuOpcoesPedidos();
    void iniciar();
    void updateTime();
    void updatePedidos();
    void getSecondStudent();
    void getUserInput();
    void getStudentNumber();
    void menuOpcoesTurmas(vector<string> lista);
    void getUC();
    void getNuc();
    void getTurma();
    void getUC_Turma();
    void getAno();
};

#endif // PROJECT_MENU_H

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
    bool showMainMenu;
    Manager manager;
    int estudante2;
    int pedidosAtivo;
    int nU;
    char userInput;
    int numero_estudante;
    string uc;
    string turma;
    int ano;
    std::vector<std::function<void(Menu&)>> menuHistory;

public:
    Menu();
    ~Menu();
    void run();

    void menuInicial(const tm* hora);
    void menuOrdenacaoTotalAno();
    void menuOrdenacaoTotalUc();
    void menuOrdenacaoTotalTurmaUc();
    void menuOrdenacaoTotalEstudantesNucs();
    void menuOrdenacaoTotalNEstudantesNUcs();
    void menuOrdenacaoParcial();
    void menuListagens();
    void menuOpcoesPedidos();
    void trocarHorarioUc();
    void adicionarUc();
    void horarioUc();
    void removerUc();
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

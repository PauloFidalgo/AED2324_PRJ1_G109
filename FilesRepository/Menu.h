#ifndef PROJECT_MENU_H
#define PROJECT_MENU_H

#include <vector>
#include <string>
#include <ctime>
#include <thread>
#include <chrono>
#include <iostream>
#include <functional>
#include "Manager.h"

enum Tipo {
    menuAnoUc,
    menuAnoTurma,
    menuAnoTurmaUc,
    menuEstudantesPorAno,
    menuEstudantesPorUc,
    numeroDeEstudantesPorUc,
    printTurmasPorUC,
    turmasEstatisticaMais,
    turmasEstatisticaMenos
};

enum OrdenacaoParcial {
    pesquisaEstudantePorNome,
    printTurmasPorUc,
    estatisticaAno,
    estisticaInscricoes,
};

enum ListagemUc {
    turma,
    uc,
    numeroUc,
    printTurmas,
    horário,
    nTurmasMais,
    nTurmasMenos,
};

enum OrdenacaoTotal {
    estudantesPorAno,
    estudantesPorUc,
    estudantesNucs,
    numeroEstuntesPorTurmaPorUc,
    estudantesPorTurmaNaUc,
};

class Menu {
private:
    time_t data = std::time(0);
    tm* hora = localtime(&data);
    thread timeThread;
    thread pedidoThread;
    bool exitTimeThread;
    bool exitPedidoThread;
    bool menu;
    bool sair;
    Manager manager;
    int estudante2;
    int pedidosAtivo;
    int nU;
    char userInput;
    int numero_estudante;
    string uc;
    string nome;
    int turma;

public:
    //Construtor
    Menu();

    //Destrutor
    ~Menu();

    //Getters

    void getSecondStudent();
    void getUserInput();
    void getStudentNumber();
    void getUC();
    void getNuc();
    void getStudentName();
    void getNTurma(const string& Uc);

    //Menus

    void iniciar();
    void menuInicial(const tm* hora);
    void menuListagens();
    void menuOpcoesPedidos();
    void menuEstatistica();
    void menuTurmaPorAno(const int &ano);
    void menuListagemTurmasPorUc(const string& uc);
    void menuListagemUc(const int &ano, ListagemUc listagem);
    void menuAno(Tipo tipo);
    void escolhaOrdenado(const bool& first);
    void menuOpcoesTurmas(map<string,list<Aula>> turmas);
    void verificarHorario();

    //Ordenações

    void ordenaçãoParcial(OrdenacaoParcial ord, string uc = "");
    void ordernaçãoTotal(OrdenacaoTotal ord, int ano = 1, string uc = "", string turma = "");

    //Execuções

    void horarioEstudante();
    void trocarHorarioUc();
    void adicionarUc();
    void removerUc();
    void updateTime();
    void updatePedidos();

};

#endif // PROJECT_MENU_H

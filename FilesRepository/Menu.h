#ifndef PROJECT_MENU_H
#define PROJECT_MENU_H

#include <vector>
#include <string>
#include <ctime>
#include <thread>
#include <chrono>
#include <iostream>
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

enum Cadeiras {
    estatistica,
    escolha,
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
};

enum OrdenacaoTotal {
    estudantesPorAno,
    estudantesPorUc,
    estudantesPorTurmaPorUc,
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
    int nTurma;
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
    void vereficarHorario();
    void menuInicial(const tm* hora);
    void menuOrdenacaoParcial();
    void menuListagens();
    void menuOrdenacaoEstudanteUc();
    void menuTurmasEstudante();
    void menuOpcoesPedidos();
    void menuEstatistica();
    void horarioEstudante();
    void horarioTurma();
    void trocarHorarioUc();
    void getNTurmas();
    void adicionarUc();
    void horarioUc();
    void removerUc();
    void iniciar();
    void updateTime();
    void updatePedidos();
    void getSecondStudent();
    void getUserInput();
    void getStudentNumber();
    void escolhaOrdenado(const bool& first);
    void menuOpcoesTurmas(vector<string> lista);
    void getUC();
    void getNuc();
    void getTurma();
    void getUC_Turma();
    void getAno();
    void menuAnoUc();
    void menuOpçoes1Ano(Cadeiras cadeira, const bool &mais = true);
    void menuOpçoes2Ano(Cadeiras cadeira, const bool &mais = true);
    void menuOpçoes3Ano(Cadeiras cadeira, const bool &mais = true);
    void menuTurmaPorAno(const int &ano);
    void menuAnoTurma();
    void menuOrdenacaoPesquisaEstudanteNome();
    void menuListagemTurmasPorUc(const string& uc);
    void menuListagemUc(const int &ano, ListagemUc listagem);
    void menuOrdenaçãoListagemTurmaNaUc(const string &uc,const string &turma);
    void menuAno(Tipo tipo);
    void menuListagemUcGetAno();
    void ordenaçãoParcial(OrdenacaoParcial ord, string uc = "");
    void ordernaçãoTotal(OrdenacaoTotal ord, int ano = 1, string uc = "", string turma = "");
};

#endif // PROJECT_MENU_H

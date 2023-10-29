//
// Created by Paulo Fidalgo on 19/10/2023.
//

#ifndef PROJECT_MANAGER_H
#define PROJECT_MANAGER_H
#include <set>
#include <queue>
#include "../DataModel/Estudante.h"
#include"../DataModel/UC.h"
#include <unordered_map>
#include "Pedido.h"
#include <stack>
#include <map>

using namespace std;

struct CompareByNumber {
    bool operator()(const Estudante& a, const Estudante& b) const {
        return a.getStudentNumber() < b.getStudentNumber();
    }
};

struct CompareByName{
    bool operator()(const Estudante& a, const Estudante& b) const {
        return a.getStudentName() < b.getStudentName();
    }
};


class Manager {
private:
    set<Estudante,CompareByNumber> estudantesNumero;
    set<Estudante,CompareByName> estudantesNome;
    set<UC> ucs;
    queue<Pedido> pedidos;
    stack<Pedido> historico;
    set<pair<int,string>> printHist;
    int nPedido;

public:
    Manager() {this->nPedido = 1;};
    void readFiles();
    void printStudents();
    int getPedidos() const;
    void printHistorico() const;
    bool nUcValido(const int &n) const;
    void printUc();
    bool addPedido(Pedido pedido);
    void executarPedidoTrocaHorario(Pedido &pedido);
    bool estudanteValido(const int &numero) const;
    void proximoPedido();
    bool trocaValida(Pedido &pedido);
    void reverterPedido();
    bool ucValida(const string &uc) const;
    bool turmaValidaNaUc(const string &uc, const string &turma);
    void testGet() const;
    Estudante getEstudante(const int &numero) const;
    bool turmaValida(const string &turma) const;
    void removerEstudanteDaUc(Pedido &pedido);
    unordered_map<string,list<Aula>> obterHorarioEstudante(const Estudante &estudante) const;
    bool verificarAulaSobreposta(const list<Aula> &horario, const Aula &aulaNova) const;
    void adicionarUcAoEstudante(Pedido &pedido);
    vector<string> enviaListaDeAulaPossivel(const string &uc, const int &estudante);
    void trocaTurma(const string &uc, const string& turma1, const int &numero1, const string &nome1, const string& turma2, const int &numero2, const string &nome2);
    list<Aula> obterHorarioEstudantePraticas(const Estudante &estudante) const;
    list<Aula> obterHorarioEstudantePraticasExceto(const Estudante &estudante, const string &uc) const;
    TurmaInfo obterInfoUc(const std::string &uc, const std::string &turma) const;
    unordered_map<string,list<Aula>> getTurmasPossiveis(const string &uc, list<Aula> &horarioPraticas);
    Aula obterPraticaUc(const string &uc, const string &turma) const;
    void addEstudanteToUc(const string &uc, const string &turma, const int &numero, const string &nome);
    void removeEstudanteFromUc(const string &uc, const string& turma, const int &numero);
    vector<pair<string,pair<string,Aula>>> getAulas(const Estudante &estudante) const;
    bool validarNovaUc(const string &uc, const int &student);
    bool removerValida(Pedido &pedido);
    bool inputToPedido(const string& uc, const int &estudante, const string &tipo, const int outro = 0, const string &turma = "");
    void printHorario();
    void printHorarioEstudante(int n);
    void printHorario(vector<pair<string,pair<string,Aula>>> horario) const;
    void inputToHorario(const char &tipo, const string &uc, const string &turma, const int &numero);
    void fakeTroca();
    void printUCS();
    void printEstudantesPorTurmaNaUc(const string& uc, const string& turma, bool orderByNumber = true, bool ascending = true) const;
    void printEstudantesPorUC(const string& uc, const bool& orderByNumber = true, const bool& ascending = true) const;
    void printEstudantesPorAno(const int& ano, const bool& orderByNumber = true, const bool& ascending = true) const;
    void printTurmasPorUC(const string& uc, const bool& ascending = true) const;
    void numeroEstudantesEmPeloMenosNUCS(const int& nUcs, const bool& orderByNumber = true, const bool& ascending = true) const;
    void printPratica(int n);
};

#endif //PROJECT_MANAGER_H

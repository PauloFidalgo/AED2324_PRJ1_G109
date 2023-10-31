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
    vector<Pedido> pedidosEspera;
    stack<Pedido> historico;
    set<pair<int,string>> printHist;
    int nPedido;

public:
    // Constructor
    Manager();

    // Initialize
    void readFiles();

    // Printers
    void printHistorico() const;
    void printHorario(vector<pair<string,pair<string,Aula>>> horario) const;
    void inputToHorario(const char &tipo, const string &uc, const string &turma, const int &numero);
    void printEstudantesPorTurmaNaUc(const string& uc, const string& turma, bool orderByNumber = true, bool ascending = true) const;
    void printEstudantesPorUC(const string& uc, const bool& orderByNumber = true, const bool& ascending = true) const;
    void printEstudantesPorAno(const int& ano, const bool& orderByNumber = true, const bool& ascending = true) const;
    void printTurmasPorUC(const string& uc, const bool& ascending = true) const;
    void numeroEstudantesEmPeloMenosNUCS(const int& nUcs, const bool& orderByNumber = true, const bool& ascending = true) const;
    void printNumeroEstudantesPorTurmaPorUc(const std::string &uc, const bool &orderByFirst = true, const bool &ascending = true) const;
    void printInfoEstudante(const int &numero) const;
    void printNumeroEstudantesDeTodasUc() const;
    void printEstudantesPorNome(string &nome, const bool &ascending) const;
    void printSets(int n, const string& uc, const bool& mais = true) const;
    void printVectors(const char &tipo = ' ', const bool &ordered = false, const bool& ascending = true) const;

    // Getters
    int getPedidos() const;
    Estudante getEstudante(const int &numero) const;
    vector<pair<string,pair<string,Aula>>> getAulas(const Estudante &estudante) const;
    map<string,list<Aula>> getTurmasPossiveis(const string &uc, list<Aula> &horarioPraticas);
    list<Aula> obterHorarioEstudantePraticas(const Estudante &estudante) const;
    list<Aula> obterHorarioEstudantePraticasExceto(const Estudante &estudante, const string &uc) const;
    TurmaInfo obterInfoUc(const std::string &uc, const std::string &turma) const;
    Aula obterPraticaUc(const string &uc, const string &turma) const;
    set<pair<string,string>> enviaUCParaRemover(const int &numero) const;
    map<string,list<Aula>> enviaListaDeAulaPossivel(const string &uc, const int &estudante);
    set<string> getUcPorAno(const int &ano) const;
    set<string> getTurmasPorAno(const int &ano) const;
    set<string> getTurmasPorUc(const string &uc) const;
    set<pair<int, string>> getOcupacaoTurmas(const string &uc) const;
    set<pair<int, string>> getOcupacaoUcs() const;
    vector<pair<int, int>> getNumeroDeAlunosPorAno() const;
    vector<pair<int, int>> getAlunosPorNIncscricoes() const;
    int getNumeroTurmas(const string& uc);



    // Validators
    bool nUcValido(const int &n) const;
    bool addPedido(Pedido pedido);
    bool estudanteValido(const int &numero) const;
    bool trocaValida(Pedido &pedido) const;
    bool ucValida(const string &uc) const;
    bool turmaValida(const string &turma) const;
    static bool verificarAulaSobreposta(const list<Aula> &horario, const Aula &aulaNova) ;
    bool validarNovaUc(const string &uc, const int &student);
    static bool removerValida(Pedido &pedido);
    bool checkAlreadyIn(vector<pair<string,pair<string,Aula>>> &horario, pair<string,pair<string,Aula>> &aula) const;
    bool verificarPedidosRepetidos(const Pedido &pedido);
    bool inputToPedido(const string& uc, const int &estudante, const string &tipo, const int outro = 0, const string &turma = "");

    // Execution
    void executarPedidoTrocaHorario(Pedido &pedido);
    void proximoPedido();
    void reverterPedido();
    void removerEstudanteDaUc(Pedido &pedido);
    void adicionarUcAoEstudante(Pedido &pedido);
    void trocaTurma(const string &uc, const string& turma1, const int &numero1, const string &nome1, const string& turma2, const int &numero2, const string &nome2);
    vector<pair<string,pair<string,Aula>>> createSobrepostas (vector<pair<string,pair<string,Aula>>> &horario) const;
    void verHorarioAntesDeConfirmar(const int &numero, const string &uc, const string &turma, const list<Aula> &aulas) const;

};

#endif //PROJECT_MANAGER_H

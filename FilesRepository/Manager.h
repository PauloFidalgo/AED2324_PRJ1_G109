//
// Created by Wagner Pedrosa on 19/10/2023.
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

public:
    void readFiles();
    void printStudents();
    void printUc();
    void addPedido(Pedido &pedido);
    void executarPedido(Pedido &pedido);
    void proximoPedido();
    bool pedidoValido(Pedido &pedido);
    void reverterPedido();
    void testGet() const;
    Estudante getEstudante(const int &numero) const;
    unordered_map<string,list<Aula>> obterHorarioEstudante(const Estudante &estudante) const;
    bool verificarAulaSobreposta(const list<Aula> &horario, const Aula &aulaNova) const;
    list<Aula> obterHorarioEstudantePraticas(const Estudante &estudante) const;
    TurmaInfo obterInfoUc(const std::string &uc, const std::string &turma) const;
    Aula obterPraticaUc(const string &uc, const string &turma) const;
    void addEstudanteToUc(const string &uc, const string &turma, const int &numero, const string &nome);
    void removeEstudanteFromUc(const string &uc, const string& turma, const int &numero);
    void printHorario();
    void printHorarioEstudante(Estudante estudante);
    void fakeTroca();
    void printEstudantesPorTurma(const string& uc, const string& turma, bool orderByNumber = true, bool ascending = true) const;
    void printEstudantesPorUC(const string& uc, const bool& orderByNumber = true, const bool& ascending = true) const;
    void printEstudantesPorAno(const int& ano, const bool& orderByNumber = true, const bool& ascending = true) const;
    void printTurmasPorUC(const string& uc, const bool& ascending = true) const;
    void numeroEstudantesEmPeloMenosNUCS(const int& nUcs, const bool& orderByNumber = true, const bool& ascending = true) const;

};

#endif //PROJECT_MANAGER_H

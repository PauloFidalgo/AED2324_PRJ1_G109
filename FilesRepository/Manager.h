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
#include "../DataModel/Slot.h"
#include "Pedido.h"
#include <stack>

using namespace std;


class Manager {
private:
    set<Estudante> estudantes;
    vector<UC> ucs;
    queue<Pedido> pedidos;
    stack<Pedido> historico;

public:
    void readFiles();
    void printStudents();
    void printUc();
    void addPedido(Pedido &pedido);
    void executarPedido();
    unordered_map<string,list<Aula>> obterHorarioEstudante(Estudante &estudante);
    TurmaInfo obterInfoUc(const std::string &uc, const std::string &turma);
    void printHorario();

};

#endif //PROJECT_MANAGER_H

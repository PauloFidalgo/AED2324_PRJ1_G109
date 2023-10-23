//
// Created by Wagner Pedrosa on 19/10/2023.
//

#ifndef PROJECT_MANAGER_H
#define PROJECT_MANAGER_H
#include <set>
#include <queue>
#include "../DataModel/Estudante.h"
#include"../DataModel/UC.h"


class Manager {
private:
    std::set<Estudante> estudantes;
    //std::queue<Pedido> pedidos;
    std::set<UC> uc;

public:
    void readFiles();
    void printStudents();



};

#endif //PROJECT_MANAGER_H

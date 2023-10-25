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

using namespace std;


class Manager {
private:
    set<Estudante> estudantes;
    vector<UC> ucs;
    //queue<Pedido> pedidos;

public:
    void readFiles();
    void printStudents();
    void printUc();



};

#endif //PROJECT_MANAGER_H

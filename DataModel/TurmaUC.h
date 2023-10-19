//
// Created by Paulo Fidalgo on 18/10/2023.
//

#ifndef PROJECT_TURMAUC_H
#define PROJECT_TURMAUC_H
#include <string>
#include <set>
#include <list>
#include "Aula.h"

class TurmaUC {
private:
    std::string nomeUC;
    std::string nomeTurma;
    std::set<int> numeroEstudante;
    std::list<Aula> aulas;

public:
    TurmaUC(const std::string &nomeUC, const std::string &nomeTurma);
    TurmaUC(const std::string &nomeUc, const std::string &nomeTurma, const std::set<int> &numeroEstudante,
            const std::list<Aula> &aulas);

    bool operator<(const TurmaUC &turmaUc) const;
};


#endif //PROJECT_TURMAUC_H

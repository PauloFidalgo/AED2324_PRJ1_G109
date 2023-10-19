//
// Created by Paulo Fidalgo on 18/10/2023.
//

#include "TurmaUC.h"

TurmaUC::TurmaUC(const std::string &nomeUC, const std::string &nomeTurma) : nomeUC(nomeUC), nomeTurma(nomeTurma){}

TurmaUC::TurmaUC(const std::string &nomeUc, const std::string &nomeTurma, const std::set<int> &numeroEstudante,
                 const std::list<Aula> &aulas) : nomeUC(nomeUc), nomeTurma(nomeTurma), numeroEstudante(numeroEstudante),
                                                 aulas(aulas) {}

bool TurmaUC::operator<(const TurmaUC &turmaUc) const {
    return this->nomeUC < turmaUc.nomeUC;
}

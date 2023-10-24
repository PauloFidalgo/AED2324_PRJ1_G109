//
// Created by Paulo Fidalgo on 18/10/2023.
//

#include "UC.h"

UC::UC(const std::string &codigoUc, const std::unordered_map<std::string, TurmaInfo> &ucTurma) : codigoUC(codigoUc), ucTurma(ucTurma) {}

UC::UC(const std::string &codigoUc) : codigoUC(codigoUc) {}

void UC::addTurma(const std::string &turma, const TurmaInfo &turmaInfo) {
    this->ucTurma.insert({turma,turmaInfo});
}

void UC::addEstudantes(const std::string &turma, const std::list<int> &estudantes) {
    auto it = ucTurma.find(turma);

    if (it != ucTurma.end()) {
        it -> second.estudantes = estudantes;
    } else {
        std::cout << "Turma não encontrada";
    }
}

const std::unordered_map<std::string, TurmaInfo> UC::getUcTurma() {
    return this->ucTurma;
}

const std::string UC::getCodigoUc() {
    return this->codigoUC;
}
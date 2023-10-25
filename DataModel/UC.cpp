//
// Created by Paulo Fidalgo on 18/10/2023.
//

#include "UC.h"

#include <utility>

UC::UC(std::string &codigoUc, const std::unordered_map<std::string, TurmaInfo> &ucTurma) : codigoUC(std::move(codigoUc)), ucTurma(ucTurma) {}

UC::UC(std::string &codigoUc) : codigoUC(std::move(codigoUc)) {}

void UC::addTurma(const std::string &turma, const TurmaInfo &turmaInfo) {
    this->ucTurma.insert({turma,turmaInfo});
}

void UC::addEstudantes(const std::string &turma, const std::list<std::pair<int,std::string>> &estudantes) {
    auto it = ucTurma.find(turma);

    if (it != ucTurma.end()) {
        it -> second.estudantes = estudantes;
    } else {
        std::cout << "Turma não encontrada";
    }
}

std::unordered_map<std::string, TurmaInfo> UC::getUcTurma() {
    return this->ucTurma;
}

Aula UC::getPratica(const std::string &turma) const {
    Aula res;
    auto it = this->ucTurma.find(turma);
    if (it != this->ucTurma.end()) {
        for (const auto& element : it->second.aulas) {
            if (element.getTipo() != "T") {
                res = element;
            }
        }
    }
    return res;
}

void UC::addEstudante(const std::string &turma, int &estudante, std::string &nome) {
    auto it = ucTurma.find(turma);

    if (it != ucTurma.end()) {
        it -> second.estudantes.emplace_back(estudante,nome);
    }
}

std::string &UC::getCodigoUc() {
    return this->codigoUC;
}
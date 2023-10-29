//
// Created by Paulo Fidalgo on 18/10/2023.
//

#include "UC.h"

#include <utility>

UC::UC(const UC &other) {
    this->codigoUC = other.codigoUC;
    this->ucTurma = other.ucTurma;
}

UC::UC(const std::string &codigoUc, const std::unordered_map<std::string, TurmaInfo> &ucTurma) : codigoUC(std::move(codigoUc)), ucTurma(ucTurma) {}

UC::UC(const std::string &codigoUc) : codigoUC(std::move(codigoUc)) {}

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

std::unordered_map<std::string, TurmaInfo> UC::getUcTurma() const {
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

std::vector<std::pair<std::string,std::pair<std::string,Aula>>> UC::getAulasUc() const {
    std::vector<std::pair<std::string,std::pair<std::string,Aula>>> res;
    for (const auto& turma : ucTurma) {
        for (const auto& aula : turma.second.aulas) {
            res.push_back({codigoUC, {turma.first, aula}});
        }
    }
    return res;
}

std::list<Aula> UC::getAulasTurma(const std::string &turma) const {
    auto it = ucTurma.find(turma);
    std::list<Aula> res;

    if (it != ucTurma.end()) {
        res = it->second.aulas;
    }
    return res;
}

void UC::addEstudante(const std::string &turma, const int &estudante, const std::string &nome) {
    auto it = ucTurma.find(turma);

    if (it != ucTurma.end()) {
        it -> second.estudantes.push_back({estudante,nome});
    }
}

std::string UC::getCodigoUc() const {
    return this->codigoUC;
}

void UC::removeEstudante(const std::string &turma, const int &numero, const std::string &nome) {
    auto it = ucTurma.find(turma);

    if (it != ucTurma.end()) {
        it -> second.estudantes.remove({numero, nome});
    }
}

bool UC::verificarTurma(const std::string &turma) const {
    auto it = ucTurma.find(turma);

    return (it != ucTurma.end());
}

bool UC::operator<(const UC uc) const {
    return this->codigoUC < uc.codigoUC;
}
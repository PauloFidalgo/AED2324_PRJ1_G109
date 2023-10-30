//
// Created by Paulo Fidalgo on 18/10/2023.
//

#include "UC.h"

#include <utility>

UC::UC(const UC &other) {
    this->codigoUC = other.codigoUC;
    this->ucTurma = other.ucTurma;
    this->media = other.media;
    this->ano = other.ano;
}

UC::UC(const std::string &codigoUc, const std::unordered_map<std::string, TurmaInfo> &ucTurma, int media) {
    this->codigoUC = codigoUc;
    this->ucTurma = ucTurma;
    this->media = media;

    auto it = ucTurma.begin();
    if (it != ucTurma.end()) {
        this->ano = std::stoi(it->first.substr(0,1));
    }
}

UC::UC(const std::string &codigoUc) : codigoUC(std::move(codigoUc)) {}

std::unordered_map<std::string, TurmaInfo> UC::getUcTurma() const {
    return this->ucTurma;
}

float UC::getMedia() const {
    return this->media;
}

int UC::getAno() const {
    return this->ano;
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

std::string UC::getCodigoUc() const {
    return this->codigoUC;
}

std::list<Aula> UC::getAulasTurma(const std::string &turma) const {
    auto it = ucTurma.find(turma);
    std::list<Aula> res;

    if (it != ucTurma.end()) {
        res = it->second.aulas;
    }
    return res;
}

int UC::getNumeroAlunos(const std::string &turma) const {
    auto it = ucTurma.find(turma);
    int res;

    if (it != ucTurma.end()) {
        res = it->second.estudantes.size();
    }
    return res;
}

void UC::addEstudante(const std::string &turma, const int &estudante, const std::string &nome) {
    auto it = ucTurma.find(turma);

    if (it != ucTurma.end()) {
        it -> second.estudantes.push_back({estudante,nome});
    }
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

int UC::getMinimum() const {
    bool first = true;
    int min;

    for (auto t : ucTurma) {
        if (first) {
            min = t.second.estudantes.size();
            first = false;
        }
        else {
            if (t.second.estudantes.size() < min) {
                min = t.second.estudantes.size();
            }
        }
    }
    return min;
}

bool UC::checkBalance(const std::string &turma) const {
   int min = this->getMinimum();

   auto it = ucTurma.find(turma);
   if ((it->second.estudantes.size() + 1) - min > 4) {
        return false;
   }
    return true;
}


bool UC::operator<(const UC uc) const {
    return this->codigoUC < uc.codigoUC;
}
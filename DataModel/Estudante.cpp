//
// Created by nelson on 05-10-2023.
//

#include "Estudante.h"
#include <iostream>

Estudante::Estudante() {}

Estudante::Estudante(const Estudante &estudante) {
    this->numero = estudante.numero;
    this->nome = estudante.nome;
    this->turmas = estudante.turmas;
    this->ano = estudante.ano;
}

Estudante::Estudante(const int& numero) : numero(numero) {}

Estudante::Estudante(const int& numero, const std::string& nome, const std::set<std::pair<std::string, std::string>> &turmas, const int& ano)  : numero(numero), nome(nome), turmas(turmas), ano(ano) {}

int const Estudante::getStudentNumber() const {
    return numero;
}

std::string const Estudante::getStudentName() const {
    return nome;
}

const std::set<std::pair<std::string, std::string>> Estudante::getTurmas() const {
    return turmas;
}

int const Estudante::getAno() const {return ano;}

void Estudante::changeTurma(const std::string &uc, const std::string &turmaNova) {
    auto it = this->turmas.lower_bound({uc,""});

    if (it->first == uc) {
        this->turmas.erase(it);
        this->turmas.insert({uc, turmaNova});
    }
}

const std::string Estudante::getTurma(const std::string &uc) const {
    auto it = turmas.lower_bound({uc,""});
    std::string res = it->second;
    return res;
}

const std::set<std::pair<std::string, std::string>> Estudante::setTurmas(std::set<std::pair<std::string, std::string>> &turmas) {
    this->turmas = turmas;
}

const bool Estudante::inscrito(std::string &uc) const {
    auto it = turmas.lower_bound({uc,""});
    if (it != turmas.end()) {
        return true;
    }
    return false;
}

void Estudante::removerUc(const std::string &uc) {
    auto it = turmas.lower_bound({uc,""});

    if (it->first == uc) {
        turmas.erase(it);
    }
}

void Estudante::adicionarUc(const std::string &uc, const std::string &turma) {
    turmas.insert({uc,turma});
}

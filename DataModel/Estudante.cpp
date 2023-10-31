//
// Created by nelson on 05-10-2023.
//

#include "Estudante.h"
#include <iostream>

Estudante::Estudante() {}

Estudante::Estudante(const int& numero) : numero(numero) {}

Estudante::Estudante(const Estudante &estudante) {
    this->numero = estudante.numero;
    this->nome = estudante.nome;
    this->turmas = estudante.turmas;
    this->ano = estudante.ano;
}

Estudante::Estudante(const int& numero, const std::string& nome, const std::set<std::pair<std::string, std::string>> &turmas, const int& ano)  : numero(numero), nome(nome), turmas(turmas), ano(ano) {}

int const Estudante::getStudentNumber() const {
    return this->numero;
}

std::string const Estudante::getStudentName() const {
    return this->nome;
}

const std::set<std::pair<std::string, std::string>> Estudante::getTurmas() const {
    return this->turmas;
}

const std::string Estudante::getTurma(const std::string &uc) const {
    auto it = turmas.lower_bound({uc,""});
    std::string res;
    if (it->first == uc) {
        res = it->second;
    }
    return res;
}

const int Estudante::getAno() const {return ano;}

const std::set<std::pair<std::string, std::string>> Estudante::setTurmas(std::set<std::pair<std::string, std::string>> &turmas) {
    this->turmas = turmas;
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

void Estudante::changeTurma(const std::string &uc, const std::string &turmaNova) {
    auto it = this->turmas.lower_bound({uc,""});

    if (it->first == uc) {
        this->turmas.erase(it);
        this->turmas.insert({uc, turmaNova});
    }
}

bool Estudante::inscrito(std::string &uc) const {
    auto it = turmas.lower_bound({uc,""});
    if (it->first == uc) {
        return true;
    }
    return false;
}

//!
bool Estudante::operator==(const Estudante &estudante) const {
    return (this->nome == estudante.nome) && (this->numero == estudante.numero) && (this->ano == estudante.ano) && (this->turmas == estudante.turmas);
}
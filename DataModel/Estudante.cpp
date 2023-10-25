//
// Created by nelson on 05-10-2023.
//

#include "Estudante.h"

Estudante::Estudante(const int& numero, const std::string& nome, const std::set<std::pair<std::string, std::string>> &turmas, const int& ano)  : numero(numero), nome(nome), turmas(turmas), ano(ano) {}

int const Estudante::getStudentNumber(){
    return numero;
}

std::string const Estudante::getStudentName(){
    return nome;
}

const std::set<std::pair<std::string, std::string>> Estudante::getTurmas() {
    return turmas;
}

int const Estudante::getAno() {return ano;}

bool Estudante::changeTurma(const std::string uc, const std::string turmaNova) {
    auto it = this->turmas.lower_bound({uc,""});

    if (it->first == uc) {
        this->turmas.erase(it);
        this->turmas.insert({uc, turmaNova});
        return true;
    }
    return false;
}

const std::string Estudante::getTurma(const std::string &uc) {
    auto it = turmas.lower_bound({uc,""});

    if (it -> first == uc) {
        return it->second;
    }
    return "";
}

const bool Estudante::inscrito(std::string &uc) {
    auto it = turmas.lower_bound({uc,""});

    return it->first == uc;
}

bool Estudante::operator<(const Estudante &estudante) const{
    return this->numero < estudante.numero;
}
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

bool Estudante::operator<(const Estudante &estudante) const{
    return this->numero < estudante.numero;
}
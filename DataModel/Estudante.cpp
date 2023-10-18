//
// Created by nelson on 05-10-2023.
//

#include "Estudante.h"

Estudante::Estudante(const int& numero, const std::string& nome, const std::set<TurmaUC> &turmas, const int& ano)  : numero(numero), nome(nome), turmas(turmas), ano(ano) {}

int const Estudante::getStudentNumber(){
    return numero;
}

std::string const Estudante::getStudentName(){
    return nome;
}

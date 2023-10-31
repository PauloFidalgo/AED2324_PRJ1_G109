//
// Created by nelson on 05-10-2023.
//

#include "Estudante.h"
#include <iostream>

Estudante::Estudante() {}

//! Construtor por número, utilizado para procurar estudantes por número
Estudante::Estudante(const int& numero) : numero(numero) {}

//! Construtor por cópia
Estudante::Estudante(const Estudante &estudante) {
    this->numero = estudante.numero;
    this->nome = estudante.nome;
    this->turmas = estudante.turmas;
    this->ano = estudante.ano;
}

//! Construtor com todos os parâmetros
Estudante::Estudante(const int& numero, const std::string& nome, const std::set<std::pair<std::string, std::string>> &turmas, const int& ano)  : numero(numero), nome(nome), turmas(turmas), ano(ano) {}

//! Retorna o número do estudante
int const Estudante::getStudentNumber() const {
    return this->numero;
}

//! Retorna o nome do estudante
std::string const Estudante::getStudentName() const {
    return this->nome;
}

//! Retorna as UC's e respetivas turmas do estudante
const std::set<std::pair<std::string, std::string>> Estudante::getTurmas() const {
    return this->turmas;
}

//! Retorna a turma do estudante numa UC O(log(n))
const std::string Estudante::getTurma(const std::string &uc) const {
    auto it = turmas.lower_bound({uc,""});
    std::string res;
    if (it->first == uc) {
        res = it->second;
    }
    return res;
}

//! Retorna o ano do estudante. O ano é determinado pela UC's de ano superior
const int Estudante::getAno() const {return ano;}

//! Adiciona um conjunto de turmas e UC's ao estudante
void Estudante::setTurmas(std::set<std::pair<std::string, std::string>> &turmas) {
    this->turmas = turmas;
}

//! Remove o estudante de uma UC O(log(n))
void Estudante::removerUc(const std::string &uc) {
    auto it = turmas.lower_bound({uc,""});

    if (it->first == uc) {
        turmas.erase(it);
    }
}

//! Atribui uma nova UC ao estudante O(log(n))
void Estudante::adicionarUc(const std::string &uc, const std::string &turma) {
    turmas.insert({uc,turma});
}

//! Altera a turma do estudante na UC (uc) para a turma nova (turmaNova) O(log(n))
void Estudante::changeTurma(const std::string &uc, const std::string &turmaNova) {
    auto it = this->turmas.lower_bound({uc,""});

    if (it->first == uc) {
        this->turmas.erase(it);
        this->turmas.insert({uc, turmaNova});
    }
}

//! Verifica se o estudante está incrito numa UC 0(log(n))
bool Estudante::inscrito(std::string &uc) const {
    auto it = turmas.lower_bound({uc,""});
    if (it->first == uc) {
        return true;
    }
    return false;
}

//! Dois estudantes são iguais se tiverem o mesmo número de estudante, nome, ano e turmas
bool Estudante::operator==(const Estudante &estudante) const {
    return (this->nome == estudante.nome) && (this->numero == estudante.numero) && (this->ano == estudante.ano) && (this->turmas == estudante.turmas);
}
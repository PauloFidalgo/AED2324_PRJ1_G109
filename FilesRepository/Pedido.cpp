//
// Created by Paulo Fidalgo on 23/10/2023.
//

#include "Pedido.h"

TipoAlteracao stringToTipo(const std::string &tipo) {
    if (tipo == "R") {
        return TipoAlteracao::R;
    }

    if (tipo == "A") {
        return TipoAlteracao::A;
    }

    return TipoAlteracao::H;
}

Pedido::Pedido(const std::string &uc, const Estudante &estudante, const Estudante &outroEstudante) {
    this->uc = uc;
    this->estudante = estudante;
    this->outroEstudante = outroEstudante;
    this->tipo = TipoAlteracao::H;
}

Pedido::Pedido(const std::string &uc, const Estudante &estudante, const std::string &tipo, const std::string &turma) {
    this->uc = uc;
    this->estudante = estudante;
    this->tipo = stringToTipo(tipo);
    this->turma = turma;
}

Estudante Pedido::getEstudante() {
    return this->estudante;
}

Estudante Pedido::getOutroEstudante() {
    return this->outroEstudante;
}

std::string Pedido::getUc() const {
    return this->uc;
}

std::string Pedido::getTurma() const {
    return this->turma;
}

TipoAlteracao Pedido::getTipoAlteracao() const {
    return this->tipo;
}


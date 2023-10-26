//
// Created by Paulo Fidalgo on 23/10/2023.
//

#include "Pedido.h"

TipoAlteracao stringToTipo(std::string &tipo) {
    if (tipo == "R") {
        return TipoAlteracao::R;
    }

    if (tipo == "A") {
        return TipoAlteracao::A;
    }

    return TipoAlteracao::H;
}

Pedido::Pedido(std::string &uc, Estudante &estudante, Estudante &outroEstudante) {
    this->uc = uc;
    this->estudante = estudante;
    this->outroEstudante = outroEstudante;
    this->tipo = TipoAlteracao::H;
}

Pedido::Pedido(std::string &uc, Estudante &estudante, std::string &tipo) {
    this->uc = uc;
    this->estudante = estudante;
    this->tipo = stringToTipo(tipo);
}

Estudante Pedido::getEstudante() {
    return this->estudante;
}

Estudante Pedido::getOutroEstudante() {
    return this->outroEstudante;
}

std::string Pedido::getUc() {
    return this->uc;
}

TipoAlteracao Pedido::getTipoAlteracao() const {
    return this->tipo;
}


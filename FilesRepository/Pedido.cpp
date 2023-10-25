//
// Created by Paulo Fidalgo on 23/10/2023.
//

#include "Pedido.h"

Pedido::Pedido(std::string &uc, Estudante &estudante, Estudante &outroEstudante) : uc(uc), estudante(estudante), outroEstudante(outroEstudante) {}

Estudante Pedido::getEstudante() {
    return this->estudante;
}

Estudante Pedido::getOutroEstudante() {
    return this->outroEstudante;
}

std::string Pedido::getUc() {
    return this->uc;
}

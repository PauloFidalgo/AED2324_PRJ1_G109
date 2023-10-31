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

void Pedido::setTurma(const std::string &turma) {
    this->turma = turma;
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

std::string Pedido::pedidoToString() const {
    std::string res;
    switch (this->tipo) {
        case TipoAlteracao::R: {
            res = "Removeu o estudante " + this->estudante.getStudentName() + " com o número " + std::to_string(this->estudante.getStudentNumber()) + " da UC " + this->uc + "\n";
            break;
        }
        case TipoAlteracao::A: {
            res = "Adicionou o estudante " + this->estudante.getStudentName() + " com o número " + std::to_string(this->estudante.getStudentNumber()) + " à UC " + this->uc + "\n";
            break;
        }
        case TipoAlteracao::H: {
            res = "Trocou a turma do estudante " + this->estudante.getStudentName() + " com o número " + std::to_string(this->estudante.getStudentNumber()) + " com o aluno " + this->outroEstudante.getStudentName() + " com o número " + std::to_string(this->outroEstudante.getStudentNumber()) + " na UC " + this->uc + "\n";
            break;
        }
    }
    return res;
}

bool Pedido::pedidosConcorrentes(const Pedido &pedido) {
    switch (pedido.tipo) {
        case TipoAlteracao::R: {
            if (this->estudante.getStudentNumber() == pedido.estudante.getStudentNumber() && this->uc == pedido.uc) {
                std::cout << "Já existe um pedido para remover o estudante " << estudante.getStudentNumber() << " da UC " << uc << std::endl;
                return false;
            }
            break;
        }
        case TipoAlteracao::H: {
            // Se um dos estudantes tiver um pedido de remoção desta UC em específico não podem trocas
            if (this->tipo == TipoAlteracao::R) {
                if (this->estudante.getStudentNumber() == pedido.estudante.getStudentNumber() && this->uc == pedido.uc) {
                    std::cout << "O estudante " << pedido.estudante.getStudentNumber() << " tem um pedido para remover a UC " << this->uc << std::endl;
                    return false;
                }
                if (this->estudante.getStudentNumber() == pedido.outroEstudante.getStudentNumber() && this->uc == pedido.uc) {
                    std::cout << "O estudante " << pedido.outroEstudante.getStudentNumber() << " tem um pedido para remover a UC " << this->uc << std::endl;
                    return false;
                }
            }
            break;
        }
    }
    return true;
}

bool Pedido::operator==(const Pedido &other) const {
    return this->estudante == other.estudante && this->outroEstudante == other.outroEstudante && this->uc == other.uc && this->tipo == other.tipo;
}

//
// Created by Paulo Fidalgo on 23/10/2023.
//

#include "Pedido.h"

//! Função auxiliar que retorna o Tipo de pedido consoante a string que receber
TipoAlteracao stringToTipo(const std::string &tipo) {
    if (tipo == "R") {
        return TipoAlteracao::R;
    }

    if (tipo == "A") {
        return TipoAlteracao::A;
    }

    return TipoAlteracao::H;
}

//! Construtor
Pedido::Pedido(const std::string &uc, const Estudante &estudante, const Estudante &outroEstudante) {
    this->uc = uc;
    this->estudante = estudante;
    this->outroEstudante = outroEstudante;
    this->tipo = TipoAlteracao::H;
}

//! Construtor
Pedido::Pedido(const std::string &uc, const Estudante &estudante, const std::string &tipo, const std::string &turma) {
    this->uc = uc;
    this->estudante = estudante;
    this->tipo = stringToTipo(tipo);
    this->turma = turma;
}

//! Retorna o primeiro estudante do pedido
Estudante Pedido::getEstudante() const {
    return this->estudante;
}

//! Atribui um valor ao atributo turma
void Pedido::setTurma(const std::string &turma) {
    this->turma = turma;
}

//! Retorna o segundo estudante do pedido
Estudante Pedido::getOutroEstudante() const {
    return this->outroEstudante;
}

//! Retorna a UC do pedido
std::string Pedido::getUc() const {
    return this->uc;
}

//! Retorna a turma do pedido
std::string Pedido::getTurma() const {
    return this->turma;
}

//! Retorna o tipo de alteração do pedido
TipoAlteracao Pedido::getTipoAlteracao() const {
    return this->tipo;
}

//! Faz a conversão do pedido para string
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

//! Verifica se dois pedidos são concorrentes. Retorna true caso não sejam e false caso contrário
bool Pedido::pedidosConcorrentes(const Pedido &pedido) {
    switch (pedido.tipo) {
        case TipoAlteracao::R: {
            //! Caso seja um pedido de remoção, é necessário verificar se já existe um pedido pendente igual. Caso existe não é permitido fazer outro igual da mesma UC
            if (this->estudante.getStudentNumber() == pedido.estudante.getStudentNumber() && this->uc == pedido.uc) {
                std::cout << "Já existe um pedido para remover o estudante " << estudante.getStudentNumber() << " da UC " << uc << std::endl;
                return false;
            }
            break;
        }
        case TipoAlteracao::H: {
            //! Se um dos estudantes tiver um pedido de remoção desta UC em específico não podem trocar
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

//! Compara dois pedidos. Dois pedidos são iguais se o tipo, os estudantes e a uc forem iguais.
bool Pedido::operator==(const Pedido &other) const {
    return this->estudante == other.estudante && this->outroEstudante == other.outroEstudante && this->uc == other.uc && this->tipo == other.tipo;
}

//
// Created by Paulo Fidalgo on 23/10/2023.
//

#ifndef PROJECT_PEDIDO_H
#define PROJECT_PEDIDO_H


#include "../DataModel/Estudante.h"
#include <stack>
#include <iostream>

enum TipoAlteracao {H, A, R};

class Pedido {
private:
    std::string uc, turma;
    Estudante estudante, outroEstudante;
    TipoAlteracao tipo;


public:
    //! Constructors
    Pedido(const std::string &uc, const Estudante &estudante, const Estudante &outroEstudante);
    Pedido(const std::string &uc, const Estudante &estudante, const std::string &tipo, const std::string &turma = "");

    //! Getters
    Estudante getEstudante() const;
    Estudante getOutroEstudante() const;
    std::string getUc() const;
    std::string getTurma() const;
    TipoAlteracao getTipoAlteracao() const;

    //! Setters
    void setTurma(const std::string &turma);

    //! To string
    std::string pedidoToString() const;

    //! Validators
    bool pedidosConcorrentes(const Pedido &pedido);
    bool operator==(const Pedido &other) const;
};


#endif //PROJECT_PEDIDO_H

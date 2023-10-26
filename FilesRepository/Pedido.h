//
// Created by Paulo Fidalgo on 23/10/2023.
//

#ifndef PROJECT_PEDIDO_H
#define PROJECT_PEDIDO_H


#include "../DataModel/Estudante.h"
#include <stack>

enum TipoAlteracao {H, A, R};

class Pedido {
private:
    std::string uc;
    Estudante estudante, outroEstudante;
    TipoAlteracao tipo;

public:
    Pedido(std::string &uc, Estudante &estudante, Estudante &outroEstudante);
    Pedido(std::string &uc, Estudante &estudante, std::string &tipo);
    Estudante getEstudante();
    Estudante getOutroEstudante();
    std::string getUc();
    TipoAlteracao getTipoAlteracao() const;
};


#endif //PROJECT_PEDIDO_H

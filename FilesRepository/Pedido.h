//
// Created by Paulo Fidalgo on 23/10/2023.
//

#ifndef PROJECT_PEDIDO_H
#define PROJECT_PEDIDO_H


#include "../DataModel/Estudante.h"
#include <stack>

class Pedido {
private:
    std::string uc;
    Estudante estudante, outroEstudante;

public:
    Pedido(std::string uc, Estudante estudante, Estudante outroEstudante);
    bool executar();
    Estudante getEstudante();
    Estudante getOutroEstudante();
    std::string getUc();
};


#endif //PROJECT_PEDIDO_H

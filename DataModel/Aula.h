//
// Created by Paulo Fidalgo on 05/10/2023.
//

#ifndef AED2324_PRJ1_G_109__COURSEUNIT_H
#define AED2324_PRJ1_G_109__COURSEUNIT_H
#include <string>
#include <iostream>
#include "Slot.h"

enum Type {TP, P, PL};

class Aula {
private:
    std::string dia;
    float inicio, duracao;
    std::string tipo;

public:
    Aula();
    Aula(std::string dia, float inicio, float duracao, std::string tipo);

    const void printData();
};




#endif //AED2324_PRJ1_G_109__COURSEUNIT_H

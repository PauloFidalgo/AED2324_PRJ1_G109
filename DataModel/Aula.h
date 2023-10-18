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
    Slot slot;
    std::string type;

public:
    Aula(const std::string &type, const Slot &slot);
};




#endif //AED2324_PRJ1_G_109__COURSEUNIT_H

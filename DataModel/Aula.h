//
// Created by Paulo Fidalgo on 05/10/2023.
//

#ifndef AED2324_PRJ1_G_109__COURSEUNIT_H
#define AED2324_PRJ1_G_109__COURSEUNIT_H
#include <string>
#include <iostream>


enum Type {TP, P, PL};

class Aula {
private:
    std::string dia;
    float inicio, duracao;
    std::string tipo;

public:
    Aula();
    Aula(std::string dia, float inicio, float duracao, std::string tipo);

    const void printData() const;
    const bool sobreposta(const Aula &aula) const;
    const std::string getTipo() const;
    const std::string getDia() const;
    const float getDuracao() const;
    const float getInicio() const;

};




#endif //AED2324_PRJ1_G_109__COURSEUNIT_H
